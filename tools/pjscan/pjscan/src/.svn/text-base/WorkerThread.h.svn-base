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
 * WorkerThread.h
 *  Created on: Apr 28, 2011
 */

#ifndef WORKERTHREAD_H_
#define WORKERTHREAD_H_

#include <stem/NGram.h>

#include "ThreadSafeMap.h"

/*
 * A thread that extracts an ngram from a PDF file. It spawns a child process
 * that runs the pjscan-child executable, and opens a pipe to read its output.
 * The output is parsed as a token sequence, which then gets embedded into an
 * NGram object.
 *
 * If the child process does not finish within a specified timeout, it gets
 * killed. This thread is supposed to always return normally.
 */
class WorkerThread {
	// The name of the PDF file
	const char *file_name;
	// The map where the results are saved
	ThreadSafeMap *map;
	// The ID of the result that this thread produces
	unsigned int id;
	// A pointer to an array for temporary storage
	char *tok_buf;
	// The size of the temporary storage
	unsigned long MAX_TOKENS;

	// Prints a formatted message to stdout using locks to ensure correct
	// printing
	void message(const char *message) const;
public:
	// Constructor (must have an empty constructor for Boost.Threading)
	WorkerThread() :
		file_name(0), map(0), id((unsigned int) -1), tok_buf(0), MAX_TOKENS(0) {
	}
	// Copy constructor currently automatic
	// Assignment operator currently automatic
	// Destructor
	virtual ~WorkerThread() {
	}

	// Sets the name of the PDF file
	void setPdfFileName(const char *name) {
		file_name = name;
	}
	// Sets the map
	void setMap(ThreadSafeMap *map) {
		this->map = map;
	}
	// Sets the ID of the result
	void setResultId(unsigned int id) {
		this->id = id;
	}
	// Sets the token buffer
	void setTokBuf(char *tb, unsigned long MAX) {
		this->tok_buf = tb;
		this->MAX_TOKENS = MAX;
	}

	// overloaded () operator (for Boost.Threading)
	void operator ()();
};
#endif /* WORKERTHREAD_H_ */
