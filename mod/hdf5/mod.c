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
#include <lux/parray.h>
#include <lux/switch.h>
#include <stdlib.h> /* for malloc(), free(), and NULL */
#include <stdint.h> /* for int8_t, int16_t, ... */

#define EGO ((Lux_hdf5 *)ego)

#define set_d_ns(T) do {                   \
	int i;                             \
	d  = pgetd((T *)pa);               \
	ns = malloc(sizeof(hsize_t) * d);  \
	for(i = 0; i < d; ++i)             \
		ns[i] = pgetn((T *)pa, i); \
} while(0)

static inline hid_t
getdims(int tc, const void *pa)
{
	hid_t dims;

	int      d;
	hsize_t *ns;
	switch(tc >> LUX_CHAR_BIT) {
	case  8: set_d_ns(int8_t);  break;
	case 16: set_d_ns(int16_t); break;
	case 32: set_d_ns(int32_t); break;
	case 64: set_d_ns(int64_t); break;
	default: return -1;         break;
	}
	dims = H5Screate_simple(d, ns, NULL);
	free(ns);

	return dims;
}

static void
close(Lux_file *ego)
{
	(void)H5Fclose(EGO->fid);
	free(ego);
}

static void
write_pa(Lux_file *ego, const char *key, int tc, const void *pa)
{
	hid_t type, dims, dset;

	type = h5t_from_tc(tc);
	dims = getdims(tc, pa);
	dset = H5Dcreate(EGO->fid, key, type, dims,
	                 H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	(void)H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, pa);

	(void)H5Dclose(dset);
	(void)H5Sclose(dims);
}

Lux_file *
LUX_MOD(const char *fname, unsigned flags)
{
	Lux_file *ego = NULL;

	ego = (Lux_file *)malloc(sizeof(Lux_hdf5));
	if(!ego)
		goto cleanup1;

	SWITCH {
	CASE(flags == H5F_ACC_EXCL || flags == H5F_ACC_TRUNC)
		EGO->fid = H5Fcreate(fname, flags, H5P_DEFAULT, H5P_DEFAULT);
	CASE(flags == H5F_ACC_RDONLY || flags == H5F_ACC_RDWR)
		EGO->fid = H5Fopen(fname, flags, H5P_DEFAULT);
	}
	if(EGO->fid < 0)
		goto cleanup2;

	ego->close    = close;
	ego->write_pa = write_pa;
	return ego;

cleanup2:
	free(ego);
cleanup1:
	return NULL;
}
