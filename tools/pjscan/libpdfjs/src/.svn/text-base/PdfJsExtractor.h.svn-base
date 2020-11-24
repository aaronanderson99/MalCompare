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
 * PdfJsExtractor.h
 * Created on: Feb 9, 2011
 */

#ifndef PDFJSEXTRACTOR_H_
#define PDFJSEXTRACTOR_H_

#include <poppler/Object.h>
#include <poppler/Catalog.h>
#include <poppler/XRef.h>
#include <poppler/GlobalParams.h>
#include <poppler/UnicodeMap.h>
#include <poppler/Annot.h>
#include <poppler/Dict.h>
#include <string>
#include <vector>

namespace pdfjs {
/*!
 * \brief A class representing an extraction mechanism for JavaScript embedded
 * inside PDF files.
 *
 * Copying and assignment are forbidden.
 */
class PdfJsExtractor {
private:
	// All the javascript that is extracted from one document
	std::vector<std::string> extracted_js;
	// PDF document's Cross-Reference stream
	XRef *xref;
	// PDF document's Catalog dictionary
	Catalog *catalog;
	// A UTF-8 map, used to convert from UTF-16-BE to UTF-8
	UnicodeMap *utf8map;

	// A flag to mark visited Action dictionaries, to avoid circular references
	static char visited_flag[];

	// Searches through the Catalog dictionary
	bool searchCatalog();
	// Searches for an AA entry in the specified dictionary object
	void searchForAA(Object *parent);
	// Searches inside an Additional Annotations dictionary
	void searchAA(Dict *AAdict);
	// Searches inside all the pages of the document
	void searchPageTree();
	// Searches inside Annotation dictionaries
	void searchAnnot(Annot *annot);
	// Starts the search through the Outline tree
	void searchOutlineTree(Object *outline_root);
	// Recursively searches through all the nodes of the Outline tree
	void searchOutline(Object *outline);
	// Starts the search of the Forms tree
	void searchAcroForm(Object *acroform);
	// Recursively searches through all the nodes of the Forms tree
	void searchFieldDictionary(Object *field);
	// Starts the search of the JavaScript name tree
	void searchNameTree(Object *cat_obj);
	// Recursively searches through all the nodes of the JavaScript name tree
	void searchNameTreeNodeDictionary(Object *node);

	// Looks for JS code in an Action dictionary, recursively
	void searchActionDictionary(Dict *action_dict);
	// Extracts JavaScript code from a PDF Action object
	bool getJsFromAction(Dict *action);

	// Returns true if the 'visited' flag is set on an Action dictionary
	bool getActionDictionaryVisited(Dict *action_dict);
	// Sets a 'visited' flag of an Action dictionary to true
	void setActionDictionaryVisited(Dict *action_dict);

	// Forbid copying
	PdfJsExtractor(const PdfJsExtractor &);
	// Forbid assignment
	PdfJsExtractor &operator =(const PdfJsExtractor &);
public:
	/*!
	 * \brief The default constructor.
	 */
	PdfJsExtractor();
	/*!
	 * \brief The destructor.
	 */
	~PdfJsExtractor();

	/*!
	 * \brief Searches through the specified PDF document.
	 *
	 * @param file_name the name of the file to search in.
	 * @return true on success, otherwise false.
	 */
	bool searchDocument(const char *file_name);
	/*!
	 * \brief Returns the number of JS scripts extracted.
	 *
	 * @return the number of JS scripts extracted.
	 */
	int getJsCount() const {
		return extracted_js.size();
	}
	/*!
	 * \brief Returns extraction results.
	 *
	 * @return a UTF-8-encoded string containing one extracted JS script with
	 * the index i or NULL if the specified index is out of bounds.
	 */
	const char *getExtractedJs(int i) const {
		if (i < 0 || i >= getJsCount())
			return NULL;
		return extracted_js[i].c_str();
	}
};
}

#endif /* PDFJSEXTRACTOR_H_ */
