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
 *  timing.h
 *  Created on: Jun 2, 2011
 */

#ifndef TIMING_H_
#define TIMING_H_

#include <sys/time.h>

/*
 * If defined, _DO_TIMING enables performance measurement.
 */
//#define _DO_TIMING

/*
 * Executes the given statement. If timing is on, also measures the time it
 * takes for the statement to execute and logs it to an output file.
 */
#ifdef _DO_TIMING
#define TIME_STATEMENT(STATEMENT, OFSTREAM) \
{ \
	struct timeval time_begin, time_end; \
	int tod = gettimeofday(&time_begin, NULL); \
	STATEMENT; \
	if (tod != -1) {\
		gettimeofday(&time_end, NULL); \
		OFSTREAM << '\t' << time_diff(time_begin, time_end); \
	} \
}
#else
#define TIME_STATEMENT(STATEMENT, OFSTREAM) \
	STATEMENT;
#endif // _DO_TIMING

/*
 * Returns the difference between two moments in time as the number of seconds
 * elapsed.
 */
double time_diff(const struct timeval &begin, const struct timeval &end);

#endif /* TIMING_H_ */
