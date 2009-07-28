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
#include <lux.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h> /* for vfprintf() */

void
lux_error(const char *restrict fmt, ...)
{
	int n, e = errno;

	va_list ap;
	va_start(ap, fmt);
	n = vfprintf(stderr, fmt, ap);
	va_end(ap);

	if(n < 0)
		errno = e; /* restore e to hide possible error emitted by
		              vfprintf(); TODO: make lux_error() more robust */
}
