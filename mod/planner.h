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
#ifndef _LUX_PLANNER_H_
#define _LUX_PLANNER_H_

#include <lux/problem.h>
#include <lux/task.h>

#define LUX_PLAN_DEFAULT    LUX_PLAN_MEASURE

#define LUX_PLAN_ESTIMATE   0U
#define LUX_PLAN_MEASURE    1U
#define LUX_PLAN_PATIENT    2U
#define LUX_PLAN_EXHAUSTIVE 3U

/* Forward declaration */
typedef struct LuxSplanner Lux_planner;

struct LuxSplanner {
        /* A Lux_planner can plan for a particular Lux_problem and
           return a single optimal plan */
	Lux_task *(*plan)(Lux_planner *, Lux_problem *, unsigned);
};

#endif /* _LUX_PLANNER_H_ */
