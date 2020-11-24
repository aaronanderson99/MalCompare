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
 * Classifier.cpp
 *  Created on: Apr 29, 2011
 */

#include <iostream>
#include <fstream>
#include <svm_oc/SvmOc.h>

#include "Classifier.h"
#include "ThreadPool.h"
#include "timing.h"

using std::cout;
using std::endl;

Classifier::Classifier(const char *model_file,
		const std::vector<std::string> &pdf_files) :
	model_file(model_file), pdf_files(pdf_files) {
	if (model_file == 0)
		throw "Classifier: Invalid model file.";
	if (pdf_files.size() == 0)
		throw "Classifier: Invalid PDF files.";
}

void Classifier::classify() const {
	// Deserialize SVM
	svm_oc::SvmOc svm(0.1, stem::LinearKernel::create());
	try {
		svm_oc::SvmOc::deserialize(model_file, svm);
	} catch (const char message[]) {
		cout << message << endl;
		return;
	} catch (...) {
		cout << "Classifier: Problem deserializing model from file" << endl;
		return;
	}

	// Run a thread pool to embed PDF files into a vector space
	cout << "Classifier: Extracting ngrams from PDF files" << endl;
	ThreadPool *pool = new ThreadPool(pdf_files);
	TIME_STATEMENT(pool->run(), cout)
#ifdef _DO_TIMING
	cout << " ==> total classification time:" << endl;
	std::ofstream timing_file("/dev/shm/pjscan/classify", std::fstream::trunc);
#endif // _DO_TIMING
	std::vector<stem::NGram> ngrams(pool->getResults());
	cout << "Returned " << std::dec << ngrams.size() << " ngrams" << endl;
	delete pool;

	// Classify files
	cout << "Classifier: Classifying " << pdf_files.size() << " files:" << endl;
	double radius = svm.getRadius();
	for (unsigned int i = 0; i < ngrams.size(); i++) {
		// Skip empties
		if (ngrams[i].getCount() == 0)
			continue;
		cout << pdf_files[i] << ": ";
		double distance;
		TIME_STATEMENT(distance = svm.getDistance(ngrams[i]), timing_file)
		if (distance <= radius)
			cout << "malicious";
		else
			cout << "benign";
		cout << " [" << distance << ']' << endl;
	}

	cout << "All files that contain no ngrams were omitted." << endl;
#ifdef _DO_TIMING
	timing_file.close();
#endif // _DO_TIMING
}
