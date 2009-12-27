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
#ifndef _LUX_LIBUX_H_
#define _LUX_LIBUX_H_

#include <lux/load.h>
#include <lux/timer.h>
#include <lux/vlog.h>

#if HAVE_TIMESTAMP
#define LIBUX_NULL {TIMESTAMP_NULL, VLOG_NULL, LOAD_NULL}
#else
#define LIBUX_NULL {VLOG_NULL, LOAD_NULL}
#endif

struct libux {
#if HAVE_TIMESTAMP
	timestamp t0;
#endif
	struct vlog vlog;
	struct load load;
}; /* TODO: implement mklibux() and rmlibux() */

#endif /* _LUX_LIBUX_H_ */
