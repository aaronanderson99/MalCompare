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
 * util.cpp
 *  Created on: Apr 21, 2011
 */

#include "util.h"

namespace stem {

static const double epsilon = 1e-06;

bool double_close_to_0(double value) {
	return -epsilon <= value && epsilon >= value;
}

bool double_cmp(double a, double b) {
	return a - epsilon <= b && a + epsilon >= b;
}

}
