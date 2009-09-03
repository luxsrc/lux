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
#ifndef _LUX_RINGBUF_H_
#define _LUX_RINGBUF_H_

#include <string.h> /* for memcpy() */

#define RINGBUF_SIZE 4096                      /* about 50 rows of text */
#define RINGBUF_INIT {0, RINGBUF_SIZE, {'\0'}} /* remainder is initialized to
                                                  '\0', see c-faq 1.30. */
struct ringbuf {
	size_t next, size;         /* size and locatoin of ring buffer */
	char   ring[RINGBUF_SIZE]; /* flexible array member */
};

static inline void
ringcat(struct ringbuf *b, const char *s, size_t n)
{
	size_t offset, usable;
	if(n > b->size) { /* save the last b->size bytes */
		const size_t skip = n - b->size;
		b->next += skip;
		s       += skip;
		n        = b->size;
	}
	offset = b->next % b->size; /* b->ring[] is a ring buffer */
	usable = b->size - offset;  /* usable bytes without wrapping around */

	if(n <= usable)
		(void)memcpy(b->ring + offset, s, n);
	else {
		(void)memcpy(b->ring + offset, s,     usable);
		(void)memcpy(b->ring, s + usable, n - usable);
	}
	b->next += n;
}

#endif /* _LUX_RINGBUF_H_ */
