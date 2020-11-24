# Copyright 2012 Nedim Srndic, University of Tuebingen
# 
# This file is part of libpdfjs.
#
# libpdfjs is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# libpdfjs is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with libpdfjs.  If not, see <http://www.gnu.org/licenses/>.
# 
# PdfJsExtractor.i
 
%module pdfjs

%{
#define SWIG_FILE_WITH_INIT
#include "../src/PdfJsExtractor.h"
%}

namespace pdfjs {
class PdfJsExtractor {
public:
   PdfJsExtractor();
   virtual ~PdfJsExtractor();

   // Searches through a PDF document
   bool searchDocument(const char *file_name);
   // Returns the number of JS objects extracted
   int getJsCount() const {
      return extracted_js.size();
   }
   // Returns a string containing all the extracted JS code
   const char *getExtractedJs(int i) const {
      return extracted_js[i].c_str();
   }
};
}