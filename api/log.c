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
#include <lux/ringbuf.h>
#include <lux/ringlog.h>

static struct ringbuf buf = RINGBUF_INIT;

int lux_log_debug = 0; /* debugging message is disabled by default */
int lux_log_print = 1;
int lux_log_error = 2;

void
lux_vlog(int flag, const char *restrict fmt, va_list ap)
{
	FILE *stream = NULL;
	switch(flag) {
	case 1: stream = stdout; break;
	case 2: stream = stderr; break;
	}

	if(stream) {
		int f = failed;
		int n = vfprintf(stream, fmt, ap);
		if(n < 0)
			failed = f; /* restore failure code to hide possible
			               error emitted by vfprintf();
			               TODO: make lux_vlog() more robust */
	} else
		vringlog(&buf, fmt, ap);
}

void
lux_fput(FILE *stream)
{
	if(stream)
		fputring(&buf, stream);
	else
		ringerase(&buf);
}
