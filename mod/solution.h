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
#ifndef _LUX_SOLUTION_H_
#define _LUX_SOLUTION_H_

#include <lux/estimate.h>
#include <lux/measure.h>
#include <lux/task.h>
#include <lux/zalloc.h>

/* Forward declaration */
typedef struct LuxSsolution Lux_solution;

struct LuxSsolution {
	Lux_task    *task;
	struct opcnt opcnt; /* floating-point operation counts     */
	double       ecost; /* estimated computation cost in ticks */
	struct mcost mcost; /* measured computation cost in ticks  */
};

static inline Lux_solution *
mksolution(Lux_task *t, double add, double mul, double fma, double other)
{
	Lux_solution *s = zalloc(sizeof(Lux_solution));
	s->task = t;
	s->opcnt.add   = add;
	s->opcnt.mul   = mul;
	s->opcnt.fma   = fma;
	s->opcnt.other = other;
	return s;
}

#endif /* _LUX_SOLUTION_H_ */
