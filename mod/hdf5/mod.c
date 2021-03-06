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

static inline void *
getpa(int nbits, hid_t dims)
{
	void *pa;

	size_t   d  = H5Sget_simple_extent_ndims(dims);
	hsize_t *ns = malloc(sizeof(hsize_t) * d);
	size_t  *Ns = malloc(sizeof(size_t)  * d);

	(void)H5Sget_simple_extent_dims(dims, ns, NULL);
	do {
		size_t i;
		for(i = 0; i < d; ++i)
			Ns[i] = ns[i];
	} while(0);

	switch(nbits) {
	case  8: pa = pallocdn(int8_t,  d, Ns); break;
	case 16: pa = pallocdn(int16_t, d, Ns); break;
	case 32: pa = pallocdn(int32_t, d, Ns); break;
	case 64: pa = pallocdn(int64_t, d, Ns); break;
	default: pa = NULL;                     break;
	}

	free(Ns);
	free(ns);

	return pa;
}

static void
close(Lux_file *ego)
{
	(void)H5Pclose(EGO->lcpl);
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
	                 EGO->lcpl, H5P_DEFAULT, H5P_DEFAULT);

	(void)H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, pa);

	(void)H5Dclose(dset);
	(void)H5Sclose(dims);
}

static void *
read_pa(Lux_file *ego, const char *key)
{
	hid_t dset, dims, type;
	void *pa;

	dset = H5Dopen(EGO->fid, key, H5P_DEFAULT);
	dims = H5Dget_space(dset);
	type = H5Dget_type(dset);

	pa = getpa(H5Tget_size(type) * LUX_CHAR_BIT, dims);
	(void)H5Dread(dset, type, dims, dims, H5P_DEFAULT, pa);

	(void)H5Tclose(type);
	(void)H5Sclose(dims);
	(void)H5Dclose(dset);

	return pa;
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

	EGO->lcpl = H5Pcreate(H5P_LINK_CREATE);
	H5Pset_create_intermediate_group(EGO->lcpl, 1);

	ego->close    = close;
	ego->write_pa = write_pa;
	ego->read_pa  = read_pa;
	return ego;

cleanup2:
	free(ego);
cleanup1:
	return NULL;
}
