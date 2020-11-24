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
 * pjscan-child.cpp
 *  Created on: Apr 27, 2011
 */

#include <cstdio>	// perror()
#include <cstdlib>	// EXIT_FAILURE
#include <climits>	// PIPE_BUF
#include <sys/types.h>	// fork(), open()
#include <sys/wait.h>	// wait()
#include <sys/stat.h>	// open()
#include <fcntl.h>	// open(), fcntl()
#include <unistd.h>	// pipe(), close(), fork(), dup2(), execl(), fcntl()
#include <errno.h>	// errno
#include <pdfjs/PdfJsExtractor.h>

#include "timing.h"

/*
 * Forks a JavaScript tokenizer process (pjscan-js) and creates two pipes that
 * connect the child with the parent:
 *   1. p_chpa (pipe from child to parent), connecting the child's stdout to
 *       the parent
 *   2. p_pach (pipe from parent to child), connecting the parent to the
 *       child's stdin
 * Returns the child's PID on success, -1 if the child could not be created
 * or -2 if the child was created but there were subsequent errors.
 * Modification based on http://snippets.dzone.com/posts/show/1134
 */
pid_t pconnect(int *infd, int *outfd) {
	const int READ = STDIN_FILENO;
	const int WRITE = STDOUT_FILENO;
	const int ERR = STDERR_FILENO;
	int p_chpa[2]; // pipe from child to parent
	int p_pach[2]; // pipe from parent to child
	pid_t pid;

	// Create the pipes
	if (pipe(p_chpa) == -1)
		return -1;
	if (pipe(p_pach) == -1) {
		close(p_chpa[READ]);
		close(p_chpa[WRITE]);
		return -1;
	}

	pid = fork();

	if (pid == -1) {
		// Fork failed
		close(p_pach[READ]);
		close(p_pach[WRITE]);
		close(p_chpa[READ]);
		close(p_chpa[WRITE]);
		return pid;
	} else if (pid == 0) {
		// Child process
		// Don't read from stdout
		close(p_chpa[READ]);
		// Pipe p_chpa to stdout
		if (dup2(p_chpa[WRITE], WRITE) == -1)
			std::exit(EXIT_FAILURE);

		// Don't write to stdin
		close(p_pach[WRITE]);
		// Pipe p_pach to stdin
		if (dup2(p_pach[READ], READ) == -1)
			std::exit(EXIT_FAILURE);

		// Pipe stderr to /dev/null
		int std_err = open("/dev/null", O_WRONLY);
		if (std_err == -1)
			std::exit(EXIT_FAILURE);
		if (dup2(std_err, ERR) == -1)
			std::exit(EXIT_FAILURE);

		// Replace process image
		execl("/usr/local/bin/pjscan-js", "pjscan-js", (const char *) NULL,
				(char *) NULL);
		// Replace failed, show error and exit
		std::perror("execl");
		std::exit(EXIT_FAILURE);
	} else {
		// Parent process
		// Close unused pipes
		close(p_chpa[WRITE]);
		close(p_pach[READ]);
		// Return handles to newly created pipes
		*infd = p_chpa[READ];
		*outfd = p_pach[WRITE];
		// Use non-blocking reads
		int flags = fcntl(p_chpa[READ], F_GETFL);
		if (flags == -1) {
			close(p_pach[WRITE]);
			close(p_chpa[READ]);
			perror("fcntl F_GETFL");
			return -2;
		}

		if (fcntl(p_chpa[READ], F_SETFL, flags | O_NONBLOCK) == -1) {
			close(p_pach[WRITE]);
			close(p_chpa[READ]);
			perror("fcntl F_SETFL");
			return -2;
		}
		return pid;
	}
}

/*
 * pjscan-child - scans a PDF file for JavaScript code and prints the
 * lexical tokens of the found code
 *
 * How to run:
 * pjscan-child pdf_file
 *
 * pdf_file: 	An absolute path to a PDF file
 *
 * Exit status:
 *
 * EXIT_SUCCESS if no problems encountered, otherwise undefined
 */
int main(int argc, char *argv[]) {
	// Make sure we don't run as root
	if (getuid() == 0 || geteuid() == 0) {
		if (write(STDERR_FILENO, "This program cannot be run as root.\n", 36)
				> -2)
			std::exit(EXIT_FAILURE);
	}
	if (argc != 2)
		return EXIT_FAILURE;

	char *pdf_file_name = argv[1];
	// Initialize the PDF extraction library
	pdfjs::PdfJsExtractor *js_extractor = new pdfjs::PdfJsExtractor();
	bool scan_success;
	TIME_STATEMENT(scan_success = js_extractor->searchDocument(pdf_file_name),
			timing_file)
	if (scan_success == false || js_extractor->getJsCount() == 0)
		return EXIT_SUCCESS; // exit without delete

	const int JS_COUNT = js_extractor->getJsCount();
	// For each JavaScript object in this PDF file
	for (int jsi = 0; jsi < JS_COUNT; jsi++) {
		// Run the tokenizer and communicate via a pipe
		int infd, outfd;
		pid_t pid = pconnect(&infd, &outfd);
		if (pid == -1)
			return EXIT_FAILURE;
		else if (pid == -2) {
			wait((int *) NULL);
			return EXIT_FAILURE;
		}

		// Read and write to the pipes
		bool iswr = false, isr = false;
		unsigned long nwr = 0;
		const unsigned long WRITELEN =
				strlen(js_extractor->getExtractedJs(jsi));
		while (!iswr || !isr) {
			// Write
			if (!iswr) {
				unsigned long to_write = WRITELEN - nwr;
				if (to_write > PIPE_BUF)
					to_write = PIPE_BUF;
				ssize_t bwr = write(outfd,
						js_extractor->getExtractedJs(jsi) + nwr, to_write);
				if (bwr == -1) {
					if (errno == EAGAIN) {
						goto jump_read;
					} else {
						perror("write() to tokenizer pipe");
						return EXIT_FAILURE;
					}
				} else {
					nwr += bwr;
					if (nwr == WRITELEN) {
						iswr = true;
						close(outfd);
					}
				}
			}
			// Read
			jump_read: if (!isr) {
				// Warning: splice() is Linux-specific
				ssize_t nret = splice(infd, NULL, STDOUT_FILENO, NULL,
						PIPE_BUF, SPLICE_F_NONBLOCK);
				if (nret == -1) {
					if (errno == EAGAIN) {

						continue;
					} else {
						perror("splice SPLICE_F_NONBLOCK");
						return EXIT_FAILURE;
					}
				} else if (nret == 0) {
					isr = true;
					// put a newline at the end of each token stream
					if (write(STDOUT_FILENO, "\n", 1) == -1)
						return EXIT_FAILURE;
					close(infd);
				}
			}
		}
		wait((int*) NULL);
	}
	delete js_extractor;

	return EXIT_SUCCESS;
}
