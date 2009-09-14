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
#ifndef _LUX_LOG_H_
#define _LUX_LOG_H_

#include <lux/failed.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdio.h>

#define LUX_LOG_SUSPEND    (1U << (LUX_UNSIGNED_BIT - 1))
#define LUX_LOG_FLAGS      (1U << (LUX_UNSIGNED_BIT - 2))
#define LUX_LOG_FATAL      (1U << (LUX_UNSIGNED_BIT - 3))
#define LUX_LOG_STREAM(ID) (1U << (5 + (ID)))
#define LUX_LOG_STDOUT     (1U <<  4)
#define LUX_LOG_STDERR     (1U <<  3)
#define LUX_LOG_AUTOFORMAT (1U <<  2)
#define LUX_LOG_PID        (1U <<  1)
#define LUX_LOG_TIMESTAMP  (1U <<  0)

#define LEVELS_INIT {	                                \
	LUX_LOG_FLAGS | LUX_LOG_STDERR | LUX_LOG_FATAL, \
	0,                                              \
	0,                                              \
	LUX_LOG_FLAGS | LUX_LOG_STDERR,                 \
	3,                                              \
	6,                                              \
	LUX_LOG_FLAGS | LUX_LOG_STDOUT,                 \
	3             | LUX_LOG_SUSPEND,                \
	0}

static unsigned levels[16] = LEVELS_INIT;

static inline void
_vlog(unsigned flags, const char *restrict fmt, va_list ap)
{
	/* TODO: implement LUX_LOG_TIMESTAMP, LUX_LOG_PID, and
	         LUX_LOG_AUTOFORMAT */
	if(flags < 8)
		vsyslog(LOG_LOCAL0 | flags, fmt, ap);
	else {
		int f = failed;

		/* TODO: add a for-loop to output to STREAM(ID) */

		if(flags & LUX_LOG_STDOUT)
			(void)vprintf(fmt, ap);

		if(flags & LUX_LOG_STDERR)
			(void)vfprintf(stderr, fmt, ap);

		failed = f; /* hide possible error emitted by v*printf();
		               TODO: make vlog() more robust */
	}
}

static inline void
vlog(unsigned flags, const char *restrict fmt, va_list ap)
{
	while(flags < LUX_LOG_FLAGS)
		flags = levels[flags]; /* FIXME: check number of levels */

	if(flags & LUX_LOG_SUSPEND)
		return; /* suspended; do nothing */

	_vlog(flags & (LUX_LOG_FATAL-1), fmt, ap);

	if(flags & LUX_LOG_FATAL)
		lux_abort();
}

#endif /* _LUX_LOG_H_ */
