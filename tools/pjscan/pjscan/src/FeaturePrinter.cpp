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
 * FeaturePrinter.cpp
 *  Created on: Jul 19, 2011
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include <svm_oc/SvmOc.h>
#include <stem/NGram.h>

#include "tokens.h"
#include "FeaturePrinter.h"
#include "ThreadPool.h"

void FeaturePrinter::print() {
	// Generate n-gram from file
	std::vector<std::string> files;
	files.push_back(pdf_file);
	ThreadPool pool(files);
	pool.run();
	stem::NGram ngram(pool.getResults()[0]);
	if (ngram.getCount() == 0) {
		std::cerr << "FeaturePrinter: No features found in PDF file."
				<< std::endl;
		return;
	} else if (ngram.getCount() < N) {
		N = ngram.getCount();
		std::cout << "Only " << N << " features available." << std::endl;
	} else {
		std::cout << "Found " << ngram.getCount() << " features in file."
				<< std::endl;
	}
	std::vector<stem::NGramItem> feats = ngram.getTopN(ngram.getCount());

	// Deserialize model
	svm_oc::SvmOc svm(0.1, stem::LinearKernel::create());
	try {
		svm_oc::SvmOc::deserialize(model_file, svm);
	} catch (const char message[]) {
		std::cerr << message << std::endl;
		return;
	} catch (...) {
		std::cerr
				<< "FeaturePrinter: Unknown problem deserializing model from file"
				<< std::endl;
		return;
	}

	// Pretty-print the features
	std::vector<stem::NGramItem> model(
			svm.getCenter().getTopN(svm.getCenter().getCount()));

	for (unsigned int i = 0; i < feats.size(); i++) {
		unsigned int position = 0;
		bool found = false;
		for (unsigned int j = 0; j < model.size(); j++) {
			if (feats[i] == model[j]) {
				position = j;
				found = true;
				break;
			}
		}

		if (found)
			feats[i].setValue(model[position].getValue() * feats[i].getValue());
		else
			feats[i].setValue(0.0);
	}
	std::sort(feats.begin(), feats.end(),
			stem::NGramItem::compare_values_descending);
	for (unsigned int i = 0; i < N; i++) {
		unsigned int position = 0;
		bool found = false;
		for (unsigned int j = 0; j < model.size(); j++) {
			if (feats[i] == model[j]) {
				position = j;
				found = true;
				break;
			}
		}

		std::cout << std::setw(5);
		if (found)
			std::cout << position + 1;
		else
			std::cout << "n/a";
		std::cout << ' ' << feats[i].getValue() << ' ';
		print_long_as_tokens(std::cout, feats[i].getFeature(), 4);
		std::cout << std::endl;
	}
}
