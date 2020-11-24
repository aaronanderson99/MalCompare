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
 * JsDumper.cpp
 *  Created on: Jul 19, 2011
 */

#include <sstream>
#include <fstream>
#include <iostream>

// Must define before including boost/filesystem.hpp, still using version 2
#define BOOST_FILESYSTEM_VERSION 2
#include <boost/filesystem.hpp>
#include "pdfjs/PdfJsExtractor.h"

#include "JsDumper.h"

namespace boost_fs = boost::filesystem;

void JsDumper::dump() const {
	pdfjs::PdfJsExtractor *js_extractor = new pdfjs::PdfJsExtractor();
	bool scan_success = js_extractor->searchDocument(pdf_file);
	if (scan_success == false || js_extractor->getJsCount() == 0) {
		delete js_extractor;
		return;
	}

	std::cout << "Dumping " << js_extractor->getJsCount()
			<< " JavaScript scripts." << std::endl;

	std::ostringstream sout;
	for (int i = 0; i < js_extractor->getJsCount(); i++) {
		boost_fs::path dir = boost_fs::path(js_dump_dir);
		sout << i << ".js";
		std::ofstream js_file((dir /= sout.str()).file_string().c_str(),
				std::fstream::trunc);
		js_file << js_extractor->getExtractedJs(i);
		js_file.close();
		sout.str("");
	}

	delete js_extractor;
}
