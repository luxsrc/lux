/*
 * Copyright (C) 2016 Chi-kwan Chan
 * Copyright (C) 2016 Steward Observatory
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
#include <lux/mangle.h>
#include <lux/file.h>
#include <lux/hdf5.h>
#include <lux/io.h>

#define setdns(T) do {                             \
	int i;                                     \
	d  = pgetd((T *)pa);                       \
	ns = malloc(sizeof(hsize_t) * d);          \
	if(ns)                                     \
		for(i = 0; i < d; ++i)             \
			ns[i] = pgetn((T *)pa, i); \
} while(0)

#define check(f, ...) do {	                              \
	herr_t status;                                        \
	status = H5##f(__VA_ARGS__);                          \
	if(status < 0)                                        \
		lux_error(LUX_STRING(H5##f) " failed (%d)\n", \
		          status);                            \
} while(0)

#define safe(f, ...) ({                                       \
	hid_t r;                                              \
	r = H5##f(__VA_ARGS__);                               \
	if(r < 0)                                             \
		lux_error(LUX_STRING(H5##f) " failed (%d)\n", \
		          r);                                 \
	r;                                                    \
})
