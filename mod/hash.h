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
#ifndef _LUX_HASH_H_
#define _LUX_HASH_H_

#if HAVE_STDINT_H
#include <stdint.h>
#else
#error <stdint.h> is not found on this platform
#endif

/* A terrible hash but hopefully enough for now; we may pass its
   result to wang32() to improve its randomness */
static inline uint32_t
djb2(const char *restrict s)
{
	uint32_t h = 5381, c;

	while((c = *s++))
		h = ((h << 5) + h) + c; /* == h * 33 + c */

	return h;
}

/* The following two integer hash functions are copied from Thomas Wang;
   see his website <http://www.concentric.net/~Ttwang/tech/inthash.htm> */
static inline uint32_t
wang32(uint32_t h)
{
	h = ~h + (h << 15);
	h =  h ^ (h >> 12);
	h =  h + (h <<  2);
	h =  h ^ (h >>  4);
	h =  h * (   2057);
	h =  h ^ (h >> 16);

	return h;
}
static inline uint64_t
wang64(uint64_t h)
{
	h = ~h + (h << 21);
	h =  h ^ (h >> 24);
	h =  h * (    265);
	h =  h ^ (h >> 14);
	h =  h * (     21);
	h =  h ^ (h >> 28);
	h =  h + (h << 31);

	return h;
}

/* Generic integer hash; branching is optimized away at compile time */
#define WANG(h) (sizeof(h) > 4 ? wang64(h) : wang32(h))

#endif /* _LUX_HASH_H_ */
