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
 * Classifier.h
 *  Created on: Apr 29, 2011
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

/*
 * A class that encapsulates the classification process.
 */
class Classifier {
private:
	// Path to the file where the learned model is saved
	const char *model_file;
	// PDF files to classify
	std::vector<std::string> pdf_files;
public:
	// Constructor
	Classifier(const char *model_file,
			const std::vector<std::string> &pdf_files);

	// Classifies files
	void classify() const;
};

#endif /* CLASSIFIER_H_ */
