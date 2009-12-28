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
#ifndef _LUX_FAILED_H_
#define _LUX_FAILED_H_

#include <errno.h> /* for errno */

#define LUX_FAILURE_MASK ((1U << LUX_FAILURE_BIT) - 1)

#define failed errno

#define FNOLIB (LUX_ELAST+1)
#define FNOSYM (LUX_ELAST+2)
#define F2CONS (LUX_ELAST+3)
#define FNOMOD (LUX_ELAST+4)
#define FAILED LUX_FAILURE_MASK

extern const char *strfailure(int); /* pseudo "standard" function
                                       provided by "sys/string.c" */
#endif /* _LUX_FAILED_H_ */
