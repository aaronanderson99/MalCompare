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
 * ThreadSafeMap.cpp
 *  Created on: Apr 29, 2011
 */

#include <sstream>

#include "ThreadSafeMap.h"

// The type of items in the map
typedef std::pair<unsigned int, stem::NGram> map_item_t;
// The type of the map iterator
typedef std::map<unsigned int, stem::NGram>::iterator map_iterator_t;

void ThreadSafeMap::insert(unsigned int key, const stem::NGram &value) {
	// Get access for writing
	boost::mutex::scoped_lock lock(mutex);

	static std::pair<map_iterator_t, bool> ret;
	map[key] = value;

	/*if (ret.second == false) {
		// The element with the specified key already existed
		// This should never happen
		throw "ThreadSafeMap: Non-uniqe element inserted";
	}*/

	// Lock gets released automatically by the destruction of the lock object
	// it seems sometimes the lock does not get released automatically :S
	// XXX necessary bug fix in the library?
	lock.unlock();
}

stem::NGram ThreadSafeMap::get(unsigned int key) {
	// Get access for reading
	boost::mutex::scoped_lock lock(mutex);

	static map_iterator_t it;
	it = map.find(key);

	if (it == map.end()) {
		// The element with the specified key does not exist
		// This should never happen
		std::ostringstream os;
		os << "ThreadSafeMap: Element not found: " << key;
		throw os.str().c_str();
	}
	stem::NGram result((*it).second);

	// Lock gets released automatically by the destruction of the lock object
	return result;
}
