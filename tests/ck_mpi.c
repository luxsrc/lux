/*
 * Copyright (C) 2010 Chi-kwan Chan
 * Copyright (C) 2010 Harvard-Smithsonian Center for Astrophysics
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
#include <lux/mpi.h>
#include <stdio.h>

#define A(E) lux_always_assert(E)

int
main(int argc, char *argv[])
{
	Lux_mpi *mpi;

	lux_setup(&argc, &argv);

	mpi = lux_load("../mod/.libs/mpi", NULL);
	A(mpi);

	switch(mpi->thread_mode) {
	case MPI_THREAD_SINGLE:     lux_print("single\n");     break;
	case MPI_THREAD_FUNNELED:   lux_print("funneled\n");   break;
	case MPI_THREAD_SERIALIZED: lux_print("serialized\n"); break;
	case MPI_THREAD_MULTIPLE:   lux_print("multiple\n");   break;
	}

	lux_unload(mpi);

	return 0;
}
