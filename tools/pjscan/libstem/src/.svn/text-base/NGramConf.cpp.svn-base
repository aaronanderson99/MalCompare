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
 * NGramConf.cpp
 *  Created on: Apr 13, 2011
 */

#include "NGramConf.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

namespace stem {
const char *NGramConf::EMBEDDING_NAMES[NUM_EMBEDDINGS] = { "COUNT", "BINARY",
		"FREQUENCY", "NORMALIZED", "BINARY_NORMALIZED" };

NGramConf::NGramConf(EmbeddingType type, unsigned int length) :
	type(type), length(length) {
	if (length > sizeof(unsigned long))
		throw "Embeddings greater than the size of unsigned long are not supported";
}

std::ifstream &fin_v1(std::ifstream &fin, NGramConf &conf) {
	static char embedding_type[20];
	unsigned int length = 0;
	fin >> std::setw(20) >> embedding_type >> std::ws >> length >> std::ws;
	if (!fin)
		throw "Malformed embedding information";

	bool type_ok = false;
	for (unsigned int i = 0; i < NUM_EMBEDDINGS; i++)
		if (std::strcmp(embedding_type, NGramConf::EMBEDDING_NAMES[i]) == 0) {
			conf.type = (EmbeddingType) i;
			type_ok = true;
		}

	if (!type_ok)
		throw "Embedding type not recognized";

	conf.length = length;
	return fin;
}

std::ifstream &operator >>(std::ifstream &fin, NGramConf &conf) {
	unsigned int version = 0;
	fin >> std::ws >> version;
	if (!fin)
		throw "Embedding file version not recognized";
	if (version > NGramConf::VERSION)
		throw "Unsupported embedding file version";
	switch (version) {
	case 1:
		return fin_v1(fin, conf);
	default:
		throw "Unsupported embedding file version";
	}
}

std::ofstream &operator <<(std::ofstream &fout, const NGramConf &conf) {
//	std::cout << "@NGramConf::fout" << std::endl;
	fout << NGramConf::VERSION << ' ' << NGramConf::EMBEDDING_NAMES[conf.type]
			<< ' ' << conf.length;
	return fout;
}

std::ostream &operator <<(std::ostream &out, const NGramConf &conf) {
//	std::cout << "@NGramConf::cout" << std::endl;
	return out << "Embedding type: " << NGramConf::EMBEDDING_NAMES[conf.type]
			<< std::endl << "Embedding length: " << conf.length;
}
}
