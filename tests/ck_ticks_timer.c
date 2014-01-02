/*
 * Copyright (C) 2014 Chi-kwan Chan
 * Copyright (C) 2014 Steward Observatory
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
#include <lux/ticks.h>
#include <lux/timer.h>
#include <stdio.h>
#include <unistd.h>

#define A(E) lux_always_assert(E)

int
main()
{
	timestamp_t T0, T1;
	ticks_t     t0, t1;
	double      dt, dT;

	t0 = getticks();
	T0 = gettimestamp();
	usleep(1000);
	T1 = gettimestamp();
	t1 = getticks();

	dT = elapsed_time(T1, T0);
	dt = elapsed(t1, t0);

	printf("dtimestamps: %g\n", dT);
	printf("dticks     : %g\n", dt);
	printf("ticks/sec  : %g\n", dt/dT);

	/* TODO: use A() to check for errors at runtime */

	return 0;
}
