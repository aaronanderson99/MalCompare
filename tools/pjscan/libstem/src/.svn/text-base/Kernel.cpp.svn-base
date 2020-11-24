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
 * Kernel.cpp
 *  Created on: Apr 26, 2011
 */

#include "Kernel.h"

namespace stem {

Kernel *Kernel::deserialize(std::istream &in) {
	int type;
	in >> type;
	if (!in) {
		in.clear();
		in.ignore();
		throw "Kernel: Invalid input";
	}
	switch (KernelType(type)) {
	case KERNEL_LINEAR:
		return new LinearKernel(in);
		break;
	case KERNEL_RBF:
		return new RBFKernel(in);
		break;
	default:
		throw "Kernel: Unknown kernel type";
	}
}

RBFKernel::RBFKernel(std::istream &in) {
	in >> gamma;
	if (!in) {
		in.clear();
		in.ignore();
		throw "RBFKernel: Invalid input";
	}
}

double RBFKernel::compute(const NGram &a, const NGram &b) const {
	return std::exp((a * a - a * b * 2 + b * b) / -gamma);
}

}
