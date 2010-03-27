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
#include <lux/mangle.h>
#include <stdlib.h> /* for NULL and malloc() */
#include "mpi.h"

#define EGO ((Lux_mpi *)ego)
#define MPI_INITED_EXT (-1)

static int count = 0;

void *
LUX_MKMOD(const void *opts)
{
	void *ego;

	if(count == 0) {
		int inited;
		(void)MPI_Initialized(&inited); /* TODO: check MPI error */
		if(inited)
			count = MPI_INITED_EXT;
	}
	if(count != MPI_INITED_EXT) {
		if(count == 0)
			(void)MPI_Init(NULL, NULL);
		++count;
	}

	ego = malloc(sizeof(Lux_mpi));
	if(!ego)
		goto cleanup;
	(void)MPI_Comm_dup(MPI_COMM_WORLD, &EGO->super);

	return ego;

 cleanup:
	return NULL;

	(void)opts; /* silence unused variable warning */
}

void
LUX_RMMOD(void *ego)
{
	(void)MPI_Comm_free(&EGO->super); /* TODO: check MPI error */
	free(ego);

	if(count != MPI_INITED_EXT) {
		--count;
		if(count == 0)
			(void)MPI_Finalize();
	}
}
