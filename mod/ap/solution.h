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

/* Forward declarations */
typedef struct LuxSsolution Lux_solution;
typedef struct LuxStask     Lux_task;

struct LuxSsolution {
	Lux_task *task; /* subclassing by composition */
	struct {
		double add;
		double mul;
		double fma;
		double other;
	} opcnt;      /* floating-point operation counts     */
	double mcost; /* measured  computation cost in ticks */
	double ecost; /* estimated computation cost in ticks */
};

#endif /* _LUX_SOLUTION_H_ */
