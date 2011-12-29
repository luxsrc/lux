/*
 * Copyright (C) 2011 Chi-kwan Chan
 * Copyright (C) 2011 Nordic Institute for Theoretical Physics
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
#ifndef _LUX_MATCH_H_
#define _LUX_MATCH_H_

#include <string.h>

static inline const char *
match(const char *sym, const char *arg)
{
	size_t i;

	const size_t l = strlen(sym);
	const size_t n = strlen(arg);

	if(l + 1 > n || '=' != arg[l])
		return NULL;

	for(i = 0; i < l; ++i)
		if(sym[i] != arg[i])
			return NULL;

	return arg + l + 1;
}

#endif /* _LUX_MATCH_H_ */
