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
#ifndef _LUX_HDF5_H_
#define _LUX_HDF5_H_

#include <lux/file.h>
#include <lux/switch.h>
#include <hdf5.h>

typedef struct LuxShdf5 Lux_hdf5;

struct LuxShdf5 {
	Lux_file super;
	hid_t    fid;
};

static inline hid_t
h5t_from_tc(int tc)
{
	int sz   = (tc >> LUX_CHAR_BIT) / LUX_CHAR_BIT;
	int type = tc & ((1 << LUX_CHAR_BIT) - 1);

	switch(type) {
	case 'i':
		SWITCH {
		CASE(sz == sizeof(char))        return H5T_NATIVE_CHAR;
		CASE(sz == sizeof(short))       return H5T_NATIVE_SHORT;
		CASE(sz == sizeof(int))         return H5T_NATIVE_INT;
		CASE(sz == sizeof(long))        return H5T_NATIVE_LONG;
		CASE(sz == sizeof(long long))   return H5T_NATIVE_LLONG;
		}
	case 'u':
		SWITCH {
		CASE(sz == sizeof(char))        return H5T_NATIVE_UCHAR;
		CASE(sz == sizeof(short))       return H5T_NATIVE_USHORT;
		CASE(sz == sizeof(int))         return H5T_NATIVE_UINT;
		CASE(sz == sizeof(long))        return H5T_NATIVE_ULONG;
		CASE(sz == sizeof(long long))   return H5T_NATIVE_ULLONG;
		}
	case 'f':
		SWITCH {
		CASE(sz == sizeof(float))       return H5T_NATIVE_FLOAT;
		CASE(sz == sizeof(double))      return H5T_NATIVE_DOUBLE;
		CASE(sz == sizeof(long double)) return H5T_NATIVE_LDOUBLE;
		}
	};

	return -1;
}

#endif /* _LUX_HDF5_H_ */
