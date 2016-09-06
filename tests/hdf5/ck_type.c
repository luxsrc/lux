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
#include <lux/assert.h>
#include <lux/hdf5.h>

#define A(E) lux_always_assert(E)

int
main(int argc, char *argv[])
{
	int char_ct     = TYPECODE('i', sizeof(char)      * LUX_CHAR_BIT);
	int short_ct    = TYPECODE('i', sizeof(short)     * LUX_CHAR_BIT);
	int int_ct      = TYPECODE('i', sizeof(int)       * LUX_CHAR_BIT);
	int long_ct     = TYPECODE('i', sizeof(long)      * LUX_CHAR_BIT);
	int llong_ct    = TYPECODE('i', sizeof(long long) * LUX_CHAR_BIT);

	int uchar_ct    = TYPECODE('u', sizeof(unsigned char)      * LUX_CHAR_BIT);
	int ushort_ct   = TYPECODE('u', sizeof(unsigned short)     * LUX_CHAR_BIT);
	int unsigned_ct = TYPECODE('u', sizeof(unsigned)           * LUX_CHAR_BIT);
	int ulong_ct    = TYPECODE('u', sizeof(unsigned long)      * LUX_CHAR_BIT);
	int ullong_ct   = TYPECODE('u', sizeof(unsigned long long) * LUX_CHAR_BIT);

	int float_ct    = TYPECODE('f', sizeof(float)       * LUX_CHAR_BIT);
	int double_ct   = TYPECODE('f', sizeof(double)      * LUX_CHAR_BIT);
	int ldouble_ct  = TYPECODE('f', sizeof(long double) * LUX_CHAR_BIT);

	lux_setup(&argc, &argv);

	A(h5t_from_tc(char_ct)  == H5T_NATIVE_CHAR);
	A(h5t_from_tc(uchar_ct) == H5T_NATIVE_UCHAR);

	if(sizeof(short) > sizeof(char)) {
		A(h5t_from_tc(short_ct)  == H5T_NATIVE_SHORT);
		A(h5t_from_tc(ushort_ct) == H5T_NATIVE_USHORT);
	}

	if(sizeof(int) > sizeof(short)) {
		A(h5t_from_tc(int_ct)      == H5T_NATIVE_INT);
		A(h5t_from_tc(unsigned_ct) == H5T_NATIVE_UINT);
	}

	if(sizeof(long) > sizeof(int)) {
		A(h5t_from_tc(long_ct)  == H5T_NATIVE_LONG);
		A(h5t_from_tc(ulong_ct) == H5T_NATIVE_ULONG);
	}

	if(sizeof(long long) > sizeof(long)) {
		A(h5t_from_tc(llong_ct)  == H5T_NATIVE_LLONG);
		A(h5t_from_tc(ullong_ct) == H5T_NATIVE_ULLONG);
	}

	A(h5t_from_tc(float_ct)  == H5T_NATIVE_FLOAT);
	A(h5t_from_tc(double_ct) == H5T_NATIVE_DOUBLE);
	if(sizeof(long double) > sizeof(double)) {
		A(h5t_from_tc(ldouble_ct) == H5T_NATIVE_LDOUBLE);
	}

	return 0;
}
