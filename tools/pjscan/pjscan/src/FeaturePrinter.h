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
 * FeaturePrinter.h
 *  Created on: Jul 19, 2011
 */

#ifndef FEATUREPRINTER_H_
#define FEATUREPRINTER_H_

/*
 * A class that performs printing of the top N most significant features in a
 * PDF file in comparison to the learned model.
 */
class FeaturePrinter {
private:
	// The path to the PDF file
	const char *pdf_file;
	// The file with the learned model
	const char *model_file;
	// Number of features to print
	unsigned long N;
public:
	FeaturePrinter(const char *pdf_file, const char *model_file,
			unsigned long N) :
		pdf_file(pdf_file), model_file(model_file), N(N) {
		if (pdf_file == 0)
			throw "FeaturePrinter: Invalid PDF file.";
		if (model_file == 0)
			throw "FeaturePrinter: Invalid model file.";
		if (N == 0)
			throw "FeaturePrinter: Invalid number of features.";
	}

	// Performs feature printing
	void print();
};

#endif /* FEATUREPRINTER_H_ */
