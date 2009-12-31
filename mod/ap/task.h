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
#ifndef _LUX_TASK_H_
#define _LUX_TASK_H_

#include <lux/message.h>

/* Forward declaration */
typedef struct LuxStask Lux_task;

struct LuxStask {
	/* Because problem and algo are problem specific, the implementation
	   of exec() below is also problem specific.  */
	Lux_message *(*exec)(Lux_task *, Lux_message **);

	/* Internally, a Lux_task should embed the problem specific
	   plan, params, ro, rw, and wo.  E.g.,

	Lux_plan    plan;
	Lux_params  params;
	const void *input;
	void       *buffer;
	void       *output; */
};

#endif /* _LUX_TASK_H_ */
