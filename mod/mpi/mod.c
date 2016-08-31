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
#include <lux/mutex.h>
#include <stdlib.h> /* for NULL and malloc() */
#include "../mpi.h"

#define EGO ((Lux_mpi *)ego)
#define MPI_INITED_EXT (-1)

/* It is non-trivial to allow multiple threads to load the MPI module
 * simutaniously.  We need to consider two situations here:
 *
 * 1) If MPI_Init() was called outside this module, then all threads
 *    will obtain inited == 1.  At the same time, count can be 0 or
 *    -1, depending on the thread.
 *
 * 2) If MPI_Init() was *not* called outside this module, then the
 *    first executed thread will obtain inited == 0.  However, other
 *    threads, depending on the execusion order, may obtain inited ==
 *    0 or 1.
 *
 * Writing thread safe code using atomic operation here is tricky.
 * Given that loading and unloading MPI module are not performance
 * critical, we use mutex lock here to keep things simple.  */
static mutex_t lock  = MUTEX_NULL;
static int     count = 0;

void *
LUX_MKMOD(const void *opts)
{
	void *ego;
	int   thread_mode;

	mutex_lock(&lock);
	if(count == 0) {
		int inited;
		(void)MPI_Initialized(&inited); /* TODO: check MPI error */
		if(inited)
			count = MPI_INITED_EXT;
	}
	if(count != MPI_INITED_EXT) {
		if(count == 0)
			(void)MPI_Init_thread(NULL, NULL,
			                      MPI_THREAD_MULTIPLE,
			                      &thread_mode);
		++count;
	}
	mutex_unlock(&lock);

	ego = malloc(sizeof(Lux_mpi));
	if(!ego)
		goto cleanup;
	(void)MPI_Comm_dup(MPI_COMM_WORLD, &EGO->super);
	EGO->thread_mode = thread_mode;

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

	mutex_lock(&lock);
	if(count != MPI_INITED_EXT) {
		--count;
		if(count == 0)
			(void)MPI_Finalize();
	}
	mutex_unlock(&lock);
}
