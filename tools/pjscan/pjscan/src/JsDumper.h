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
 * JsDumper.h
 *  Created on: Jul 19, 2011
 */

#ifndef JSDUMPER_H_
#define JSDUMPER_H_

/*
 * A class that performs dumping of JavaScript scripts from a PDF file.
 */
class JsDumper {
private:
	// The path to the PDF file that contains the JavaScript
	const char *pdf_file;
	// The path to the directory where the scripts should be dumped to
	const char *js_dump_dir;
public:
	// Constructor
	JsDumper(const char *pdf_file, const char *js_dump_dir) :
		pdf_file(pdf_file), js_dump_dir(js_dump_dir) {
		if (pdf_file == 0)
			throw "JsDumper: Invalid PDF file.";
		if (js_dump_dir == 0)
			throw "JsDumper: Invalid JavaScript dump directory.";
	}

	// Dumps JavaScript into files
	void dump() const;
};

#endif /* JSDUMPER_H_ */
