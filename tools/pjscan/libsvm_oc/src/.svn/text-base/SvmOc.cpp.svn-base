/*
 * Copyright 2012 Nedim Srndic, Pavel Laskov, University of Tuebingen
 *
 * This file is part of libsvm_oc.

 * libsvm_oc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libsvm_oc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libsvm_oc.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SvmOc.cpp
 *  Created on: Apr 27, 2011
 */

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "SvmOc.h"

using std::cout;
using std::endl;

namespace svm_oc {

unsigned int SvmOc::verbose = 0;

SvmOc::SvmOc(double nu, stem::Kernel *kernel) :
	kernel(kernel->clone()), center(), model(0), param(), cnorm(0.0) {
	param.svm_type = SVDD;
	param.nu = nu;
	param.kernel = kernel;
	param.cache_size = 256;
	param.eps = 1e-6;
}

SvmOc::SvmOc(const SvmOc &other) :
	kernel(other.kernel->clone()), center(other.center), model(other.model),
			param(other.param), cnorm(other.cnorm) {
	// Deep copying of pointers in structs
	param.kernel = kernel;
	if (other.model == 0)
		return;
	model->SV = new stem::NGram *[model->l];
	std::copy(other.model->SV, other.model->SV + model->l, model->SV);
	model->sv_coef = new double[model->l];
	std::copy(other.model->sv_coef, other.model->sv_coef + model->l,
			model->sv_coef);
}

SvmOc::~SvmOc() {
	delete kernel;

	if (model != 0)
		svm_destroy_model(model);
}

void SvmOc::learn(stem::NGram *ngram_array, unsigned int size) {
	if (size == 0)
		throw "Empty array";

	struct svm_problem prob;
	prob.l = size;
	prob.x = new stem::NGram*[size];
	for (unsigned int i = 0; i < size; i++)
		prob.x[i] = &(ngram_array[i]);

	char *parms = svm_check_parameter(&param);
	if (parms)
		cout << "libsvm error: " << parms << endl;

	model = svm_train(&prob, &param);
	// Zero out the center and norm
	cnorm = 0.0;
	center = stem::NGram();

	for (int i = 0; i < model->l; i++) {
		stem::NGram temp(model->SV[i]->clone());
		temp *= model->sv_coef[i];
		center += temp;
	}

	cnorm = kernel->compute(center, center);

	if (kernel->getType() == stem::KERNEL_RBF) {
		model->rbf_const = 0;

		for (int i = 0; i < model->l; i++)
			for (int j = 0; j < model->l; j++) {
				model->rbf_const += model->sv_coef[i] * model->sv_coef[j]
						* kernel->compute(*model->SV[i], *model->SV[j]);
			}
	}

	delete[] prob.x;
}

double SvmOc::getDistance(const stem::NGram &ngram) const {
	if (model == 0)
		throw "Unable to classify. Need to train first";

	if (kernel->getType() == stem::KERNEL_RBF) {
		double result = model->rbf_const + 1;
		for (int i = 0; i < model->l; i++)
			result -= 2 * kernel->compute(*model->SV[i], ngram)
					* model->sv_coef[i];

		return std::sqrt(result);
	}

	return std::sqrt(
			kernel->compute(ngram, ngram) - 2 * kernel->compute(ngram, center)
					+ cnorm);
}

double SvmOc::getRadius() const {
	if (model == 0)
		throw "Radius not calculated. Need to train first";

	return model->radius;
}

bool SvmOc::serialize(const char *file_name) const {
	if (model == 0)
		throw "Cannot serialize, SVM not initialized";

	std::ofstream out(file_name, std::ios::trunc);
	out << cnorm << endl;
	out << *kernel << endl;
	out << center << endl;

	// svm_parameter
	out << param.svm_type << ' ' << param.cache_size << ' ' << param.eps << ' '
			<< param.nu << ' ' << param.shrinking << ' ' << param.probability
			<< endl;
	// svm_model
	out << model->l << ' ' << model->rho << ' ' << model->obj << ' '
			<< model->radius << ' ' << model->rbf_const << endl;
	for (int i = 0; i < model->l; i++)
		out << *model->SV[i] << endl << model->sv_coef[i] << endl;
	return true;
}

void SvmOc::deserialize(const char *file_name, SvmOc &svm) {
	std::ifstream in(file_name);
	in >> svm.cnorm;
	delete svm.kernel;
	svm.kernel = stem::Kernel::deserialize(in);
	svm.param.kernel = svm.kernel;
	in >> svm.center;

	// svm_parameter
	in >> svm.param.svm_type >> svm.param.cache_size >> svm.param.eps
			>> svm.param.nu >> svm.param.shrinking >> svm.param.probability;

	// svm_model
	svm.model = new ::svm_model;
	svm.model->param = svm.param;
	in >> svm.model->l >> svm.model->rho >> svm.model->obj >> svm.model->radius
			>> svm.model->rbf_const;
	svm.model->SV = new stem::NGram *[svm.model->l];
	svm.model->sv_coef = new double[svm.model->l];
	for (int i = 0; i < svm.model->l; i++) {
		svm.model->SV[i] = new stem::NGram();
		in >> *svm.model->SV[i] >> svm.model->sv_coef[i];
	}
}

}
