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
#include <lux.h>
#include <lux/failed.h>
#include <dlfcn.h>  /* for dlerror() */
#include <string.h> /* for strerror() */

#define FAILURE_MASK (~0U >> (LUX_INT_BIT-LUX_FAILURE_BIT))

const char *
strfailure(int f)
{
	f &= FAILURE_MASK;

	if(f <= LUX_ELAST)
		return strerror(f);
	else if(f <= FNOSYM)
		return dlerror(); /* FIXME: dlerror() cleans up error; how to
		                     ensure multiple calls to strfailure()
		                     prodcue the same result? */
	else if(f == F2CONS)
		return "Module construction failed";
	else
		return strerror(f);
}
