/*
 * Copyright (C) 2009 Chi-kwan Chan
 * Copyright (C) 2009 Harvard-Smithsonian Center for Astrophysics
 *
 * This file is part of lux.
 *
 * lux is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lux is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lux.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _LUX_MEASURE_H_
#define _LUX_MEASURE_H_

#include <lux/task.h>
#include <lux/ticks.h>
#include <lux/timer.h>
#include <math.h>

#if !HAVE_TICK_COUNTER
# error high resolution tick counter is not available on this platform
#else
# ifdef TIME_MIN
#  define TICK_MIN TIME_MIN /* defined in <lux/ticks.h> in ticks */
# else
#  define TICK_MIN 100.0 /* from "fftw3/kernel/timer.c" */
# endif
#endif

#if !HAVE_TIMESTAMP
# error second-based timestamp is not available on this platform
#else
# define TIME_LIM 2.0 /* from "fftw3/kernel/timer.c" */
# define N_REPEAT 8   /* from "fftw3/kernel/timer.c" */
#endif

static inline double
measure_ave(Lux_task *task, unsigned n)
{
	ticks_t t0, t1;
	unsigned i;

	t0 = getticks();
	for(i = 0; i < n; ++i)
		task->exec(task);
	t1 = getticks();

	return elapsed(t1, t0) / n;
}

static inline double
measure(Lux_task *task)
{
	double dt_min = HUGE_VAL;

	unsigned n;
	for(n = 1; n; n *= 2) {
		timestamp_t t0 = gettimestamp();

		unsigned i;
		for(i = 0; i < N_REPEAT; ++i) {
			double      dt = measure_ave(task, n);
			timestamp_t t1 = gettimestamp();
			if(dt < 0) /* unlikely: tick counter wraps around */
				break;
			if(dt_min > dt)
				dt_min = dt;
			if(elapsed_time(t1, t0) > TIME_LIM)
				break;
		}

		/* dt_min could only be HUGE_VAL here if n == 1 and the tick
		   counter continuously wraps around for N_REPEAT times.  In
		   such a case, we (and probably the OS) are totally messed
		   up anyway.  It's fine to break and then return HUGE_VAL. */
		if(dt_min * n > TICK_MIN)
			break;
	}

	return dt_min;
}

#endif /* _LUX_MEASURE_H_ */
