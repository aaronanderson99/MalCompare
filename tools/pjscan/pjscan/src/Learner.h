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
 * Learner.h
 *  Created on: Apr 29, 2011
 */

#ifndef LEARNER_H_
#define LEARNER_H_

#include <vector>
#include <string>

/*
 * A class that encapsulates the learning algorithm.
 */
class Learner {
private:
	// Path to the file where the learned model should be saved
	const char *model_file;
	// PDF files to scan
	std::vector<std::string> pdf_files;
	// The nu parameter for the One-class SVM
	double nu;
public:
	// Constructor
	Learner(const char *model_file, const std::vector<std::string> &pdf_files, double nu);

	// Learns the model and saves it to a file
	void learn() const;
};

#endif /* LEARNER_H_ */
