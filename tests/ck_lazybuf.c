/*
 * Copyright 2017 Chi-kwan Chan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <lux.h>
#include <lux/assert.h>
#include <lux/lazybuf.h>
#include <string.h>

#define A(E) lux_always_assert(E)

int
main(int argc, char *argv[])
{
	char lzbuf[256] = "", *buf;

	buf = strcpy(lzmalloc(64), "lux");
	A(buf == lzbuf && !strcmp(buf, "lux"));

	buf = lzrealloc(buf, 128);
	A(buf == lzbuf && !strcmp(buf, "lux"));

	buf = lzrealloc(buf, 256);
	A(buf == lzbuf && !strcmp(buf, "lux"));

	buf = lzrealloc(buf, 512);
	A(buf != lzbuf && !strcmp(buf, "lux"));

	buf = lzrealloc(buf, 1024);
	A(buf != lzbuf && !strcmp(buf, "lux"));

	lzfree(buf);
	return 0;

	(void)argc; /* silence unused variable warning */
	(void)argv; /* silence unused variable warning */
}
