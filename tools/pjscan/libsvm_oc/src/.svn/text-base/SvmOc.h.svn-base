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
 * SvmOc.h
 *  Created on: Apr 27, 2011
 */

#ifndef SVMOC_H_
#define SVMOC_H_

#include <stem/NGram.h>
#include <stem/Kernel.h>
#include "libsvm.h"

namespace svm_oc {

class SvmOc {
private:
	// Kernel
	stem::Kernel *kernel;
	// Center of the hypersphere
	stem::NGram center;
	// libsvm model information
	struct svm_model *model;
	// libsvm parameter information
	struct svm_parameter param;
	// Norm of the center
	double cnorm;

	// Verbosity level
	static unsigned int verbose;
	// Forbid assignment
	SvmOc &operator =(const SvmOc &other);
public:
	// Constructor
	SvmOc(double nu, stem::Kernel *kernel);
	// Copy constructor
	SvmOc(const SvmOc &other);
	// Destructor
	~SvmOc();

	// Learns from data
	void learn(stem::NGram *ngram_array, unsigned int size);
	// Classifies a data point and returns it's distance from the center
	double getDistance(const stem::NGram &ngram) const;
	// Sets the verbosity level
	void setVerbosity(unsigned int verbosity) {
		verbose = verbosity;
	}
	// Returns the radius
	double getRadius() const;
	// Returns the center
	stem::NGram getCenter() const {
		return center;
	}
	// Saves the SVM model to a file
	bool serialize(const char *file_name) const;
	// Loads the SVM model from a file
	static void deserialize(const char *file_name, SvmOc &svm);
};

}
#endif /* SVMOC_H_ */
