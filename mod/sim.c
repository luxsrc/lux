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
#include <lux/sim.h>
#include <string.h> /* for strcat(), strcpy(), and strlen() */

void *
LUXC(va_list ap)
{
	void  *sim;

	int    argc = va_arg(ap, int);
	char **argv = va_arg(ap, char **);

	char lazybuf[256], *buf;
	buf = (char *)MALLOC(sizeof("sim/") + strlen(argv[0]));
	if(!buf)
		return NULL;

	(void)strcat(strcpy(buf, "sim/"), argv[0]);
	sim = lux_load(buf, argc, argv); /* "chain-loading" works as expected
	                                    because of the array-of-stacks
	                                    design of the hash table.  */
	FREE(buf);
	return sim;
}

void
LUXD(Lux_sim *sim)
{
	lux_unload(sim);
}
