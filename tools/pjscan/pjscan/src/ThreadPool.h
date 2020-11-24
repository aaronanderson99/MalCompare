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
 * ThreadPool.h
 *  Created on: Apr 29, 2011
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>
#include <string>
#include <stem/NGram.h>
#include <map>
#include <boost/thread.hpp>

#include "WorkerThread.h"
#include "ThreadSafeMap.h"

/*
 * A class representing a pool of worker threads. The number of threads is
 * kept constant over time, and it depends on the number of hardware thread
 * execution units available. It allocates N-1 threads on a system with N
 * execution units (processor cores or HyperThreading units). Every concurrent
 * thread gets a static preallocated buffer to reduce memory allocation.
 */
class ThreadPool {
private:
	// PDF files to scan
	std::vector<std::string> pdf_files;
	// A map to hold the processing results
	ThreadSafeMap thread_map;
	// Maximum number of threads to run concurrently
	const unsigned int MAX_THREAD_COUNT;
	// Size of a buffer for one thread's storage
	const unsigned long THREAD_BUFFER_SIZE;
public:
	// Constructor
	ThreadPool(const std::vector<std::string> &pdf_files) :
				pdf_files(pdf_files),
				thread_map(),
				MAX_THREAD_COUNT(
						boost::thread::hardware_concurrency() > 1 ?
								boost::thread::hardware_concurrency() - 1 : 1),
				THREAD_BUFFER_SIZE(2UL * 1024UL * 1024UL){
		if (pdf_files.size() < 1)
			throw "ThreadPool: There must be at least one file to scan.";
	}

	// Runs the thread pool until all threads finish
	void run();
	// Returns the results
	std::vector<stem::NGram> getResults();
};

#endif /* THREADPOOL_H_ */
