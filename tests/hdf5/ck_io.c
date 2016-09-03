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
#include <lux/parray.h>
#include <stdlib.h>
#include <stdio.h>

#define A(E) lux_always_assert(E)

#define TESTFNAME "test.h5"

static void
cleanup(void)
{
	(void)remove(TESTFNAME);
}

int
main(int argc, char *argv[])
{
	Lux_io   *hdf5;
	Lux_file *file;

	double *out, *in;
	int     i;

	atexit(cleanup); /* in case some of the tests fail */
	lux_setup(&argc, &argv);

	hdf5 = lux_load("../../mod/hdf5/.libs/hdf5", NULL);
	A(hdf5);

	/* Test write to new file */
	out = palloc(double, 11, 13, 17, 19);
	A(out);

	for(i = 0; i < 11 * 13 * 17 * 19; ++i)
		out[i] = (double)rand() / RAND_MAX;

	file = hdf5(TESTFNAME, H5F_ACC_EXCL);
	A(file);

	file->write_pa(file, "/test", typecodeof(out[0]), out);
	file->close(file);

	/* Test read existing file */
	file = hdf5(TESTFNAME, H5F_ACC_RDONLY);
	A(file);

	in = file->read_pa(file, "/test");
	A(in);

	file->close(file);

	for(i = 0; i < 11 * 13 * 17 * 19; ++i)
		A(in[i] == out[i]);

	pfree(in);
	pfree(out);

	cleanup();

	lux_unload(hdf5);

	return 0;
}
