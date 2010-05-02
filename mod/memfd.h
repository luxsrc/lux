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
#ifndef _LUX_MEMFD_H_
#define _LUX_MEMFD_H_

#if HAVE_SYS_MMAN_H
#define _GNU_SOURCE
#include <sys/mman.h> /* needed even if memfd_create() is not available */
#else
#error <sys/mman.h> not found
#endif

#if HAVE_MEMFD_CREATE
/* Use the system memfd_create() in <sys/mman.h>; declare nothing */
#else
/* Declare memfd_create() and related types */
int memfd_create(const char *, unsigned);
#endif

#endif /* _LUX_MEMFD_H_ */
