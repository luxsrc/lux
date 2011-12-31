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
#ifndef _LUX_RESMAN_H_
#define _LUX_RESMAN_H_

#include <lux/ap/message.h>
#include <lux/ap/task.h>
#if HAVE_STDDEF_H
#include <stddef.h> /* for size_t */
#else
#include <stdlib.h> /* for size_t */
#endif

typedef struct LuxSresman Lux_resman;

struct LuxSresman {
	void *(*mk)(Lux_resman *, size_t, int);
	void  (*rm)(Lux_resman *, void *);

	Lux_message *(*submit)(Lux_resman *, Lux_task *, Lux_message **);
	void (*wait)(Lux_resman *, Lux_message **);
};

#define LUX_RESMAN_INIT {NULL, NULL, NULL, NULL}

#endif /* _LUX_RESMAN_H_ */
