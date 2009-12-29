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
#ifndef _LUX_XOF_H_
#define _LUX_XOF_H_

#if HAVE_STDDEF_H
#include <stddef.h>
#else
#define offsetof(T, F) ((size_t)&((T *)0)->F)
#endif

#define headof(T, P, F) ((T *)((char *)(P) - offsetof(T, F)))
#define lengthof(T, P) (headof(T, P, v)->n)

#endif /* _LUX_XOF_H_ */
