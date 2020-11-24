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
 * NGramItem.cpp
 *  Created on: Apr 21, 2011
 */

#include "NGramItem.h"

namespace stem {

void NGramItem::print(std::ostream &cout, unsigned int length) const {
	const char *char_feature = (const char *) &feature;
	for (unsigned int i = length; i >= 1; i--) {
		if (isprint(char_feature[i - 1]))
			cout << char_feature[i - 1];
		else
			cout << '.';
	}
	cout << '\t' << value;
}
void NGramItem::print(std::ofstream &fout) const {
	fout << feature << '\t' << value;
}

bool NGramItem::close_to_0(NGramItem value) {
	return double_close_to_0(value.getValue());
}

}
