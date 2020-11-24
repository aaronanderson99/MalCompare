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
 * NGram.cpp
 *  Created on: Apr 13, 2011
 */

#include "NGram.h"
#include <algorithm>
#include <cstring>
#include <cassert>
#include <cctype> // isprint()
#include <cmath> // sqrt()
namespace stem {

void NGram::do_count(NGram &ngram) {
	// do nothing
}

void NGram::do_binary(NGram &ngram) {
	for (unsigned int i = 0; i < ngram.getCount(); i++)
		ngram.features[i].setValue(1.0);
}

void NGram::do_frequency(NGram &ngram) {
	double sum = ngram.L1norm();
	for (unsigned int i = 0; i < ngram.getCount(); i++)
		ngram.features[i].setValue(ngram.features[i].getValue() / sum);
}

void NGram::do_normalized(NGram &ngram) {
	double sum_of_squares = ngram.L2norm();
	for (unsigned int i = 0; i < ngram.getCount(); i++)
		ngram.features[i].setValue(
				ngram.features[i].getValue() / sum_of_squares);
}

void NGram::do_binary_normalized(NGram &ngram) {
	double new_value = 1.0 / std::sqrt((double) ngram.getCount());
	for (unsigned int i = 0; i < ngram.getCount(); i++) {
		ngram.features[i].setValue(new_value);
	}
}

void
		(*( NGram::embedding_fns[]))(NGram &) = {NGram::do_count, NGram::do_binary, NGram::do_frequency, NGram::do_normalized, NGram::do_binary_normalized};

void NGram::extract(const char *s, unsigned long l) {
	if (this->initialized())
		throw "N-gram already initialized, extraction not possible";

	if (l < conf->getLength())
		throw "Cannot extract n-grams: input too short";

	// Allocate the maximum amount of space neccessary
	const unsigned long length = l - conf->getLength() + 1;
	features.reset(new NGramItem[length]);
	count = 0; // count will be incremented on insertion

	// Extract features
	// This byte manipulation works only for little-endian architectures
	for (unsigned long i = 0; i < length; i++) {
		// Construct an unsigned long feature from the substring
		unsigned long feature = 0x0UL;
		// Use a char pointer to manipulate the unsigned long variable
		char *feature_string = (char *) &feature;
		// Feature bytes are 'right aligned'
		for (unsigned int j = 0, k = conf->getLength() - 1; j
				< conf->getLength(); j++, k--) {
			feature_string[j] = s[i + k];
		}
		// Both unique and duplicate features are added at this time
		features[getCount()] = NGramItem(feature, 1.0);
		count++;
	}

	assert(getCount() == length);

	// Sort features and remove duplicates
	std::sort(&(features[0]), &(features[getCount()]));
	unsigned int unique_count = 0;
	for (unsigned int i = 0; i < getCount();) {
		unsigned int duplicate_count = 1;
		for (unsigned int j = i + 1; j < getCount() && features[i]
				== features[j]; j++)
			duplicate_count++;

		features[unique_count] = NGramItem(features[i].getFeature(),
				double(duplicate_count));
		unique_count++;
		i = i + duplicate_count;
	}

	// Reallocate the features to get rid of duplicates
	NGramItem *new_features = new NGramItem[unique_count];
	std::copy(&(features[0]), &(features[unique_count]), new_features);
	features.reset(new_features);
	count = unique_count;

	// Calculate values
	(*this->get_embedding_fn())(*this);
}

void NGram::sparsify() {
	unsigned int nonzero_count = count - std::count_if(&(features[0]),
			&(features[count]), NGramItem::close_to_0);
	if (nonzero_count == count)
		return;
	NGramItem *new_features = new NGramItem[nonzero_count];
	std::remove_copy_if(&(features[0]), &(features[count]), new_features,
			NGramItem::close_to_0);
	features.reset(new_features);
	count = nonzero_count;
}

bool NGram::initialized() const {
	return features.get() != 0;
}

NGram::NGram() :
	count(0), conf(new NGramConf(COUNT, 1)), features() {
}

NGram::NGram(const NGram &src) :
	count(src.getCount()), conf(src.conf), features(src.features) {
}

NGram::NGram(const NGramConf &conf) :
	count(0), conf(boost::shared_ptr<NGramConf>(new NGramConf(conf))),
			features() {
}

NGram::NGram(const NGramConf &conf, const char *s, unsigned long l) :
	count(0), conf(boost::shared_ptr<NGramConf>(new NGramConf(conf))),
			features() {
	this->extract(s, l);
}

NGram NGram::clone() const {
	if (!this->initialized())
		return NGram(*this->conf.get());

	NGram clone;
	clone.count = getCount();
	clone.conf.reset(new NGramConf(conf->getEmbeddingType(), conf->getLength()));
	clone.features.reset(new NGramItem[clone.getCount()]);
	std::copy(features.get(), features.get() + getCount(), clone.features.get());

	return clone;
}

NGram &NGram::operator =(const NGram &src) {
	if (this == &src)
		return *this;

	count = src.getCount();
	conf = src.conf;
	features = src.features;
	return *this;
}

NGram &NGram::operator +=(const NGram &other) {
	// Be on the safe side, first declare an array that can hold all features
	NGramItem *new_features = new NGramItem[getCount() + other.getCount()];
	unsigned int insert_index = 0;
	unsigned int i = 0, j = 0;
	while (i < getCount() && j < other.getCount()) {
		NGramItem &mine(features[i]);
		NGramItem &theirs(other.features[j]);
		if (mine < theirs) {
			i++;
			new_features[insert_index++] = mine;
		} else if (theirs < mine) {
			j++;
			new_features[insert_index++] = theirs;
		} else {
			i++;
			j++;
			double new_value = mine.getValue() + theirs.getValue();
			// Remove zero values as we go
			if (!double_close_to_0(new_value)) {
				new_features[insert_index].setFeature(mine.getFeature());
				new_features[insert_index++].setValue(new_value);
			}
		}
	}

	while (i < getCount()) {
		new_features[insert_index++] = features[i++];
	}

	while (j < other.getCount()) {
		new_features[insert_index++] = other.features[j++];
	}

	features.reset(new NGramItem[insert_index]);
	count = insert_index;
	std::copy(new_features, new_features + insert_index, features.get());
	delete[] new_features;
	return *this;
}

NGram &NGram::operator -=(const NGram &other) {
	// Be on the safe side, first declare an array that can hold all features
	NGramItem *new_features = new NGramItem[getCount() + other.getCount()];
	unsigned int insert_index = 0;
	unsigned int i = 0, j = 0;
	while (i < getCount() && j < other.getCount()) {
		NGramItem &mine(features[i]);
		NGramItem &theirs(other.features[j]);
		if (mine < theirs) {
			i++;
			new_features[insert_index++] = mine;
		} else if (theirs < mine) {
			j++;
			new_features[insert_index].setFeature(theirs.getFeature());
			new_features[insert_index++].setValue(0.0 - theirs.getValue());
		} else {
			i++;
			j++;
			double new_value = mine.getValue() - theirs.getValue();
			// Remove zero values as we go
			if (!double_close_to_0(new_value)) {
				new_features[insert_index].setFeature(mine.getFeature());
				new_features[insert_index++].setValue(new_value);
			}
		}
	}

	while (i < getCount()) {
		new_features[insert_index++] = features[i++];
	}

	while (j < other.getCount()) {
		new_features[insert_index].setFeature(other.features[j].getFeature());
		new_features[insert_index++].setValue(
				0.0 - other.features[j].getValue());
		j++;
	}

	features.reset(new NGramItem[insert_index]);
	count = insert_index;
	std::copy(new_features, new_features + insert_index, features.get());
	delete[] new_features;
	return *this;
}

NGram &NGram::operator *=(double arg) {
	if (double_close_to_0(arg))
		throw "Multiplication by zero not allowed";
	for (unsigned int i = 0; i < getCount(); i++)
		features[i].setValue(features[i].getValue() * arg);

	return *this;
}

bool NGram::operator ==(const NGram &arg) const {
	if (count != arg.count || !(*this->conf.get() == *arg.conf.get()))
		return false;

	for (unsigned int i = 0; i < count; i++)
		if (!(features[i] == arg.features[i]))
			return false;

	return true;
}

double NGram::operator *(const NGram &other) const {
	double inner_product = 0.0;
	for (unsigned int i = 0, j = 0; i < getCount() && j < other.getCount();) {
		const NGramItem &mine(features[i]);
		const NGramItem &theirs(other.features[j]);
		if (mine < theirs)
			i++;
		else if (theirs < mine)
			j++;
		else {
			inner_product += mine.getValue() * theirs.getValue();
			i++;
			j++;
		}
	}

	return inner_product;
}

double NGram::L1norm() const {
	double sum = 0.0;
	for (unsigned int i = 0; i < getCount(); i++)
		sum += features[i].getValue();

	return sum;
}
double NGram::L2norm() const {
	double sum_of_squares = 0.0;
	for (unsigned int i = 0; i < getCount(); i++)
		sum_of_squares += features[i].getValue() * features[i].getValue();

	return std::sqrt(sum_of_squares);
}

void NGram::trim(double min, double max) {
	for (unsigned int i = 0; i < getCount(); i++) {
		if (features[i].getValue() < min)
			features[i].setValue(min);
		if (features[i].getValue() > max)
			features[i].setValue(max);
	}
	if (double_close_to_0(min))
		this->sparsify();
}

std::vector<NGramItem> NGram::getTopN(unsigned int n) const {
	if (n == 0 || n > count)
		throw "Mind your 'n'";
	std::vector<NGramItem> top_n(n);
	std::partial_sort_copy(&(features[0]), &(features[count]), top_n.begin(),
			top_n.end(), NGramItem::compare_values_descending);
	return top_n;
}

std::ostream &operator <<(std::ostream &cout, const NGram &ngram) {
	//	std::cout << "@NGram::cout" << std::endl;
	cout << *(ngram.conf) << std::endl;
	cout << "Number of features: " << ngram.getCount() << std::endl << '\t';
	for (unsigned int i = 0; i < ngram.getCount(); i++) {
		ngram.features[i].print(cout, ngram.conf->getLength());
		cout << std::endl << '\t';
	}
	return cout;
}

std::ofstream &operator <<(std::ofstream &fout, const NGram &ngram) {
	//	std::cout << "@NGram::fout" << std::endl;
	fout << NGram::VERSION << std::endl;
	fout << *(ngram.conf.get()) << std::endl << ngram.getCount() << std::endl;
	for (unsigned int i = 0; i < ngram.getCount(); i++) {
		ngram.features[i].print(fout);
		fout << std::endl;
	}
	return fout;
}

std::ifstream &fin_v1(std::ifstream &fin, NGram &ngram) {
	NGram temp;
	fin >> *temp.conf >> std::ws >> temp.count >> std::ws;
	if (!fin)
		throw "Malformed n-gram information";

	temp.features.reset(new NGramItem[temp.getCount()]);

	for (unsigned int i = 0; i < temp.getCount(); i++) {
		unsigned long feature;
		double value;
		fin >> feature >> std::ws >> value >> std::ws;
		if (!fin)
			throw "Malformed n-gram feature list";
		temp.features[i].setFeature(feature);
		temp.features[i].setValue(value);
	}

	ngram = temp;
	return fin;
}

std::ifstream &operator >>(std::ifstream &fin, NGram &ngram) {
	unsigned int version = 0;
	fin >> std::ws >> version;
	if (!fin)
		throw "N-gram file version not recognized";
	if (version > NGram::VERSION)
		throw "Unsupported n-gram file version";
	switch (version) {
	case 1:
		return fin_v1(fin, ngram);
	default:
		throw "Unsupported n-gram file version";
	}
}

}
