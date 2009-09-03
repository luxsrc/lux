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
#ifndef _LUX_TIMER_H_
#define _LUX_TIMER_H_

#if HAVE_SYS_TIME_H && (HAVE_GETTIMEOFDAY || HAVE_BSDGETTIMEOFDAY)
#include <sys/time.h>

typedef struct timeval timestamp;

static inline timestamp
gettimestamp(void)
{
	timestamp t;
#if HAVE_GETTIMEOFDAY
	(void)gettimeofday(&t, NULL);
#else
	(void)BSDgettimeofday(&t, NULL);
#endif
	return t;
}

static inline double
elapsed_time(timestamp t1, timestamp t0)
{
	return ((double)(t1.tv_sec  - t0.tv_sec ) +
	        (double)(t1.tv_usec - t0.tv_usec) * 1.0e-6);
}

static inline double
elapsed_since(timestamp t0)
{
	timestamp t1 = gettimestamp();
	return elapsed_time(t1, t0);
}

#define HAVE_TIMESTAMP 1
#endif

#endif /* _LUX_TIMER_H_ */
