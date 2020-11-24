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
 * ThreadSafeMap.h
 *  Created on: Apr 29, 2011
 */

#ifndef THREADSAFEMAP_H_
#define THREADSAFEMAP_H_

#include <map>
#include <stem/NGram.h>
#include <boost/thread.hpp>

/*
 * A class representing a thread-safe map. It utilizes a mutex to ensure that
 * no two threads can access it at the same time.
 */
class ThreadSafeMap {
private:
	// The map data structure
	std::map<unsigned int, stem::NGram> map;
	// The mutex for synchronization
	boost::mutex mutex;
public:
	// Constructor
	ThreadSafeMap() :
		map() {
	}

	// Inserts an element
	void insert(unsigned int key, const stem::NGram &value);
	// Returns an element
	stem::NGram get(unsigned int key);
};

#endif /* THREADSAFEMAP_H_ */
