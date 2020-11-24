/*
 * Copyright (c) 2000-2008 Chih-Chung Chang and Chih-Jen Lin
 * All rights reserved.
 *
 * Modified for use with libstem by Nedim Srndic and Pavel Laskov.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither name of copyright holders nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <string.h>
#include <stdarg.h>
#include <algorithm>
#include "libsvm.h"

typedef double Qfloat;
typedef signed char schar;

inline double powi(double base, int times) {
	double tmp = base, ret = 1.0;

	for (int t = times; t > 0; t /= 2) {
		if (t % 2 == 1)
			ret *= tmp;
		tmp = tmp * tmp;
	}
	return ret;
}

#define INF HUGE_VAL
#define TAU 1e-12
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

void info(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

void info_flush() {
	fflush(stdout);
}

//
// Kernel Cache
//
// l is the number of total data items
// size is the cache size limit in bytes
//
class Cache {
public:
	Cache(int l, long int size);
	~Cache();

	// request data [0,len)
	// return some position p where [p,len) need to be filled
	// (p >= len if nothing needs to be filled)
	int get_data(const int index, Qfloat ** data, int len);
	void swap_index(int i, int j); // future_option
private:
	int l;
	long int size;
	struct head_t {
		head_t *prev, *next; // a cicular list
		Qfloat *data;
		int len; // data[0,len) is cached in this entry
	};

	head_t *head;
	head_t lru_head;
	void lru_delete(head_t * h);
	void lru_insert(head_t * h);
};

Cache::Cache(int l_, long int size_) :
	l(l_), size(size_) {
	head = (head_t *) calloc(l, sizeof(head_t)); // initialized to 0
	size /= sizeof(Qfloat);
	size -= l * sizeof(head_t) / sizeof(Qfloat);
	size = std::max(size, 2 * (long int) l); // cache must be large enough for two columns
	lru_head.next = lru_head.prev = &lru_head;
}

Cache::~Cache() {
	for (head_t * h = lru_head.next; h != &lru_head; h = h->next)
		free(h->data);
	free(head);
}

void Cache::lru_delete(head_t * h) {
	// delete from current location
	h->prev->next = h->next;
	h->next->prev = h->prev;
}

void Cache::lru_insert(head_t * h) {
	// insert to last position
	h->next = &lru_head;
	h->prev = lru_head.prev;
	h->prev->next = h;
	h->next->prev = h;
}

int Cache::get_data(const int index, Qfloat ** data, int len) {
	head_t *h = &head[index];
	if (h->len)
		lru_delete(h);
	int more = len - h->len;

	if (more > 0) {
		// free old space
		while (size < more) {
			head_t *old = lru_head.next;
			lru_delete(old);
			free(old->data);
			size += old->len;
			old->data = 0;
			old->len = 0;
		}

		// allocate new space
		h->data = (Qfloat *) realloc(h->data, sizeof(Qfloat) * len);
		size -= more;
		std::swap(h->len, len);
	}

	lru_insert(h);
	*data = h->data;
	return len;
}

void Cache::swap_index(int i, int j) {
	if (i == j)
		return;

	if (head[i].len)
		lru_delete(&head[i]);
	if (head[j].len)
		lru_delete(&head[j]);
	std::swap(head[i].data, head[j].data);
	std::swap(head[i].len, head[j].len);
	if (head[i].len)
		lru_insert(&head[i]);
	if (head[j].len)
		lru_insert(&head[j]);

	if (i > j)
		std::swap(i, j);
	for (head_t * h = lru_head.next; h != &lru_head; h = h->next) {
		if (h->len > i) {
			if (h->len > j)
				std::swap(h->data[i], h->data[j]);
			else {
				// give up
				lru_delete(h);
				free(h->data);
				size += h->len;
				h->data = 0;
				h->len = 0;
			}
		}
	}
}

//
// Kernel evaluation
//
// the static method k_function is for doing single kernel evaluation
// the constructor of Kernel prepares to calculate the l*l kernel matrix
// the member function get_Q is for getting one column from the Q Matrix
//
class QMatrix {
public:
	virtual Qfloat * get_Q(int column, int len) const = 0;
	virtual Qfloat *get_QD() const = 0;
	virtual void swap_index(int i, int j) const = 0;
	virtual ~ QMatrix() {
	}
};

class Kernel: public QMatrix {
public:
	Kernel(int l, stem::NGram * const *x, const svm_parameter & param);
	virtual ~ Kernel();

	static double k_function(const stem::NGram * x, const stem::NGram * y,
			const svm_parameter & param);
	virtual Qfloat *get_Q(int column, int len) const = 0;
	virtual Qfloat *get_QD() const = 0;
	virtual void swap_index(int i, int j) const // no so const...
	{
		std::swap(x[i], x[j]);
	}
protected:

	double (Kernel::*kernel_function)(int i, int j) const;

private:
	stem::NGram **x;
	stem::Kernel *k;

	double kernel(int i, int j) const {
		//printf("kern_lin(%d, %d)\n", i, j);
		return k->compute(*(x[i]), *(x[j]));
	}
};

Kernel::Kernel(int l, stem::NGram * const *x_, const svm_parameter & param) {
	//printf("Kernel1\n");
	this->k = param.kernel;
	kernel_function = &Kernel::kernel;

	//printf("Kernel2\n");
	x = new stem::NGram *[l];
	std::copy(x_, x_ + l, x);
	//printf("Kernel3\n");
}

Kernel::~Kernel() {
	delete[] x;
}

double Kernel::k_function(const stem::NGram * x, const stem::NGram * y,
		const svm_parameter & param) {
	return param.kernel->compute(*x, *y);
}

// An SMO algorithm in Fan et al., JMLR 6(2005), p. 1889--1918
// Solves:
//
//      min 0.5(\alpha^T Q \alpha) + p^T \alpha
//
//              y^T \alpha = \delta
//              y_i = +1 or -1
//              0 <= alpha_i <= Cp for y_i = 1
//              0 <= alpha_i <= Cn for y_i = -1
//
// Given:
//
//      Q, p, y, Cp, Cn, and an initial feasible point \alpha
//      l is the size of vectors and matrices
//      eps is the stopping tolerance
//
// solution will be put in \alpha, objective value will be put in obj
//
class Solver {
public:
	Solver() {
	}
	virtual ~ Solver() {
	}

	struct SolutionInfo {
		double obj;
		double rho;
		double upper_bound_p;
		double upper_bound_n;
		double r; // for Solver_NU
		//double radius;			// for Solver SVDD, removed by PL
	};

	void Solve(int l, const QMatrix & Q, const double *p_, const schar * y_,
			double *alpha_, double Cp, double Cn, double eps,
			SolutionInfo * si, int shrinking);
protected:
	int active_size;
	schar *y;
	double *G; // gradient of objective function
	enum {
		LOWER_BOUND, UPPER_BOUND, FREE
	};
	char *alpha_status; // LOWER_BOUND, UPPER_BOUND, FREE
	double *alpha;
	const QMatrix *Q;
	const Qfloat *QD;
	double eps;
	double Cp, Cn;
	double *p;
	int *active_set;
	double *G_bar; // gradient, if we treat free variables as 0
	int l;
	bool unshrinked; // XXX

	double get_C(int i) {
		return (y[i] > 0) ? Cp : Cn;
	}
	void update_alpha_status(int i) {
		if (alpha[i] >= get_C(i))
			alpha_status[i] = UPPER_BOUND;
		else if (alpha[i] <= 0)
			alpha_status[i] = LOWER_BOUND;
		else
			alpha_status[i] = FREE;
	}
	bool is_upper_bound(int i) {
		return alpha_status[i] == UPPER_BOUND;
	}
	bool is_lower_bound(int i) {
		return alpha_status[i] == LOWER_BOUND;
	}
	bool is_free(int i) {
		return alpha_status[i] == FREE;
	}
	void swap_index(int i, int j);
	void reconstruct_gradient();
	virtual int select_working_set(int &i, int &j);
	virtual double calculate_rho();
	virtual void do_shrinking();
private:
	bool be_shrunken(int i, double Gmax1, double Gmax2);
};

void Solver::swap_index(int i, int j) {
	Q->swap_index(i, j);
	std::swap(y[i], y[j]);
	std::swap(G[i], G[j]);
	std::swap(alpha_status[i], alpha_status[j]);
	std::swap(alpha[i], alpha[j]);
	std::swap(p[i], p[j]);
	std::swap(active_set[i], active_set[j]);
	std::swap(G_bar[i], G_bar[j]);
}

void Solver::reconstruct_gradient() {
	// reconstruct inactive elements of G from G_bar and free variables

	if (active_size == l)
		return;

	int i;
	for (i = active_size; i < l; i++)
		G[i] = G_bar[i] + p[i];

	for (i = 0; i < active_size; i++)
		if (is_free(i)) {
			const Qfloat *Q_i = Q->get_Q(i, l);
			double alpha_i = alpha[i];
			for (int j = active_size; j < l; j++)
				G[j] += alpha_i * Q_i[j];
		}
}

void Solver::Solve(int l, const QMatrix & Q, const double *p_,
		const schar * y_, double *alpha_, double Cp, double Cn, double eps,
		SolutionInfo * si, int shrinking) {
	this->l = l;
	this->Q = &Q;
	QD = Q.get_QD();
	p = new double[l];
	std::copy(p_, p_ + l, p);
	y = new schar[l];
	std::copy(y_, y_ + l, y);
	alpha = new double[l];
	std::copy(alpha_, alpha_ + l, alpha);
	this->Cp = Cp;
	this->Cn = Cn;
	this->eps = eps;
	unshrinked = false;

	// initialize alpha_status
	{
		alpha_status = new char[l];
		for (int i = 0; i < l; i++)
			update_alpha_status(i);
	}

	// initialize active set (for shrinking)
	{
		active_set = new int[l];
		for (int i = 0; i < l; i++)
			active_set[i] = i;
		active_size = l;
	}

	// initialize gradient
	{
		G = new double[l];
		G_bar = new double[l];
		int i;
		for (i = 0; i < l; i++) {
			G[i] = p[i];
			G_bar[i] = 0;
		}
		for (i = 0; i < l; i++)
			if (!is_lower_bound(i)) {
				const Qfloat *Q_i = Q.get_Q(i, l);
				double alpha_i = alpha[i];
				int j;
				for (j = 0; j < l; j++)
					G[j] += alpha_i * Q_i[j];
				if (is_upper_bound(i))
					for (j = 0; j < l; j++)
						G_bar[j] += get_C(i) * Q_i[j];
			}
	}

	// optimization step

	int iter = 0;
	int counter = std::min(l, 1000) + 1;

	while (1) {
		// show progress and do shrinking
		if (--counter == 0) {
			counter = std::min(l, 1000);
			if (shrinking)
				do_shrinking();
			info(".");
			info_flush();
		}

		int i, j;
		if (select_working_set(i, j) != 0) {
			// reconstruct the whole gradient
			reconstruct_gradient();
			// reset active set size and check
			active_size = l;
			info("*");
			info_flush();
			if (select_working_set(i, j) != 0)
				break;
			else
				counter = 1; // do shrinking next iteration
		}

		++iter;

		// update alpha[i] and alpha[j], handle bounds carefully

		const Qfloat *Q_i = Q.get_Q(i, active_size);
		const Qfloat *Q_j = Q.get_Q(j, active_size);

		double C_i = get_C(i);
		double C_j = get_C(j);

		double old_alpha_i = alpha[i];
		double old_alpha_j = alpha[j];

		if (y[i] != y[j]) {
			double quad_coef = Q_i[i] + Q_j[j] + 2 * Q_i[j];
			if (quad_coef <= 0)
				quad_coef = TAU;
			double delta = (-G[i] - G[j]) / quad_coef;
			double diff = alpha[i] - alpha[j];
			alpha[i] += delta;
			alpha[j] += delta;

			if (diff > 0) {
				if (alpha[j] < 0) {
					alpha[j] = 0;
					alpha[i] = diff;
				}
			} else {
				if (alpha[i] < 0) {
					alpha[i] = 0;
					alpha[j] = -diff;
				}
			}
			if (diff > C_i - C_j) {
				if (alpha[i] > C_i) {
					alpha[i] = C_i;
					alpha[j] = C_i - diff;
				}
			} else {
				if (alpha[j] > C_j) {
					alpha[j] = C_j;
					alpha[i] = C_j + diff;
				}
			}
		} else {
			double quad_coef = Q_i[i] + Q_j[j] - 2 * Q_i[j];
			if (quad_coef <= 0)
				quad_coef = TAU;
			double delta = (G[i] - G[j]) / quad_coef;
			double sum = alpha[i] + alpha[j];
			alpha[i] -= delta;
			alpha[j] += delta;

			if (sum > C_i) {
				if (alpha[i] > C_i) {
					alpha[i] = C_i;
					alpha[j] = sum - C_i;
				}
			} else {
				if (alpha[j] < 0) {
					alpha[j] = 0;
					alpha[i] = sum;
				}
			}
			if (sum > C_j) {
				if (alpha[j] > C_j) {
					alpha[j] = C_j;
					alpha[i] = sum - C_j;
				}
			} else {
				if (alpha[i] < 0) {
					alpha[i] = 0;
					alpha[j] = sum;
				}
			}
		}

		// update G

		double delta_alpha_i = alpha[i] - old_alpha_i;
		double delta_alpha_j = alpha[j] - old_alpha_j;

		for (int k = 0; k < active_size; k++) {
			G[k] += Q_i[k] * delta_alpha_i + Q_j[k] * delta_alpha_j;
		}

		// update alpha_status and G_bar

		{
			bool ui = is_upper_bound(i);
			bool uj = is_upper_bound(j);
			update_alpha_status(i);
			update_alpha_status(j);
			int k;
			if (ui != is_upper_bound(i)) {
				Q_i = Q.get_Q(i, l);
				if (ui)
					for (k = 0; k < l; k++)
						G_bar[k] -= C_i * Q_i[k];
				else
					for (k = 0; k < l; k++)
						G_bar[k] += C_i * Q_i[k];
			}

			if (uj != is_upper_bound(j)) {
				Q_j = Q.get_Q(j, l);
				if (uj)
					for (k = 0; k < l; k++)
						G_bar[k] -= C_j * Q_j[k];
				else
					for (k = 0; k < l; k++)
						G_bar[k] += C_j * Q_j[k];
			}
		}
	}

	// calculate rho
	si->rho = calculate_rho();

	// calculate objective value
	{
		double v = 0;
		int i;
		for (i = 0; i < l; i++)
			v += alpha[i] * (G[i] + p[i]);

		si->obj = v / 2;
	}

	// put back the solution
	{
		for (int i = 0; i < l; i++)
			alpha_[active_set[i]] = alpha[i];
	}

	// juggle everything back
	/*{
	 for(int i=0;i<l;i++)
	 while(active_set[i] != i)
	 swap_index(i,active_set[i]);
	 // or Q.swap_index(i,active_set[i]);
	 } */

	si->upper_bound_p = Cp;
	si->upper_bound_n = Cn;

	info("\noptimization finished, #iter = %d\n", iter);

	delete[] p;
	delete[] y;
	delete[] alpha;
	delete[] alpha_status;
	delete[] active_set;
	delete[] G;
	delete[] G_bar;
}

// return 1 if already optimal, return 0 otherwise
int Solver::select_working_set(int &out_i, int &out_j) {
	// return i,j such that
	// i: maximizes -y_i * grad(f)_i, i in I_up(\alpha)
	// j: minimizes the decrease of obj value
	//    (if quadratic coefficeint <= 0, replace it with tau)
	//    -y_j*grad(f)_j < -y_i*grad(f)_i, j in I_low(\alpha)

	double Gmax = -INF;
	double Gmax2 = -INF;
	int Gmax_idx = -1;
	int Gmin_idx = -1;
	double obj_diff_min = INF;

	for (int t = 0; t < active_size; t++)
		if (y[t] == +1) {
			if (!is_upper_bound(t))
				if (-G[t] >= Gmax) {
					Gmax = -G[t];
					Gmax_idx = t;
				}
		} else {
			if (!is_lower_bound(t))
				if (G[t] >= Gmax) {
					Gmax = G[t];
					Gmax_idx = t;
				}
		}

	int i = Gmax_idx;
	const Qfloat *Q_i = NULL;
	if (i != -1) // NULL Q_i not accessed: Gmax=-INF if i=-1
		Q_i = Q->get_Q(i, active_size);

	for (int j = 0; j < active_size; j++) {
		if (y[j] == +1) {
			if (!is_lower_bound(j)) {
				double grad_diff = Gmax + G[j];
				if (G[j] >= Gmax2)
					Gmax2 = G[j];
				if (grad_diff > 0) {
					double obj_diff;
					double quad_coef = Q_i[i] + QD[j] - 2 * y[i] * Q_i[j];
					if (quad_coef > 0)
						obj_diff = -(grad_diff * grad_diff) / quad_coef;
					else
						obj_diff = -(grad_diff * grad_diff) / TAU;

					if (obj_diff <= obj_diff_min) {
						Gmin_idx = j;
						obj_diff_min = obj_diff;
					}
				}
			}
		} else {
			if (!is_upper_bound(j)) {
				double grad_diff = Gmax - G[j];
				if (-G[j] >= Gmax2)
					Gmax2 = -G[j];
				if (grad_diff > 0) {
					double obj_diff;
					double quad_coef = Q_i[i] + QD[j] + 2 * y[i] * Q_i[j];
					if (quad_coef > 0)
						obj_diff = -(grad_diff * grad_diff) / quad_coef;
					else
						obj_diff = -(grad_diff * grad_diff) / TAU;

					if (obj_diff <= obj_diff_min) {
						Gmin_idx = j;
						obj_diff_min = obj_diff;
					}
				}
			}
		}
	}

	if (Gmax + Gmax2 < eps)
		return 1;

	out_i = Gmax_idx;
	out_j = Gmin_idx;
	return 0;
}

bool Solver::be_shrunken(int i, double Gmax1, double Gmax2) {
	if (is_upper_bound(i)) {
		if (y[i] == +1)
			return (-G[i] > Gmax1);
		else
			return (-G[i] > Gmax2);
	} else if (is_lower_bound(i)) {
		if (y[i] == +1)
			return (G[i] > Gmax2);
		else
			return (G[i] > Gmax1);
	} else
		return (false);
}

void Solver::do_shrinking() {
	int i;
	double Gmax1 = -INF; // max { -y_i * grad(f)_i | i in I_up(\alpha) }
	double Gmax2 = -INF; // max { y_i * grad(f)_i | i in I_low(\alpha) }

	// find maximal violating pair first
	for (i = 0; i < active_size; i++) {
		if (y[i] == +1) {
			if (!is_upper_bound(i)) {
				if (-G[i] >= Gmax1)
					Gmax1 = -G[i];
			}
			if (!is_lower_bound(i)) {
				if (G[i] >= Gmax2)
					Gmax2 = G[i];
			}
		} else {
			if (!is_upper_bound(i)) {
				if (-G[i] >= Gmax2)
					Gmax2 = -G[i];
			}
			if (!is_lower_bound(i)) {
				if (G[i] >= Gmax1)
					Gmax1 = G[i];
			}
		}
	}

	// shrink

	for (i = 0; i < active_size; i++)
		if (be_shrunken(i, Gmax1, Gmax2)) {
			active_size--;
			while (active_size > i) {
				if (!be_shrunken(active_size, Gmax1, Gmax2)) {
					swap_index(i, active_size);
					break;
				}
				active_size--;
			}
		}
	// unshrink, check all variables again before final iterations

	if (unshrinked || Gmax1 + Gmax2 > eps * 10)
		return;

	unshrinked = true;
	reconstruct_gradient();

	for (i = l - 1; i >= active_size; i--)
		if (!be_shrunken(i, Gmax1, Gmax2)) {
			while (active_size < i) {
				if (be_shrunken(active_size, Gmax1, Gmax2)) {
					swap_index(i, active_size);
					break;
				}
				active_size++;
			}
			active_size++;
		}
}

double Solver::calculate_rho() {
	double r;
	int nr_free = 0;
	double ub = INF, lb = -INF, sum_free = 0;
	for (int i = 0; i < active_size; i++) {
		double yG = y[i] * G[i];

		if (is_upper_bound(i)) {
			if (y[i] == -1)
				ub = std::min(ub, yG);
			else
				lb = std::max(lb, yG);
		} else if (is_lower_bound(i)) {
			if (y[i] == +1)
				ub = std::min(ub, yG);
			else
				lb = std::max(lb, yG);
		} else {
			++nr_free;
			sum_free += yG;
		}
	}

	if (nr_free > 0)
		r = sum_free / nr_free;
	else
		r = (ub + lb) / 2;

	return r;
}

class ONE_CLASS_Q: public Kernel {
public:
	ONE_CLASS_Q(const svm_problem & prob, const svm_parameter & param) :
		Kernel(prob.l, prob.x, param) {
		//printf("ocq1\n");
		cache = new Cache(prob.l, (long int) (param.cache_size * (1 << 20)));
		//printf("ocq2\n");
		QD = new Qfloat[prob.l];
		//printf("ocq3\n");
		//printf("prob.l = %d\n", prob.l);
		for (int i = 0; i < prob.l; i++) {
//			cout << this->
//			printf("kf(%d, %d) = %g\n", i, i, (Qfloat) (this->*kernel_function) (i, i));
			QD[i] = (Qfloat) (this->*kernel_function)(i, i);
		}
		//printf("ocq4\n");
	}
	Qfloat *get_Q(int i, int len) const {
		Qfloat *data;
		int start;
		if ((start = cache->get_data(i, &data, len)) < len) {
			for (int j = start; j < len; j++)
				data[j] = (Qfloat) (this->*kernel_function)(i, j);
		}
		return data;
	}

	Qfloat *get_QD() const {
		return QD;
	}
	void swap_index(int i, int j) const {
		cache->swap_index(i, j);
		Kernel::swap_index(i, j);
		std::swap(QD[i], QD[j]);
	}
	~ONE_CLASS_Q() {
		delete cache;
		delete[] QD;
	}
private:
	Cache * cache;
	Qfloat *QD;
};

static void solve_svdd(const svm_problem * prob, const svm_parameter * param,
		double *alpha, Solver::SolutionInfo* si) {
	//printf("1.1.1.1\n");
	int l = prob->l;
	double *linear_term = new double[l];
	schar *ones = new schar[l];
	int i;

	//printf("1.1.1.2\n");
	//int n = (int) (param->nu * l);
	//double ub = 1 / (double) n;
	double ub = 1 / (param->nu * l);
	int n = (int) 1 / ub;
	for (i = 0; i < n; i++)
		alpha[i] = ub;
	if (n < l)
		alpha[n] = 1 - n * ub;
	for (i = n + 1; i < l; i++)
		alpha[i] = 0;

	//printf("1.1.1.3\n");
	ONE_CLASS_Q Q = ONE_CLASS_Q(*prob, *param); //re-use one-class Q
	//printf("1.1.1.3.1\n");
	for (i = 0; i < l; i++) {
		linear_term[i] = -0.5 * Q.get_QD()[i];
		ones[i] = 1;
	}

	//printf("1.1.1.4\n");
	//	//printf("eps = %g\n", param->eps);
	Solver s;
	s.Solve(l, Q, linear_term, ones, alpha, ub, ub, param->eps, si,
			param->shrinking);

	//printf("1.1.1.5\n");
	delete[] linear_term;
	delete[] ones;
	//printf("1.1.1.6\n");
}

//
// decision_function
//
struct decision_function {
	double *alpha;
	double rho;
	double obj;
};

decision_function svm_train_one(const svm_problem * prob,
		const svm_parameter * param) {
	//printf("1.1.1\n");
	double *alpha = new double[prob->l];
	Solver::SolutionInfo si;
	solve_svdd(prob, param, alpha, &si);

	//printf("1.1.2\n");
	info("obj = %f, rho = %f\n", si.obj, si.rho);
	if (false) {
		info("alphas: ");
		for (int i = 0; i < prob->l; i++)
			info("%f ", alpha[i]);

		info("\nSVs: ");
	}

	//printf("1.1.3\n");
	// output SVs
	int nSV = 0;
	int nBSV = 0;
	for (int i = 0; i < prob->l; i++) {
		if (fabs(alpha[i]) > 0) {
			++nSV;
			if (false)
				info("%d[%f] ", i, alpha[i]);
			if (fabs(alpha[i]) >= si.upper_bound_p)
				++nBSV;
		}
	}

	//printf("1.1.4\n");
	info("\nnSV = %d, nBSV = %d\n", nSV, nBSV);
	decision_function f;
	f.alpha = alpha;
	f.rho = si.rho;
	f.obj = si.obj;
	//printf("1.1.5\n");
	return f;
}

//
// Interface functions
//
svm_model *svm_train(const svm_problem * prob, const svm_parameter * param) {
	//printf("1\n");
	svm_model *model = new svm_model;
	model->param = *param;

	//printf("1.1\n");
	decision_function f = svm_train_one(prob, param);
	model->rho = f.rho;
	model->obj = f.obj;

	//printf("2\n");
	/* Fill in the basic components of the model: the list of SVs, their coefficients etc. */
	int nSV = 0;
	int i;
	for (i = 0; i < prob->l; i++)
		if (fabs(f.alpha[i]) > 0)
			++nSV;
	model->l = nSV;
	model->SV = new stem::NGram *[nSV];
	model->sv_coef = new double[nSV];
	int j = 0;
	for (i = 0; i < prob->l; i++)
		if (fabs(f.alpha[i]) > 0) {
			model->SV[j] = prob->x[i];
			model->sv_coef[j] = f.alpha[i];
			++j;
		}

	// Compute norm of SVs and find one bounded SV
	// This buggy code is removed
	/*
	 j = 0;
	 double min_coef = DBL_MAX;
	 double *sv_coef = model->sv_coef;
	 model->SVsq = Malloc(double, nSV);
	 for (i = 0; i < model->l; i++) {
	 model->SVsq[i] =
	 Kernel::k_function(model->SV[i], model->SV[i], model->param);
	 if (model->sv_coef[i] < min_coef) {
	 min_coef = sv_coef[j];
	 j = i;
	 }
	 }
	 */

	//printf("3\n");
	int min_i = 0;
	double min_coef = DBL_MAX;
	double ub = 1 / (param->nu * prob->l);

	/* Find the support vector with the smallest alpha */
	for (int i = 0; i < nSV; i++) {
		if (model->sv_coef[i] > 0 && model->sv_coef[i] < ub
				&& model->sv_coef[i] < min_coef) {
			min_coef = model->sv_coef[i];
			min_i = i;
		}
	}

	double r;
	//printf("4\n");
	/* If unbounded support vectors exist, use the classification of the found
	 point as r */
	if (min_coef < ub)
		svm_predict_values(model, model->SV[min_i], &r);

	/* If not, classify all (bounded) support vectors and take the smallest
	 prediction value as r */
	else {
		double min_z = DBL_MAX;
		for (int k = 0; k < nSV; k++) {
			double tmp;
			svm_predict_values(model, model->SV[k], &tmp);
			if (tmp < min_z)
				min_z = tmp;
		}
		r = min_z;
	}
	//printf("5\n");
	model->radius = r;

	// Determine radius using bounded SV[j]

	/*double r = model->SVsq[j];
	 for (int i = 0; i < model->l; i++) {
	 r -= 2 * sv_coef[i] * Kernel::k_function(model->SV[j], model->SV[i],
	 model->param);
	 r += sv_coef[i] * model->SVsq[i];
	 }
	 model->radius = sqrt(r);*/

	delete[] f.alpha;
	//printf("6\n");
	return model;
}

void svm_predict_values(const struct svm_model *model, const stem::NGram *x,
		double* dec_values) {
	// Compute distance from center of hypersphere              
	double *sv_coef = model->sv_coef;
	double d = Kernel::k_function(x, x, model->param), l = 0;
	for (int i = 0; i < model->l; i++) {
		d -= 2 * sv_coef[i] * Kernel::k_function(x, model->SV[i], model->param);
		l += sv_coef[i] * Kernel::k_function(model->SV[i], model->SV[i],
				model->param);

	}
	// Nice Hack: Get quadratic term from objective of QP            
	//       First remove scaling and then linear term          
	double q = 2 * model->obj + l;
	*dec_values = sqrt(d + q);
}

void svm_destroy_model(svm_model * model) {
	delete[] model->SV;
	delete[] model->sv_coef;
	delete model;
}

char *svm_check_parameter(const svm_parameter * param) {
	// cache_size,eps,C,nu,p,shrinking
	if (param->cache_size <= 0)
		return (char *) "cache_size <= 0";

	if (param->eps <= 0)
		return (char *) "eps <= 0";

	if (param->nu <= 0 || param->nu > 1)
		return (char *) "nu <= 0 or nu > 1";

	if (param->shrinking != 0 && param->shrinking != 1)
		return (char *) "shrinking != 0 and shrinking != 1";

	return NULL;
}
