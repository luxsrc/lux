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
#ifndef _LUX_RINGLOG_H_
#define _LUX_RINGLOG_H_

#include <lux/bytewise.h>
#include <lux/failed.h>
#include <lux/ringbuf.h>
#include <ctype.h> /* for isspace() */

static inline void
vringlog(struct ringbuf *b, const char *fmt, va_list ap)
{
	int f = failed; /* save failure code as vs[n]printf() may change it */

	char msg[256]; /* about 3 rows of text */
#if HAVE_VSNPRINTF
	int n = vsnprintf(msg, sizeof(msg), fmt, ap);
	if(n >= (int)sizeof(msg)) { /* message truncated; no need to abort */
		for(n = sizeof(msg)-1; n >= 3; --n)
			if(isspace(msg[n-3]))
				break;
		msg[n-2] = msg[n-1] = msg[n] = '.'; /* add ellipsis */
	}
#else
#warning vsnprintf() is not found on this platform
	int n = vsprintf(msg, fmt, ap);
	if(n >= (int)sizeof(msg)) { /* may mess up important data: fatal */
		fputs("vringlog(): buffer overflow\n", stderr);
		lux_abort();
	}
#endif
	if(n < 0) /* catch vs[n]printf() error */
		perror("vringlog(): fail to log message\n");
	else
		ringcat(b, msg, n);

	failed = f; /* restore failure code; vringlog() effectively
	               cannot fail */
}

static inline void
fputring(struct ringbuf *b, FILE *stream)
{
	int f;

	if(b->next == 0)
		return; /* avoid system call when ring buffer is empty */

	if(b->next > b->size) {
		size_t erased = b->next - b->size;
		b->next = b->size; /* there are only b->size useful bytes */
		while(b->next > b->size / 16 * 15
		      && !isspace(b->ring[(erased+3) % b->size])) {
			++erased;
			--b->next;
		}
		leftrot(b->ring, erased, b->size);
		b->ring[2] = b->ring[1] = b->ring[0] = '.';
	}

	f = failed;
	if(fwrite(b->ring, 1, b->next, stream) == b->next)
		b->next = 0; /* logically erase ring buffer */
	else {
		perror("fputring(): fail to output the internal log\n");
		failed = f; /* restore failure code */
	}
}

#endif /* _LUX_RINGLOG_H_ */
