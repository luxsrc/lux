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
#include <lux/switch.h>
#include <stdlib.h> /* for malloc(), free(), and NULL */

#define EGO ((Lux_hdf5 *)ego)

static void
close(Lux_file *ego)
{
	(void)H5Fclose(EGO->fid);
	free(ego);
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

	ego->close = close;
	return ego;

cleanup2:
	free(ego);
cleanup1:
	return NULL;
}
