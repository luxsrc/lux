/*
 * Copyright (C) 2010 Chi-kwan Chan
 * Copyright (C) 2010 Harvard-Smithsonian Center for Astrophysics
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
#ifndef LUX_ASSERTION
#define LUX_ASSERTION 1
#endif

#include <lux.h>
#include <lux/assert.h>
#include <lux/lazybuf.h>
#include <string.h>

int
main()
{
	char  lazybuf[256] = "";
	char *buf = lazybuf;

	buf = strcpy(lzmalloc(64), "lux");
	lux_assert(buf == lazybuf && !strcmp(buf, "lux"));

	buf = lzrealloc(buf, 128);
	lux_assert(buf == lazybuf && !strcmp(buf, "lux"));

	buf = lzrealloc(buf, 256);
	lux_assert(buf == lazybuf && !strcmp(buf, "lux"));

	buf = lzrealloc(buf, 512);
	lux_assert(buf != lazybuf && !strcmp(buf, "lux"));

	buf = lzrealloc(buf, 1024);
	lux_assert(buf != lazybuf && !strcmp(buf, "lux"));

	lzfree(buf);
	return 0;
}