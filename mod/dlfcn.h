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
#ifndef _LUX_DLFCN_H_
#define _LUX_DLFCN_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#define _GNU_SOURCE_BY_ME
#endif
#include <dlfcn.h>
#ifdef  _GNU_SOURCE_BY_ME
#undef  _GNU_SOURCE_BY_ME
#undef  _GNU_SOURCE
#endif

#ifndef HAVE_DLMOPEN /* define our own dlmopen() */
typedef long int Lmid_t;
#define LM_ID_BASE 0 /* initial namespace */
extern char *dlmopen(Lmid_t, const char *, int);
#endif

extern const char  *dlfname (void *);
extern       Lmid_t dllmid  (void *);
extern       void  *dlhandle(Lmid_t, void *);

#endif /* _LUX_DLFCN_H_ */
