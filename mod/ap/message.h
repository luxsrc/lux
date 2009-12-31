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
#ifndef _LUX_MESSAGE_H_
#define _LUX_MESSAGE_H_

#include <lux/atomic.h>

typedef struct LuxSmessage Lux_message;

struct LuxSmessage {
	atomic status;
};

#define LUX_MESSAGE_INIT {ATOMIC_NULL}

#define LUX_MESSAGE_RUNNING (1 << 0)
#define LUX_MESSAGE_DONE    (1 << 1)

#endif /* _LUX_MESSAGE_H_ */
