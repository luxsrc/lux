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
#include <string.h> /* for strlen() and memcpy() */

void *
LUXC(va_list ap)
{
	void *sim;

	const char *arg = va_arg(ap, const char *);
	size_t      len = strlen(arg);

	char lazybuf[256] = "sim/", *buf = lazybuf;
	buf = (char *)REALLOC(buf, sizeof("sim/") + len);
	if(!buf)
		return NULL;

	(void)memcpy(buf + sizeof("sim/") - 1, arg, len + 1);
	sim = lux_load(buf); /* chain-loading works as expected because of the
	                        array-of-stacks design of the hash table. */
	FREE(buf);
	return sim;
}

void
LUXD(Lux_job *sim)
{
	lux_unload(sim);
}
