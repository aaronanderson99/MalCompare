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
 * NGramTest.cpp
 *  Created on: Apr 20, 2011
 */

#define BOOST_TEST_MODULE KernelTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../src/NGram.h"
#include "../src/Kernel.h"
#include "../src/util.h"

using namespace stem;

BOOST_AUTO_TEST_CASE(linear)
{
	LinearKernel linear_kernel;
	NGramConf conf2(COUNT, 2);
	NGram a(conf2, "ababcbc", 7);
	NGram b(conf2, "bacbaba", 7);
	BOOST_CHECK_EQUAL(double_cmp(linear_kernel.compute(a, b), 6.0), true);

	NGramConf conf1(COUNT, 1);
	NGram c(conf1, "abaacc", 6);
	NGram d(conf1, "dddaabbb", 8);
	BOOST_CHECK_EQUAL(double_cmp(linear_kernel.compute(c, d), 9.0), true);
}

BOOST_AUTO_TEST_CASE(rbf)
{
	RBFKernel rbf_kernel(10.0);
	NGramConf conf2(COUNT, 2);
	NGram a(conf2, "ababcbc", 7);
	NGram b(conf2, "bacbaba", 7);
	BOOST_CHECK_EQUAL(double_cmp(rbf_kernel.compute(a, b), 0.36787944119999), true);

	NGramConf conf1(COUNT, 1);
	NGram c(conf1, "abaacc", 6);
	NGram d(conf1, "dddaabbb", 8);
	BOOST_CHECK_EQUAL(double_cmp(rbf_kernel.compute(c, d), 0.16529888819999), true);
}

