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
#include <lux/log.h>

static unsigned levels[16] = LEVELS_INIT;

void
lux_vlog(unsigned flags, const char *restrict fmt, va_list ap)
{
	while(flags < LUX_LOG_FLAGS)
		flags = levels[flags]; /* FIXME: check number of levels */

	if(flags & LUX_LOG_SUSPEND)
		return; /* suspended; do nothing */

	vlog(flags & (LUX_LOG_FATAL-1), fmt, ap);

	if(flags & LUX_LOG_FATAL)
		lux_abort();
}
