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

#ifndef _LIBSVM_H
#define _LIBSVM_H

#include <stem/NGram.h>
#include <stem/Kernel.h>

#define LIBSVM_VERSION 286

struct svm_problem {
	int l;
	stem::NGram **x;
};

enum {
	SVDD
};

struct svm_parameter {
	int svm_type;
	stem::Kernel *kernel;

	/* these are for training only */
	double cache_size; /* Cache in MB */
	double eps; /* Stopping criteria */
	double nu; /* For SVDD */
	int shrinking; /* Use the shrinking heuristics */
	int probability; /* Do probability estimates */
};

struct svm_model {
	struct svm_parameter param; // parameter
	int l; // total #SV
	stem::NGram **SV; // SVs (SV[l])
	double *sv_coef; // coefficients for SVs in decision functions (sv_coef[k-1][l])
	double rho; // constants in decision functions (rho[k*(k-1)/2])
	double obj; // objective function

	// for SVDD
	double radius; // radius of hypersphere
	double rbf_const; // constant factor in the objective for RBF kernel
};

char *svm_check_parameter(const struct svm_parameter *param);
struct svm_model *svm_train(const struct svm_problem *prob,
		const struct svm_parameter *param);
void svm_predict_values(const struct svm_model *model, const stem::NGram *x,
		double* dec_values);
void svm_destroy_model(struct svm_model *model);

#endif /* _LIBSVM_H */
