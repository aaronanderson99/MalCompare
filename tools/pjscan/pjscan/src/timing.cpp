/*
 * Copyright 2012 Nedim Srndic, University of Tuebingen
 *
 * This file is part of pjscan.

 * pjscan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pjscan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pjscan.  If not, see <http://www.gnu.org/licenses/>.
 *
 * timing.cpp
 *  Created on: Jun 2, 2011
 */

#include "timing.h"

double time_diff(const struct timeval &begin, const struct timeval &end) {
	struct timeval res;
	timersub(&end, &begin, &res);
	return res.tv_sec + double(res.tv_usec) / 1000000;
}
