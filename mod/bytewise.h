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
#ifndef _LUX_BYTEWISE_H_
#define _LUX_BYTEWISE_H_

static inline void
reverse(char *restrict s, size_t l, size_t u)
{
	for(; l < u; ++l, --u) {
		s[l] ^= s[u];
		s[u] ^= s[l];
		s[l] ^= s[u];
	}
}

static inline void
leftrot(char *restrict s, size_t c, size_t n)
{
	if(c %= n) {
		reverse(s, c, n - 1);
		reverse(s, 0, c - 1);
		reverse(s, 0, n - 1);
	}
}

static inline void
rightrot(char *restrict s, size_t c, size_t n)
{
	if(c %= n) {
		reverse(s, 0, n - 1);
		reverse(s, 0, c - 1);
		reverse(s, c, n - 1);
	}
}

#endif /* _LUX_BYTEWISE_H_ */
