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
#include <lux/lazybuf.h>
#include <lux/mangle.h>
#include <string.h> /* for strlen() and memcpy() */
#include "../job.h"

void *
LUX_MKMOD(const void *opts)
{
	if(((const char *)opts)[0] == '/') /* using absolute path */
		return lux_load(opts, NULL);
	else {
		void *sim;

		char lazybuf[256] = "sim/", *buf = lazybuf;
		size_t len = strlen(opts);
		buf = (char *)lzrealloc(buf, sizeof("sim/") + len);
		if(!buf)
			return NULL; /* no need to lzfree(buf);
			                failure code was set by lzrealloc() */
		(void)memcpy(buf + sizeof("sim/") - 1, opts, len + 1);
		sim = lux_load(buf, NULL); /* chain-loading works because of
		                              htab's array-of-stacks design */
		lzfree(buf);
		return sim;
	}
}

void
LUX_RMMOD(void *ego)
{
	lux_unload(ego);
}
