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
#include <string.h> /* for strerror() */

const char *
strfailure(int f)
{
	f &= FAILED; /* apply failure code mask---see defintion of
	                FAILED in <lux/failed.h> */
	switch(f) {
	case FNOLIB:
		return "Invalid library";
	case FNOSYM:
		return "Invalid symbol";
	case F2CONS:
		return "lux module construction failed";
	case FNOMOD:
		return "Invalid module";
	case FAILED:
		return "Generic failure";
	default:
		return strerror(f);
	}
}
