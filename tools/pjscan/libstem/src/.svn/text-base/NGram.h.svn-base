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
 * NGram.h
 *  Created on: Apr 13, 2011
 */

#include "NGramConf.h"
#include "NGramItem.h"
#include "util.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <iostream>
#include <fstream>
#include <vector>

#ifndef NGRAM_H_
#define NGRAM_H_

namespace stem {

/*
 * A class representing information about a collection of n-grams extracted from a sequence.
 */
class NGram {
private:
	// Class version
	static const unsigned int VERSION = 1;
	// Count of features and values
	unsigned int count;
	// Configuration object
	boost::shared_ptr<NGramConf> conf;
	// Array of features
	boost::shared_array<NGramItem> features;

	// Extracts ngrams from data
	void extract(const char *s, unsigned long l);
	// Deletes entries with zero values
	void sparsify();
	// Returns true if the n-gram is initialized
	bool initialized() const;

	// Embedding functions
	static void do_count(NGram &ngram);
	static void do_binary(NGram &ngram);
	static void do_frequency(NGram &ngram);
	static void do_normalized(NGram &ngram);
	static void do_binary_normalized(NGram &ngram);
	// Embedding function table
	static void (*embedding_fns[])(NGram &);
	// Returns the embedding function for the embedding used
	void (*get_embedding_fn())(NGram &) const {
		return NGram::embedding_fns[conf->getEmbeddingType()];
	}
public:
	// Constructors, destructor
	NGram();
	NGram(const NGram &src);
	NGram(const NGramConf &conf);
	NGram(const NGramConf &conf, const char *s, unsigned long l);

	// Deep copy
	NGram clone() const;

	// Overloaded operators
	NGram &operator =(const NGram &src);
	NGram &operator +=(const NGram &arg);
	NGram &operator -=(const NGram &arg);
	NGram &operator *=(double arg);
	double operator *(const NGram &arg) const;
	bool operator ==(const NGram &arg) const;

	// Norms
	double L1norm() const;
	double L2norm() const;
	void reembed(EmbeddingType e) {
		NGram::embedding_fns[e](*this);
	}

	// Bound the values to min <= value <= max
	void trim(double min, double max);
	// Returns the configuration object
	NGramConf getConfiguration() const {
		return *conf;
	}
	// Returns the number of features
	unsigned int getCount() const {
		return count;
	}
	// Returns top N features, sorted by value, descending
	std::vector<NGramItem> getTopN(unsigned int n) const;

	// Input/Output
	friend std::ostream &operator <<(std::ostream &out, const NGram &ngram);
	friend std::ofstream &operator <<(std::ofstream &fout, const NGram &ngram);
	friend std::ifstream &operator >>(std::ifstream &fin, NGram &ngram);
private:
	// File input for version 1
	friend std::ifstream &fin_v1(std::ifstream &fin, NGram &ngram);
};

}

#endif /* NGRAM_H_ */
