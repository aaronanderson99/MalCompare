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
 * NGramItem.h
 *  Created on: Apr 21, 2011
 */

#ifndef NGRAMITEM_H_
#define NGRAMITEM_H_

#include "util.h"
#include <iostream>
#include <fstream>

namespace stem {

/*
 * Encapsulated information about an n-gram.
 */
class NGramItem {
private:
	unsigned long feature;
	double value;
public:
	NGramItem() {
	}
	NGramItem(unsigned long feature, double value) :
		feature(feature), value(value) {
	}

	// Getters
	unsigned long getFeature() const {
		return feature;
	}
	double getValue() const {
		return value;
	}
	// Setters
	void setFeature(unsigned long newFeature) {
		feature = newFeature;
	}
	void setValue(double newValue) {
		value = newValue;
	}

	// Comparison operators
	bool operator <(const NGramItem &other) const {
		return feature < other.feature;
	}
	bool operator ==(const NGramItem &other) const {
		return feature == other.feature;
	}
	// Returns true if the value of the first item is smaller than the value of
	// the second item
	static bool compare_values(const NGramItem &a, const NGramItem &b) {
		return a.value < b.value;
	}
	// Returns true if the value of the first item is larger than the value of
	// the second item
	static bool compare_values_descending(const NGramItem &a,
			const NGramItem &b) {
		return a.value > b.value;
	}

	// Output
	void print(std::ostream &cout, unsigned int length) const;
	void print(std::ofstream &fout) const;

	// Numeric
	static bool close_to_0(NGramItem item);
};

}

#endif /* NGRAMITEM_H_ */
