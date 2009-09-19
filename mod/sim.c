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
#include <lux/job.h>
#include <lux/lazybuf.h>
#include <stdarg.h> /* for va_list and va_arg() */
#include <string.h> /* for strlen() and memcpy() */

void *
LUXC(va_list ap)
{
	const char *arg = va_arg(ap, const char *);

	if(arg[0] == '/') /* using absolute path */
		return lux_load(arg);
	else {
		void *sim;

		char lazybuf[256] = "sim/", *buf = lazybuf;
		size_t len = strlen(arg);
		buf = (char *)REALLOC(buf, sizeof("sim/") + len);
		if(!buf)
			return NULL; /* no need to FREE(buf);
			                failure code was set by REALLOC() */
		(void)memcpy(buf + sizeof("sim/") - 1, arg, len + 1);
		sim = lux_load(buf); /* chain-loading works because of
		                        htab's array-of-stacks design */
		FREE(buf);
		return sim;
	}
}

void
LUXD(Lux_job *sim)
{
	lux_unload(sim);
}
