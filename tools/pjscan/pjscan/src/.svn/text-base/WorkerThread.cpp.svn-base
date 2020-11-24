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
 * WorkerThread.cpp
 *  Created on: Apr 28, 2011
 */

#include "WorkerThread.h"
#include "tokens.h"

#include <cstdio>	// perror()
#include <cstdlib>	// EXIT_FAILURE
#include <climits>	// PIPE_BUF
#include <string>
#include <iostream>
#include <vector>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <signal.h> // kill()
#include <sys/wait.h> // waitpid()
#include <sys/types.h>	// fork(), open()
#include <fcntl.h>	// open(), fcntl()
#include <unistd.h>	// pipe(), close(), fork(), dup2(), execl(), fcntl()
#include <errno.h>	// errno

/*
 * Forks a new process and connects it's standard output to the parent's
 * standard input, then runs execl() to run pjscan-child. Returns the child's
 * PID.
 * From http://snippets.dzone.com/posts/show/1134, modified
 */
pid_t popen2(const char *file_name, int *outfp) {
	const int READ = STDIN_FILENO;
	const int WRITE = STDOUT_FILENO;
	const int ERR = STDERR_FILENO;
	int p_stdout[2];
	pid_t pid;

	// Create a pipe
	if (pipe(p_stdout) == -1)
		return -1;

	pid = fork();

	if (pid == -1) {
		// Fork failed
		close(p_stdout[READ]);
		close(p_stdout[WRITE]);
		return pid;
	} else if (pid == 0) {
		// Child process
		// Don't read from stdout
		close(p_stdout[READ]);
		// Pipe p_stdout to stdout
		if (dup2(p_stdout[WRITE], WRITE) == -1)
			std::exit(EXIT_FAILURE);

		// Pipe stderr to /dev/null
		int std_err = open("/dev/null", O_WRONLY);
		if (std_err == -1)
			std::exit(EXIT_FAILURE);
		if (dup2(std_err, ERR) == -1)
			std::exit(EXIT_FAILURE);

		// Replace process image
		execl("/usr/local/bin/pjscan-child", "pjscan-child", file_name,
				(char *) NULL);
		// Replace failed, show error and exit
		std::perror("execl");
		std::exit(EXIT_FAILURE);
	} else {
		// Parent process
		// Return a handle to the newly created pipe
		*outfp = p_stdout[READ];
		// Don't write to the new pipe
		close(p_stdout[WRITE]);
		// Use non-blocking reads
		int flags = fcntl(p_stdout[READ], F_GETFL, 0);
		if (flags == -1) {
			close(p_stdout[READ]);
			perror("fcntl F_GETFL");
			return -2;
		}

		if (fcntl(p_stdout[READ], F_SETFL, flags | O_NONBLOCK) == -1) {
			close(p_stdout[READ]);
			perror("fcntl F_SETFL");
			return -2;
		}
		return pid;
	}
}

boost::mutex popen2_mutex;
void WorkerThread::message(const char *message) const {
	boost::mutex::scoped_lock lock(popen2_mutex);
	std::cout << "Thread " << boost::this_thread::get_id() << ": " << message
			<< std::endl;
}

void WorkerThread::operator ()(void) {
	if (file_name == 0) {
		this->message("File name not set");
		return;
	}
	if (map == 0) {
		this->message("Map not set");
		return;
	}
	if (id == (unsigned int) -1) {
		this->message("Result id not set");
		return;
	}
	// Put an empty ngram first to make sure there is something returned 
	// even if the thread fails
	map->insert(id, stem::NGram());

	// Runs a new instance of pjscan-child for this PDF file
	int fd;
	const pid_t PID = popen2(file_name, &fd);
	if (PID == -1) {
		message("Failed to open pjscan-child");
		return;
	} else if (PID == -2) {
		message("Failed to use nonblocking reads");
		return;
	}

	/*
	 * Fill a string (buffer) with the data output from pjscan-child before
	 * doing any parsing.
	 */
	std::string buffer;
	buffer.reserve(1024);
	char buf[PIPE_BUF];
	size_t nbytes = sizeof(buf);
	ssize_t bytes_read;
	unsigned int read_errors = 0;
	while (true) {
		bytes_read = read(fd, buf, nbytes);
		if (bytes_read > 0) { // Success
			buffer.append(buf, bytes_read);
		} else if (bytes_read == 0) { // EOF
			break;
		} else if (bytes_read == -1 && errno == EAGAIN) { // Empty pipe
			static const boost::posix_time::time_duration timeout =
					boost::posix_time::milliseconds(10);
			read_errors++;
			if (read_errors == 15000) { // ~150 seconds of waiting
				message((std::string("bailed out: ") + file_name).c_str());
				close(fd);
				kill(PID, SIGKILL);
				return;
			} else {
				boost::this_thread::sleep(timeout);
			}
		} else { // Error
			message("read() error");
			close(fd);
			waitpid(PID, NULL, 0);
			return;
		}
	}
	close(fd);
	waitpid(PID, NULL, 0);

	// Parse token sequences from the input
	const char *input = buffer.c_str();
	unsigned long tok_ind = 0UL;
	std::vector<stem::NGram> ngrams;
	stem::NGramConf conf(stem::BINARY, 4);
	while (true) {
		char *num_end;
		errno = 0;
		long token = strtol(input, &num_end, 10);
		if (errno != 0) {
			message("Invalid input");
			return;
		} else if (input == num_end) {
			// No valid digits found
			goto end_input_processing;
		}

		// Every token must be followed by a ' ', even the last one
		if (*num_end != ' ') {
			message("Invalid input");
			return;
		}
		// Check for the token sequence delimiter (a newline character)
		if (*(num_end + 1) == '\n') {
			// We have a new token sequence
			// Make sure the last token remains between SM_TOK_ERROR and 255
			if (token < SM_TOK_ERROR || token > 255L) {
				message("Invalid token");
				return;
			}

			// Convert SM_TOKEN_ERROR to TOK_ERROR
			if (token == (long) SM_TOK_ERROR) {
				tok_buf[tok_ind++] = (unsigned char) TOK_ERROR;
			} else {
				// Append token
				tok_buf[tok_ind++] = (unsigned char) token;
				// If the processing was interrupted there is no SM_TOK_EOF
				if (token != SM_TOK_EOF) {
					if (tok_ind == MAX_TOKENS) {
						message("Token capacity exceeded");
						return;
					} else {
						tok_buf[tok_ind++]
								= (unsigned char) TOK_ERROR;
					}
				}
			}

			// Embed the new token sequence
			if (tok_ind >= conf.getLength())
				ngrams.push_back(stem::NGram(conf, tok_buf, tok_ind));
			tok_ind = 0UL;
		} else {
			// We have a new token
			// Make sure the token remains between 0 and 255
			if (token < 0L || token > 255L) {
				message("Invalid token");
				return;
			}
			tok_buf[tok_ind++] = (unsigned char) token;
			if (tok_ind == MAX_TOKENS) {
				message("Token capacity exceeded");
				return;
			}
		}

		input = num_end + 1;
	}
	end_input_processing:

	// No need to further process empty files
	if (ngrams.size() == 0) {
		return;
	}

	// Add counts of all ngrams into the first one
	for (unsigned int i = 1; i < ngrams.size(); i++)
		ngrams[0] += ngrams[i];
	// Binarize the counts
	ngrams[0].trim(0.0, 1.0);
	// Normalize the counts
	ngrams[0].reembed(stem::BINARY_NORMALIZED);
	map->insert(id, ngrams[0]);
}
