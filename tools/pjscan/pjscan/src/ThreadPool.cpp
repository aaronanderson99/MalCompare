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
 * ThreadPool.cpp
 *  Created on: Apr 29, 2011
 */

#include <boost/date_time.hpp>
#include <algorithm> // find()
#include "ThreadPool.h"

using std::cout;
using std::endl;

/*
 * Runs a constant number of threads concurrently.
 */
void ThreadPool::run() {
	cout << "ThreadPool running with " << MAX_THREAD_COUNT << " threads."
			<< endl;
	// Index of the next job/thread to start
	unsigned int next_job = 0;
	// Number of finished jobs/threads
	unsigned int jobs_done = 0;
	// Primitive Boost thread pool
	boost::thread_group ts;
	// Pointers to the threads in the thread pool. Needed to reference them
	boost::thread **tps = new boost::thread*[MAX_THREAD_COUNT];
	// Preallocated buffers to be reused by threads
	char **tbs = new char*[MAX_THREAD_COUNT];
	for (unsigned int i = 0; i < MAX_THREAD_COUNT; i++) {
		tbs[i] = new char[THREAD_BUFFER_SIZE];
		tps[i] = (boost::thread *) NULL;
	}
	// Helper static const variable
	static const boost::posix_time::time_duration timeout =
			boost::posix_time::milliseconds(0);

	// Go through all jobs to be done
	while (jobs_done < pdf_files.size()) {
		/*
		 * Start a new job/thread if the number of concurrently running threads
		 * is not at its maximum, or if all jobs have not yet been started
		 */
		if (ts.size() < MAX_THREAD_COUNT && next_job < pdf_files.size()) {
			// Start a new thread
			WorkerThread worker;
			worker.setPdfFileName(pdf_files[next_job].c_str());
			worker.setMap(&thread_map);
			worker.setResultId(next_job);
			// Find the first unused slot in the thread pool
			unsigned int tokbufi = std::find(tps, tps + MAX_THREAD_COUNT,
					(boost::thread *) NULL) - tps;
			worker.setTokBuf(tbs[tokbufi], THREAD_BUFFER_SIZE);
			next_job++;
			// This actually starts the thread
			boost::thread *thread = ts.create_thread(worker);
			tps[tokbufi] = thread;
		}

		/*
		 * Stop a thread if the maximum number of concurrently running threads
		 * has been reached, or there are no more jobs/threads to start
		 */
		if (ts.size() == MAX_THREAD_COUNT || next_job == pdf_files.size()) {
			// Poll all the threads in the pool until at least one thread
			// voluntarily finishes
			unsigned int i = 0;
			for (;; i = (i + 1) % MAX_THREAD_COUNT) {
				if (tps[i] != (boost::thread *) NULL && tps[i]->timed_join(
						timeout)) {
					break;
				}
			}

			// Deallocate the finished thread
			ts.remove_thread(tps[i]);
			delete tps[i];
			tps[i] = (boost::thread *) NULL;
			jobs_done++;
			unsigned int jobs_remaining = pdf_files.size() - jobs_done;
			if (jobs_remaining % 100 == 0 || jobs_remaining < 100)
				cout << pdf_files.size() - jobs_done << endl;
		}
	}

	// Deallocate memory
	for (unsigned int i = 0; i < MAX_THREAD_COUNT; i++) {
		delete[] tbs[i];
	}
	delete[] tbs;
	delete[] tps;

	cout << "ThreadPool finished." << endl;
}

std::vector<stem::NGram> ThreadPool::getResults() {
	std::vector<stem::NGram> results;
	for (unsigned int i = 0; i < pdf_files.size(); i++)
		results.push_back(thread_map.get(i));

	return results;
}
