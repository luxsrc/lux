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
#ifndef _LUX_DLTRY_H_
#define _LUX_DLTRY_H_

#include <lux/libux.h>
#include <lux/dlfcn.h> /* for dlerror(), dlmopen(), and dlsym() */
#include <lux/failed.h>

static inline void *
dltryopen(struct libux *lux, const char *file)
{
	int   fsv = failed;
	void *hdl = dlmopen(lux->namespace, file, lux->dlmode);
	if(!hdl) {
		(void)dlerror(); /* clear error message */
		failed = fsv;    /* restore previous failure code */
	}
	return hdl;
}

static inline void *
dltrysym(void *restrict hdl, const char *restrict name)
{
	int   fsv = failed;
	void *sym = dlsym(hdl, name);
	if(!sym) {
		(void)dlerror(); /* clear error message */
		failed = fsv;    /* restore previous failure code */
	}
	return sym;
}

#endif /* _LUX_DLTRY_H_ */
