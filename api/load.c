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
#include "api.h"
#include <lux/dm/load.h>
#include <stdarg.h> /* for va_list, va_start(), and va_end() */

void *
lux_load(const void *name, ...)
{
	va_list ap;
	const void *opts;

	va_start(ap, name);
	opts = va_arg(ap, const void *);
	va_end(ap);

	return vload(&lux->load, name, opts);
}

void
lux_unload(void *ins)
{
	uload(&lux->load, ins);
}
