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
 * PdfJsExtractor.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: nedim
 */

#include "PdfJsExtractor.h"
#include <poppler/PDFDoc.h>
#include <poppler/GlobalParams.h>
#include <poppler/Catalog.h>
#include <poppler/XRef.h>
#include <poppler/PDFDocEncoding.h>
#include <poppler/UnicodeMap.h>
#include <poppler/Annot.h>
#include <memory>
#include <string>

namespace pdfjs {
char PdfJsExtractor::visited_flag[] = "VisitedByPopplerJS";

/*
 * Some important dictionary entry names that are queried more than once.
 */
static char queryA[] = "A";
static char queryAA[] = "AA";
static char queryFirst[] = "First";
static char queryKids[] = "Kids";
static char queryNames[] = "Names";
static char queryNext[] = "Next";

PdfJsExtractor::PdfJsExtractor() :
		extracted_js(), xref(0), catalog(0) {
	if (globalParams == NULL)
		globalParams = new GlobalParams();

	utf8map = globalParams->getUnicodeMap(new GooString("UTF-8"));
}

PdfJsExtractor::~PdfJsExtractor() {
	delete globalParams;
	/* These pointers are not allocated inside the constructor
	 * and should not be freed:
	 * xref
	 * catalog
	 * utf8map
	 */
}

bool PdfJsExtractor::searchDocument(const char *file_name) {
	// Clear the (possibly) existing scripts
	extracted_js.clear();
	// Open the PDF document
	PDFDoc* doc(new PDFDoc(new GooString(file_name)));
	/*
	 *
	 * For some versions of Poppler, deleting doc will generate a
	 * segmentation fault, so DO NOT DELETE doc!
	 *
	 *
	 */
	if (!doc->isOk()) {
		//		print_warn("Loading error");
//		delete doc;
		return false;
	}

	catalog = doc->getCatalog();
	if (!catalog->isOk()) {
		//		print_warn("Catalog NOT OK!");
//		delete doc;
		return false;
	}

	xref = doc->getXRef();
	if (!xref->isOk()) {
		//		print_warn("XRef NOT OK!");
//		delete doc;
		return false;
	}

	if (!searchCatalog()) {
//		delete doc;
		return false;
	}

//	delete doc;
	return true;
}

bool PdfJsExtractor::searchCatalog() {
	Object cat_obj;
	xref->getCatalog(&cat_obj);
	if (!cat_obj.isDict()) {
		//		print_warn("XRef Catalog problem");
		return false;
	}

	// Search all the known locations where JS can be found
	// Search in Catalog::AA
	searchForAA(&cat_obj);
	// Search in Catalog::OpenAction
	Object open_action;
	static char query[] = "OpenAction";
	cat_obj.dictLookup(query, &open_action);
	if (open_action.isDict())
		searchActionDictionary(open_action.getDict());

	// Search in Catalog::Pages
	searchPageTree();
	if (!catalog->getOutline()->isNull())
		searchOutlineTree(catalog->getOutline()); // Search in Catalog::Outline
	if (catalog->getAcroForm()->isDict())
		searchAcroForm(catalog->getAcroForm()); // Search in Catalog::AcroForm
	// Search in Catalog::Names
	searchNameTree(&cat_obj);

	return true;
}

void PdfJsExtractor::searchActionDictionary(Dict *action_dict) {
	/* Action dictionaries are the PDF objects that actually contain JavaScript.
	 * There are many types of these objects, and only the JavaScript type can
	 * contain JavaScript code. Since they can form trees, one has to search
	 * all the Action dictionaries referenced by the Next entry, and to take
	 * care not to enter an infinite loop due to circular references. That's why
	 * all the visited nodes get marked by a new dictionary entry.
	 */
	// If already visited, bail out
	if (getActionDictionaryVisited(action_dict))
		return;
	else
		setActionDictionaryVisited(action_dict); // mark as visited

	// Get the JS string, if there is one
	if (getJsFromAction(action_dict)) {
		//		cout << "ActionDictionary: \n" << gs->getCString() << endl;
	}

	// Look at all the 'Next' actions
	Object next;
	action_dict->lookup(queryNext, &next);
	if (next.isArray()) {
		Array *nexts = next.getArray();
		for (int i = 0; i < nexts->getLength(); i++) {
			Object action;
			nexts->get(i, &action);
			if (!action.isNull() && action.isDict())
				searchActionDictionary(action.getDict());
		}
	} else if (next.isDict()) {
		searchActionDictionary(next.getDict());
	}
}

bool PdfJsExtractor::getActionDictionaryVisited(Dict *action_dict) {
	Object flag;
	action_dict->lookup(PdfJsExtractor::visited_flag, &flag);
	bool visited = !flag.isNull();
	return visited;
}

void PdfJsExtractor::setActionDictionaryVisited(Dict *action_dict) {
	if (getActionDictionaryVisited(action_dict))
		return;

	Object visited_value;
	action_dict->set(PdfJsExtractor::visited_flag, &visited_value);
}

void PdfJsExtractor::searchForAA(Object *parent) {
	/* The AA entry of a dictionary contains an Additional Annotation dictionary
	 */
	Object AA;
	parent->dictLookup(queryAA, &AA);
	if (AA.isDict())
		searchAA(AA.getDict());
}

void PdfJsExtractor::searchAA(Dict *AAdict) {
	/* Every entry in an Additional Annotations dictionary should be an
	 * action dictionary, that can contain JS
	 */
	for (int i = 0; i < AAdict->getLength(); i++) {
		Object action;
		AAdict->getVal(i, &action);
		if (action.isDict())
			searchActionDictionary(action.getDict());
	}
}

void PdfJsExtractor::searchPageTree() {
	/* A page tree consists of a tree of Page Tree Node dictionaries and Page
	 * dictionaries. The Page dictionaries are the leaves of the tree, and
	 * they can contain references to JS in the AA and Annots entries.
	 */
	for (int i = 1; i <= catalog->getNumPages(); i++) {
		// Search the Page::AA entry
		Object AA;

		if(catalog->getPage(i) == NULL)
			continue;

		catalog->getPage(i)->getActions(&AA);
		if (AA.isDict())
			searchAA(AA.getDict());

		// Search the Page::Annots entry
		Annots *annots = catalog->getPage(i)->getAnnots(catalog);
		for (int j = 0; j < annots->getNumAnnots(); j++) {
			searchAnnot(annots->getAnnot(j));
		}
		delete annots;
	}
}

void PdfJsExtractor::searchAnnot(Annot *annot) {
	/* PDF Annotations can contain JavaScript Action dictionaries or Additional
	 * Action dictionaries.
	 */

	Object my_annot;
	Ref annot_ref = annot->getRef();
	xref->fetch(annot_ref.num, annot_ref.gen, &my_annot);
	if (my_annot.isDict()) {
		// Look for an Action dictionary
		Object action;
		my_annot.dictLookup(queryA, &action);
		if (action.isDict())
			searchActionDictionary(action.getDict());

		// Look for an Additional Action dictionary
		Object aa;
		static char queryAA[] = "AA";
		my_annot.dictLookup(queryAA, &aa);
		if (aa.isDict())
			searchAA(aa.getDict());
	}
}

void PdfJsExtractor::searchOutlineTree(Object *outline_root) {
	/* An outline tree consists of a tree of Outline dictionaries and
	 * Outline Item dictionaries. The latter are the leaves of the tree, and
	 * can contain references to Action dictionaries containing JS.
	 */

	// Sanity check, errors happened before
	if (!outline_root->isDict())
		return;
	// Look for the First entry of the root node of the dictionary tree
	Object first;
	outline_root->dictLookup(queryFirst, &first);
	if (first.isDict())
		searchOutline(&first);
}

void PdfJsExtractor::searchOutline(Object *outline) {
	/* Recursively iterates through all the Outline dictionaries
	 * looking for JS */

	// Look for an A entry, containing an Action dictionary
	Object action;
	outline->dictLookup(queryA, &action);
	if (action.isDict())
		searchActionDictionary(action.getDict());

	// Recursively search in all the children of this node
	Object first;
	outline->dictLookup(queryFirst, &first);
	if (first.isDict())
		searchOutline(&first);

	// Recursively search in all the other nodes on the same level of the tree
	Object next;
	outline->dictLookup(queryNext, &next);
	if (next.isDict())
		searchOutline(&next);
}

void PdfJsExtractor::searchAcroForm(Object *acroform) {
	/* Forms are structured in a tree, whose root is in an Interactive Form
	 * Dictionary found in the AcroForm entry of the Catalog dictionary.
	 * The tree must be recursively searched, because the leaves can contain
	 * references to JS.
	 */

	// The root form fields are located in an array in the Fields entry
	Object fields;
	static char query[] = "Fields";
	acroform->dictLookup(query, &fields);
	if (fields.isArray())
		for (int i = 0; i < fields.arrayGetLength(); i++) {
			Object field;
			fields.arrayGet(i, &field);
			if (field.isDict())
				searchFieldDictionary(&field); // Search all fields
		}
}

void PdfJsExtractor::searchFieldDictionary(Object *field) {
	/* Recursively iterates through all the Field dictionaries, looking for
	 * references to JS.
	 * In some cases, Field dictionaries can be merged with Widget annotations,
	 * so we need to check if there is an A or AA entry
	 */

	// Look for an Action dictionary in the A entry
	Object action;
	field->dictLookup(queryA, &action);
	if (action.isDict())
		searchActionDictionary(action.getDict());

	// Look for an Additional Actions dictionary in the AA entry
	Object aa;
	field->dictLookup(queryAA, &aa);
	if (aa.isDict())
		searchAA(aa.getDict());

	// Recursively search in all the children of this node
	Object kids;
	field->dictLookup(queryKids, &kids);
	if (kids.isArray())
		for (int i = 0; i < kids.arrayGetLength(); i++) {
			Object kid;
			kids.arrayGet(i, &kid);
			if (kid.isDict())
				searchFieldDictionary(&kid);
		}
}

void PdfJsExtractor::searchNameTree(Object *cat_obj) {
	/* A PDF Document's Name tree can contain JavaScript that gets executed
	 * on document load.
	 */

	// Find the Names entry of the catalog, containing the Name tree root
	Object names;
	cat_obj->dictLookup(queryNames, &names);
	if (names.isDict()) {
		Object js_names;
		// Find the JavaScript entry, containing the root node of the
		// JavaScript name tree
		static char query[] = "JavaScript";
		names.dictLookup(query, &js_names);
		if (js_names.isDict())
			searchNameTreeNodeDictionary(&js_names);
	}
}

void PdfJsExtractor::searchNameTreeNodeDictionary(Object *node) {
	/* The tree shall always have exactly one root node, which shall contain a
	 * single entry: either Kids or Names but not both.
	 * If the root node has a Names entry, it shall be the only node in the tree.
	 * If it has a Kids entry, each of the remaining nodes shall be either an
	 * intermediate node, that shall contain a Limits entry and a Kids entry, or
	 * a leaf node, that shall contain a Limits entry and a Names entry.
	 */

	// Look for all the JS entries in the Names array
	Object names;
	node->dictLookup(queryNames, &names);
	if (names.isArray()) {
		Object name;
		// The array consists of pairs of values, where the i-th value is
		// a PDF name, and the i+1-th value is an indirect reference to the
		// PDF object that contains the value for this name
		for (int i = 1; i < names.arrayGetLength(); i += 2) {
			names.arrayGet(i, &name);
			if (name.isDict())
				searchActionDictionary(name.getDict());
		}
	}

	// Recursively search through the children of this node
	Object kids;
	node->dictLookup(queryKids, &kids);
	if (kids.isArray()) {
		Object kid;
		for (int i = 0; i < kids.arrayGetLength(); i++) {
			kids.arrayGet(i, &kid);
			if (kid.isDict())
				searchNameTreeNodeDictionary(&kid);
		}
	}
}

bool PdfJsExtractor::getJsFromAction(Dict *action) {
	/* JavaScript in PDF resides in the JS entry of JavaScript dictionaries.
	 * It can be a Text String, or a Text Stream. It can be encoded using
	 * PDFDocEncoding or UTF-16-BE.
	 */

	// Get the JS entry, if any
	Object js;
	static char query[] = "JS";
	action->lookup(query, &js);
	if (js.isNull()) {
		return false;
	}

	GooString *tmp;
	if (js.isString()) {
		// It's inside a Text String
		tmp = new GooString(js.getString());
	} else if (js.isStream()) {
		// It's inside a Text Stream
		Stream *stream = js.getStream();
		stream->reset();
		int j;
		tmp = new GooString();
		while ((j = stream->getChar()) != EOF) {
			tmp->append((char) j);
		}
	} else {
		// Malformed
		return false;
	}

	char *decoded = tmp->getCString();
	int utf16_len = tmp->getLength();
	bool was_decoded = true;
	// If it's not in UTF-16-BE
	if (tmp->getChar(0) != char(0xFE) || tmp->getChar(1) != char(0xFF)) {
		was_decoded = false;
		// Decode to UTF-16-BE
		decoded = pdfDocEncodingToUTF16(tmp, &utf16_len);
	}

	// Test if encoding can be done
	if (utf16_len % 2 == 1) {
		delete tmp;
		if (!was_decoded) {
			delete decoded;
		}
		return false;
	}

	// Encode to UTF-8
	char *buf = new char[utf16_len * 2];
	char *buf_begin = buf;
	int utf8_len = 0;
	// Encode all characters
	for (int i = 2; i < utf16_len; i += 2) {
		Unicode u = ((decoded[i] & 0xFF) << 8) | (decoded[i + 1] & 0xFF);
		int n = utf8map->mapUnicode(u, buf, 4);
		buf += n;
		utf8_len += n;
	}
	delete tmp;
	tmp = new GooString(buf_begin, utf8_len);
	if (tmp->getLength() > 0)
		extracted_js.push_back(tmp->getCString());
	delete[] buf_begin;
	if (!was_decoded) {
		delete[] decoded;
	}
	delete tmp;
	return true;
}
}
