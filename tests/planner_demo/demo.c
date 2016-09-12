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
#include <lux/pvector.h>
#include <lux/solver.h>

static inline int
exec(Lux_task *t)
{
	lux_print("executing %p\n", t);
	return 0;
}

Lux_solution **
LUX_MOD(const void *prob, unsigned flags)
{
	Lux_task *t = malloc(sizeof(Lux_task));
	t->exec = exec;

	return pvector(
		Lux_solution *,
		mksolution(t, 0, 0, 0, 0));

	(void)prob;  /* silence unused variable warning */
	(void)flags; /* silence unused variable warning */
}
