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
 * pjscan.cpp
 *  Created on: Apr 27, 2011
 */

#include <cstdlib>
#include <iostream>
#include <climits>	// LONG_MAX, ULONG_MAX
#include <cstring>

#include <argtable2.h>
#include <boost/thread.hpp>
// Must define before including boost/filesystem.hpp, still using version 2
#define BOOST_FILESYSTEM_VERSION 2
#include <boost/filesystem.hpp>
#include <unistd.h>	// getuid(), geteuid()
#include <errno.h>	// errno
#include "Learner.h"
#include "Classifier.h"
#include "JsDumper.h"
#include "FeaturePrinter.h"
#include "timing.h"

using std::cout;
using std::endl;
namespace boost_fs = boost::filesystem;

/*
 * Function that gets called when an unhandled exception occurs.
 */
void myterminate() {
	std::cerr << "Unhandled exception occured" << endl;
	std::exit(EXIT_FAILURE);
}

/*
 * A structure for command-line arguments parsing and storing.
 */
struct Args {
private:
public:
	enum run_mode {
		LEARNING, CLASSIFICATION, JSDUMP, FEATPRINT
	} mode;
	const char *model_file;
	const char *pdf_file;
	const char *js_dump_dir;
	std::vector<std::string> file_names;
	unsigned long N;
	double nu;
	Args() :
		mode(LEARNING), model_file(0), pdf_file(0), js_dump_dir(0),
				file_names(), N(0), nu(0.0) {
	}
	bool isfile(const char *file) const {
		boost_fs::path path(file);
		return boost_fs::exists(path) && boost_fs::is_regular_file(path);
	}
	/*
	 * Returns true if the next argument is a directory with PDF files inside,
	 * otherwise false. Appends all names of found PDF files to args.file_names.
	 */
	bool ispdfdir(const char *dir) {
		boost_fs::path path(dir);
		if (!isdir(dir)) {
			return false;
		} else {
			// PDF directory exists, checking if there are PDF files inside
			for (boost_fs::directory_iterator itr(path); itr
					!= boost_fs::directory_iterator(); ++itr) {
				if ((boost_fs::is_regular_file(itr->path())
						|| boost_fs::is_symlink(itr->path()))
						&& itr->path().extension() == ".pdf")
					file_names.push_back(itr->path().file_string());
			}

			if (file_names.empty())
				return false;
		}
		return true;
	}
	/* Returns true if the next argument is a directory, otherwise false. */
	bool isdir(const char *dir) const {
		boost_fs::path path(dir);
		return boost_fs::exists(path) && boost_fs::is_directory(path);
	}
};

/*
 * Prints an error message, the program usage and exits the program
 */
void error_exit(const char *msg) {
	std::cerr << "ERROR: " << msg << endl;
	std::exit(EXIT_FAILURE);
}

/*
 * Prints an error message
 */
void error_print(const char *msg) {
	std::cerr << "ERROR: " << msg << endl;
}

void print_usage(void **argtable, const char *prog) {
	std::cerr << std::endl << prog;
	arg_print_syntaxv(stderr, argtable, "\n");
	std::cerr << endl;
	arg_print_glossary(stderr, argtable, "%-25s %s\n");
	std::cerr << endl;
}

void parse_arguments(int argc, char *argv[], Args &args) {
	/* SYNTAX 1: learn [--nu=<double>] <model file> <PDF directory> */
	struct arg_rex *cmd1 = arg_rex1(NULL, NULL, "learn", NULL, 0, NULL);
	struct arg_dbl *nu1 = arg_dbl0(NULL, "nu", "<double>",
			"the nu parameter for the One-class SVM (default 0.05)");
	struct arg_file *model1 = arg_file1(NULL, NULL, "<model file>",
			"output model file");
	struct arg_file *pdfdir1 = arg_file1(NULL, NULL, "<PDF directory>",
			"a directory of PDF files to scan");
	struct arg_end *end1 = arg_end(20);
	void* argtable1[] = { cmd1, nu1, model1, pdfdir1, end1 };
	int nerrors1;

	/* SYNTAX 2: classify <model file> <PDF directory> */
	struct arg_rex *cmd2 = arg_rex1(NULL, NULL, "classify", NULL, 0, NULL);
	struct arg_file *model2 = arg_file1(NULL, NULL, "<model file>",
			"input model file");
	struct arg_file *pdfdir2 = arg_file1(NULL, NULL, "<PDF directory>",
			"a directory of PDF files to scan");
	struct arg_end *end2 = arg_end(20);
	void* argtable2[] = { cmd2, model2, pdfdir2, end2 };
	int nerrors2;

	/* SYNTAX 3: dumpjs <PDF file> <PDF directory>*/
	struct arg_rex *cmd3 = arg_rex1(NULL, NULL, "dumpjs", NULL, 0, NULL);
	struct arg_file *pdffile3 = arg_file1(NULL, NULL, "<PDF file>",
			"a PDF file to scan");
	struct arg_file *pdfdir3 = arg_file1(NULL, NULL, "<dump directory>",
			"a directory where the JavaScript strings will be dumped");
	struct arg_end *end3 = arg_end(20);
	void* argtable3[] = { cmd3, pdffile3, pdfdir3, end3 };
	int nerrors3;

	/* SYNTAX 3: printfeats <PDF file> <model file> <feature count> */
	struct arg_rex *cmd4 = arg_rex1(NULL, NULL, "printfeats", NULL, 0, NULL);
	struct arg_file *pdffile4 = arg_file1(NULL, NULL, "<PDF file>",
			"a PDF file to scan");
	struct arg_file *model4 = arg_file1(NULL, NULL, "<model file>",
			"input model file");
	struct arg_int *fc4 = arg_int1(NULL, NULL, "<feature count>",
			"number of features to print");
	struct arg_end *end4 = arg_end(20);
	void* argtable4[] = { cmd4, pdffile4, model4, fc4, end4 };
	int nerrors4;

	/* SYNTAX 4: [-help] [-version] */
	struct arg_lit *help5 = arg_lit0(NULL, "help", "print this help and exit");
	struct arg_end *end5 = arg_end(20);
	void* argtable5[] = { help5, end5 };
	int nerrors5;

	// Check allocation
	if (arg_nullcheck(argtable1) != 0 or arg_nullcheck(argtable2) != 0
			or arg_nullcheck(argtable3) != 0 or arg_nullcheck(argtable4) != 0
			or arg_nullcheck(argtable5) != 0) {
		error_exit("Unable to allocate memory for command-line arguments.");
	}

	// Set default values
	nu1->dval[0] = 0.05;

	// Parse the parameters
	nerrors1 = arg_parse(argc, argv, argtable1);
	nerrors2 = arg_parse(argc, argv, argtable2);
	nerrors3 = arg_parse(argc, argv, argtable3);
	nerrors4 = arg_parse(argc, argv, argtable4);
	nerrors5 = arg_parse(argc, argv, argtable5);

	// Determine which syntax has been used
	if (nerrors1 == 0) {
		args.mode = Args::LEARNING;
		boost_fs::path path(*model1->filename);
		if (boost_fs::exists(path)) {
			error_exit("Model file already exists. Will not overwrite.");
		}
		args.model_file = *model1->filename;
		if (not args.ispdfdir(*pdfdir1->filename)) {
			error_exit("Directory does not exist or has no PDF files.");
		}
		if (nu1->dval[0] < 0.0 or nu1->dval[0] > 1.0) {
			error_exit("The value of the nu parameter must be between 0.0 and 1.0");
		}
		args.nu = nu1->dval[0];
	} else if (nerrors2 == 0) {
		args.mode = Args::CLASSIFICATION;
		if (not args.isfile(*model2->filename)) {
			error_exit("Model file does not exist.");
		}
		args.model_file = *model2->filename;
		if (!args.ispdfdir(*pdfdir2->filename)) {
			error_exit("Directory does not exist or has no PDF files.");
		}
	} else if (nerrors3 == 0) {
		args.mode = Args::JSDUMP;
		if (not args.isfile(*pdffile3->filename)) {
			error_exit("PDF file does not exist.");
		}
		args.pdf_file = *pdffile3->filename;
		if (not args.isdir(*pdfdir3->filename)) {
			error_exit("The specified directory does not exist.");
		}
		args.js_dump_dir = *pdfdir3->filename;
	} else if (nerrors4 == 0) {
		args.mode = Args::FEATPRINT;
		if (not args.isfile(*pdffile4->filename)) {
			error_exit("PDF file does not exist.");
		}
		args.pdf_file = *pdffile4->filename;
		if (not args.isfile(*model4->filename)) {
			error_exit("Model file does not exist.");
		}
		args.model_file = *model4->filename;
		if (fc4->ival[0] <= 0) {
			error_exit("Feature count expected as positive integer.");
		}
		args.N = fc4->ival[0];
	} else if (nerrors5 == 0) {
		std::cerr << "Usage 1: ";
		print_usage(argtable1, argv[0]);
		std::cerr << "Usage 2: ";
		print_usage(argtable2, argv[0]);
		std::cerr << "Usage 3: ";
		print_usage(argtable3, argv[0]);
		std::cerr << "Usage 4: ";
		print_usage(argtable4, argv[0]);
		std::cerr << "Usage 5: ";
		print_usage(argtable5, argv[0]);
		std::exit(EXIT_SUCCESS);
	} else {
		// Handle errors
		if (cmd1->count > 0) {
			arg_print_errors(stderr, end1, argv[0]);
			print_usage(argtable1, argv[0]);
		} else if (cmd2->count > 0) {
			arg_print_errors(stderr, end2, argv[0]);
			print_usage(argtable2, argv[0]);
		} else if (cmd3->count > 0) {
			arg_print_errors(stderr, end3, argv[0]);
			print_usage(argtable3, argv[0]);
		} else if (cmd4->count > 0) {
			arg_print_errors(stderr, end4, argv[0]);
			print_usage(argtable4, argv[0]);
		} else {
			print_usage(argtable5, argv[0]);
		}
		// No need to free resources because we will exit()
		error_exit("Error parsing arguments");
	}

	// Will not free argtables because they are needed
}

/*
 * pjscan - detects malicious PDF files based on the contents of their
 * JavaScript code.
 */
int main(int argc, char *argv[]) {
	// Make sure we don't run as root
	if (getuid() == 0 || geteuid() == 0) {
		error_exit("This program cannot be run as root.");
	}
#ifdef _DO_TIMING
	struct timeval prog_begin, prog_end;
	gettimeofday(&prog_begin, NULL);
#endif // _DO_TIMING
	std::set_terminate(myterminate);
	// Parse input
	Args args;
	try {
		parse_arguments(argc, argv, args);
	} catch (boost_fs::basic_filesystem_error<boost_fs::path> e) {
		error_exit(e.what());
	} catch (...) {
		error_exit("Argument parsing exception");
	}

	switch (args.mode) {
	case Args::LEARNING: {
		// Run the learning algorithm
		Learner *learner = new Learner(args.model_file, args.file_names,
				args.nu);
		learner->learn();
		delete learner;
		break;
	}
	case Args::CLASSIFICATION: {
		// Classify files
		Classifier *classifier = new Classifier(args.model_file,
				args.file_names);
		classifier->classify();
		delete classifier;
		break;
	}
	case Args::JSDUMP: {
		// Dump JavaScript scripts
		JsDumper *dumper = new JsDumper(args.pdf_file, args.js_dump_dir);
		dumper->dump();
		delete dumper;
		break;
	}
	case Args::FEATPRINT: {
		// Print top features
		FeaturePrinter *fp = new FeaturePrinter(args.pdf_file, args.model_file,
				args.N);
		fp->print();
		delete fp;
		break;
	}
	}

	cout << "\nExiting" << endl;
#ifdef _DO_TIMING
	gettimeofday(&prog_end, NULL);
	cout << "Total running time: " << time_diff(prog_begin, prog_end) << endl;
#endif // _DO_TIMING
	return EXIT_SUCCESS;
}
