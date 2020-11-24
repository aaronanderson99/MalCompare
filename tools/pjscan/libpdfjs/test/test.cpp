/*
 * Copyright 2012 Nedim Srndic, University of Tuebingen
 *
 * This file is part of libpdfjs.

 * libpdfjs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libpdfjs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpdfjs.  If not, see <http://www.gnu.org/licenses/>.
 *
 * test.cpp
 * Created on: Oct 7, 2011
 */

#include <iostream>
#include <pdfjs/PdfJsExtractor.h>
#include <cstdlib>

int main() {
	char pdf_file[] = "/path/to/file";
	pdfjs::PdfJsExtractor *pje = new pdfjs::PdfJsExtractor();
	// Perform the scanning
	if (!pje->searchDocument(pdf_file)) {
		// Scanning unsuccesful, handle the error
		std::exit(EXIT_FAILURE);
	} else {
		// Scanning successful, iterate through JavaScript scripts in this PDF file
		for (int i = 0; i < pje->getJsCount(); i++) {
			// Do something with the script, for example write to stdout
			std::cout << pje->getExtractedJs(i) << std::endl;
		}
	}

	// Done using PdfJsExtractor
	delete pje;
	return EXIT_SUCCESS;
}
