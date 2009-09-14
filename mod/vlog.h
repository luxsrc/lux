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
#ifndef _LUX_VLOG_H_
#define _LUX_VLOG_H_

#include <lux/failed.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdio.h>

#define VLOG_SUSPEND    (1U << (LUX_UNSIGNED_BIT - 1))
#define VLOG_FLAGS      (1U << (LUX_UNSIGNED_BIT - 2))
#define VLOG_FATAL      (1U << (LUX_UNSIGNED_BIT - 3))
#define VLOG_STREAM(ID) (1U << (5 + (ID)))
#define VLOG_STDOUT     (1U <<  4)
#define VLOG_STDERR     (1U <<  3)
#define VLOG_AUTOFORMAT (1U <<  2)
#define VLOG_PID        (1U <<  1)
#define VLOG_TIMESTAMP  (1U <<  0)

#define VLOG_EMERG (VLOG_FLAGS | VLOG_STDERR | VLOG_FATAL)

#define VLOG_COUNT 16
#define VLOG_INIT {VLOG_COUNT, {   \
	VLOG_EMERG,                \
	0,                         \
	0,                         \
	VLOG_FLAGS | VLOG_STDERR,  \
	3,                         \
	6,                         \
	VLOG_FLAGS | VLOG_STDOUT,  \
	3          | VLOG_SUSPEND, \
	0}}

struct vlog {
	size_t   count;              /* number of levels */
	unsigned levels[VLOG_COUNT]; /* flexible array member */
};

static inline void
hlog(unsigned flags, const char *restrict fmt, va_list ap)
{
	/* TODO: implement VLOG_TIMESTAMP, VLOG_PID, and VLOG_AUTOFORMAT */

	if(flags < 8)
		vsyslog(LOG_LOCAL0 | flags, fmt, ap);
	else {
		int f = failed;

		/* TODO: add a for-loop to output to STREAM(ID) */

		if(flags & VLOG_STDOUT)
			(void)vprintf(fmt, ap);

		if(flags & VLOG_STDERR)
			(void)vfprintf(stderr, fmt, ap);

		failed = f; /* hide possible error emitted by v*printf();
		               TODO: make vlog() more robust */
	}
}

static inline void
vlog(struct vlog *l, unsigned flags, const char *restrict fmt, va_list ap)
{
	while(flags < VLOG_FLAGS)
		flags = flags < l->count ? l->levels[flags] : VLOG_EMERG;

	if(flags & VLOG_SUSPEND)
		return; /* suspended; do nothing */

	hlog(flags & (VLOG_FATAL-1), fmt, ap);

	if(flags & VLOG_FATAL)
		lux_abort();
}

#endif /* _LUX_VLOG_H_ */
