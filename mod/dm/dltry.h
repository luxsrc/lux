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

#include <lux/failed.h>
#include <dlfcn.h> /* for dlerror(), dlopen(), and dlsym() */

static inline void *
dltryopen(const char *file, int mode)
{
	int   fsv = failed;
	void *hdl = dlopen(file, mode);
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
