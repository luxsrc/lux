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
#include <lux/io.h>
#include <lux/file.h>
#include <lux/hdf5.h>
#include <stdlib.h>

#define A(E) lux_always_assert(E)

int
main(int argc, char *argv[])
{
	Lux_io   *hdf5;
	Lux_file *file;

	lux_setup(&argc, &argv);

	hdf5 = lux_load("../../mod/hdf5/.libs/hdf5", NULL);
	A(hdf5);

	file = hdf5("test.h5", H5F_ACC_TRUNC);
	A(file);

	lux_print("fid: %d\n", ((Lux_hdf5 *)file)->fid);

	file->close(file);
	lux_unload(hdf5);

	return 0;
}
