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
#include <lux.h>
#include <lux/memfd.h>

#if HAVE_MEMFD_CREATE
/* Use the system memfd_create() in <sys/mman.h>; declare nothing */
#else
/* Implement memfd_create() using shm_open() */
#include <lux/assert.h>
#include <stdio.h>

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

int
memfd_create(const char *name, unsigned flags)
{
	int  fd;
	char uniq[64];

	lux_assert(flags); /* do not accept any flag */

	(void)snprintf(uniq, sizeof(uniq), "/lux-%s", name);
	fd = shm_open(uniq, O_RDWR|O_CREAT|O_EXCL|O_NOFOLLOW,
	              S_IRUSR|S_IWUSR);
	if(fd != -1)
		(void)shm_unlink(uniq);

	return fd;

	(void)flags; /* silence unused parameter warning */
}

#endif
