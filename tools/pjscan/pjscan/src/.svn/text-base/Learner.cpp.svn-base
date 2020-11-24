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
 * Learner.cpp
 *  Created on: Apr 29, 2011
 */

#include <map>
#include <stem/NGram.h>
#include <svm_oc/SvmOc.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "Learner.h"
#include "ThreadPool.h"
#include "timing.h"

using std::cout;
using std::endl;

Learner::Learner(const char *model_file,
		const std::vector<std::string> &pdf_files, double nu) :
	model_file(model_file), pdf_files(pdf_files), nu(nu) {
	if (model_file == 0)
		throw "Learner: Invalid model file.";
	if (pdf_files.size() == 0)
		throw "Learner: Invalid PDF files.";
	if (nu < 0.0 or nu > 1.0) {
		throw "Learner: Invalid value for 'nu', must be between 0.0 and 1.0.";
	}
}

void Learner::learn() const {
	// Run a thread pool to embed PDF files into a vector space
	cout << "Learner: Extracting ngrams from PDF files" << endl;
	ThreadPool *pool = new ThreadPool(pdf_files);
	TIME_STATEMENT(pool->run(), cout)
#ifdef _DO_TIMING
	cout << " ==> total learning time" << endl;
#endif // _DO_TIMING
	std::vector<stem::NGram> ngrams(pool->getResults());
	cout << "Returned " << std::dec << ngrams.size() << " ngrams" << endl;
	delete pool;

	// Put non-empties into an array
	stem::NGram *ngram_array = new stem::NGram[pdf_files.size()];
	unsigned int ngram_count = 0;
	unsigned int empty_count = 0;
	for (unsigned int i = 0; i < ngrams.size(); i++) {
		if (ngrams[i].getCount() > 0) {
			ngram_array[ngram_count++] = ngrams[i];
		} else {
			empty_count++;
		}
	}

	if (ngram_count == 0) {
		delete[] ngram_array;
		cout << "Learner: There seem to be no JavaScript scripts in any of the PDF files, or the scripts are too short. Unable to learn." << endl;
		return;
	}

	cout << "Empties: " << empty_count << endl;
	cout << "Learning with " << std::dec << ngram_count << " files:" << endl;

	// Create and train a One-class SVM
	stem::Kernel *kernel = new stem::LinearKernel();
	svm_oc::SvmOc *svm = new svm_oc::SvmOc(nu, kernel);
	TIME_STATEMENT(svm->learn(ngram_array, ngram_count), cout)
#ifdef _DO_TIMING
	cout << " ==> Total learning time" << endl;
#endif // _DO_TIMING

	cout << "Learning done!\nRadius: " << svm->getRadius() << endl;
	cout << "Saving model file to: " << model_file << endl;

	try {
		if (!svm->serialize(model_file)) {
			cout << "Problem writing model file" << endl;
			goto destroy_and_exit;
		}
	} catch (const char message[]) {
		cout << message << endl;
		goto destroy_and_exit;
	} catch (...) {
		cout << "Exception caught, exiting" << endl;
		goto destroy_and_exit;
	}

	destroy_and_exit:
	delete[] ngram_array;
	delete svm;
	delete kernel;
}
