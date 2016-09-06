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
#include <lux/tpool.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define A(E) lux_always_assert(E)

struct task {
	Lux_task super;
	int id;
};

void
exec(Lux_task *t)
{
	lux_print("[%" PRIxPTR "] %d\n",
	          (uintptr_t)pthread_self(), ((struct task *)t)->id);
	usleep(1);
}

int
main(int argc, char *argv[])
{
	struct tpool *q;
	struct task  *t;
	int i, n;

	lux_setup(&argc, &argv);

	n = 8;
	q = mktpool(4);
	t = malloc(n * sizeof(struct task));
	for(i = 0; i < n; ++i) {
		t[i].super.exec = exec;
		t[i].id         = i + 1;
	}

	for(i = 0; i < n; ++i)
		tpool_enqueue(q, &t[i].super);

	tpool_wait(q);

	for(i = 0; i < n; ++i)
		tpool_enqueue(q, &t[i].super);

	rmtpool(q);
	free(t);

	/* TODO: use A() to check for errors at runtime */

	return 0;
}
