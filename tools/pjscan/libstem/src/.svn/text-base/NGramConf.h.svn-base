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
 * NGramConf.h
 *  Created on: Apr 13, 2011
 */

#ifndef NGRAMCONF_H_
#define NGRAMCONF_H_

#include <fstream>
#include <iostream>

namespace stem {

// Supported embedding types
enum EmbeddingType {
	COUNT,
	BINARY,
	FREQUENCY,
	NORMALIZED,
	BINARY_NORMALIZED
};
// Number of supported embedding types
static const unsigned int NUM_EMBEDDINGS = 5;

/*
 * A class representing configuration settings for n-gram extraction.
 */
class NGramConf {
private:
	// Class version
	static const unsigned int VERSION = 1;
	// Embedding type
	EmbeddingType type;
	// Embedding length
	unsigned int length;
	// A list of names of embedding types
	static const char *EMBEDDING_NAMES[];

	// File input for version 1
	friend std::ifstream &fin_v1(std::ifstream &fin, NGramConf &conf);
public:
	NGramConf(EmbeddingType type, unsigned int length);

	// Getters
	EmbeddingType getEmbeddingType() const {
		return type;
	}
	unsigned int getLength() const {
		return length;
	}

	// Input / Output
	friend std::ifstream &operator >>(std::ifstream &fin, NGramConf &conf);
	friend std::ofstream &operator <<(std::ofstream &fout, const NGramConf &conf);
	friend std::ostream &operator <<(std::ostream &out, const NGramConf &conf);

	// Comparison
	bool operator ==(const NGramConf &other) const {
		return type == other.type && length == other.length;
	}
};

}

#endif /* NGRAMCONF_H_ */
