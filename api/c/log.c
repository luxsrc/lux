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
#include "api.h"
#include <lux/vlog.h>
#include <stdarg.h> /* for va_list, va_start(), and va_end() */

#define DEF_LOG(L, S) void                                      \
	lux_##S(const void *fmt, ...)                           \
	{                                                       \
		va_list ap;                                     \
		va_start(ap, fmt);                              \
		vlog(&LUX->vlog, LUX->vlog.levels[L], fmt, ap); \
		va_end(ap);                                     \
	}

DEF_LOG(0, fatal)
DEF_LOG(3, error)
DEF_LOG(6, print)
DEF_LOG(7, debug)
