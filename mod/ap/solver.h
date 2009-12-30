/*
 * Copyright (C) 2009 Chi-kwan Chan
 * Copyright (C) 2009 Harvard-Smithsonian Center for Astrophysics
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
#ifndef _LUX_SOLVER_H_
#define _LUX_SOLVER_H_

#include <lux/ap/problem.h>
#include <lux/ap/task.h>

/* Forward declaration */
typedef struct LuxSsolver Lux_solver;

struct LuxSsolver {
	/* A Lux_solver can solve a particular Lux_problem and return
	   a <lux/vector.h> of pointers to Lux_task */
	Lux_task **(*solve)(Lux_solver *, Lux_problem *);
};

#endif /* _LUX_SOLVER_H_ */
