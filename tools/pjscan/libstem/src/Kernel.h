/*
 * Copyright 2012 Nedim Srndic, Pavel Laskov, University of Tuebingen
 *
 * This file is part of libstem.

 * libstem is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libstem is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libstem.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Kernel.h
 *  Created on: Apr 26, 2011
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "NGram.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

namespace stem {

enum KernelType {
	KERNEL_LINEAR, KERNEL_RBF
};

/*
 * A class representing a kernel.
 */
class Kernel {
protected:
	// Prints out kernel information
	virtual std::ostream &serialize(std::ostream &out) const = 0;
public:
	// Virtual destructor
	virtual ~Kernel() {
	}
	// Virtual constructor idiom
	virtual Kernel *clone() const = 0;

	// Virtual method, returns a constant describing the type of this kernel
	virtual KernelType getType() const = 0;
	// Virtual method, computes the distance between two vectors
	virtual double compute(const NGram &a, const NGram &b) const = 0;
	// Prints out kernel information to the output stream
	friend std::ostream &operator <<(std::ostream &out, const Kernel &kernel) {
		return kernel.serialize(out);
	}
	// Loads a new kernel from the input stream
	static Kernel *deserialize(std::istream &in);
};

/*
 * A class representing a linear kernel.
 */
class LinearKernel: public Kernel {
protected:
	// Prints out kernel information
	virtual std::ostream &serialize(std::ostream &out) const {
		return out << this->getType();
	}
public:
	// Default constructor
	LinearKernel() {
	}
	// Constructor for deserialization
	explicit LinearKernel(std::istream &in) {
		// do nothing
	}
	// Virtual destructor
	virtual ~LinearKernel() {
	}
	// Virtual constructor idiom
	static LinearKernel *create() {
		return new LinearKernel();
	}
	virtual LinearKernel *clone() const {
		return new LinearKernel(*this);
	}

	// Virtual method, returns a constant describing the type of this kernel
	virtual KernelType getType() const {
		return KERNEL_LINEAR;
	}
	// Computes the dot product between two vectors
	double compute(const NGram &a, const NGram &b) const {
		return a * b;
	}
};

/*
 * A class representing a kernel based on the radial basis function.
 */
class RBFKernel: public Kernel {
protected:
	// Prints out kernel information
	virtual std::ostream &serialize(std::ostream &out) const {
		return out << this->getType() << ' ' << this->gamma;
	}
private:
	// The gamma parameter
	double gamma;
public:
	// Constructor for deserialization
	explicit RBFKernel(std::istream &in);
	// Constructor
	explicit RBFKernel(double gamma) :
		gamma(gamma) {

	}
	// Virtual constructor idiom
	static RBFKernel *create() {
		return new RBFKernel(10.0);
	}
	virtual RBFKernel *clone() const {
		return new RBFKernel(*this);
	}

	// Virtual method, returns a constant describing the type of this kernel
	virtual KernelType getType() const {
		return KERNEL_RBF;
	}
	// Virtual destructor
	virtual ~RBFKernel() {
	}
	// Computes the RBF distance between two vectors
	double compute(const NGram &a, const NGram &b) const;
};

}

#endif /* KERNEL_H_ */
