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
#include <lux/planner.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	Lux_planner *p;
	Lux_task    *t;

	lux_setup(&argc, &argv);

	lux_print("1. Load solvers from \"planner_demo\" into planner... ");
	p = lux_load("../mod/planner/.libs/planner", "planner_demo");
	lux_print("%p DONE\n", p);

	lux_print("2. Plan an algorithm... ");
	t = p->plan(p, NULL, LUX_PLAN_EXHAUSTIVE);
	lux_print("DONE\n");

	lux_print("3. Unload the planner... ");
	lux_unload(p);
	lux_print("DONE\n");

	return 0;
}
