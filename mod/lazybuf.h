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
#ifndef _LUX_LAZYBUF_H_
#define _LUX_LAZYBUF_H_

#include <stdlib.h> /* for malloc(), realloc(), and free() */
#include <string.h> /* for memcpy() */

#ifdef __cplusplus
extern "C" {
#endif

static inline void *
_lzmalloc(void *b, size_t bsz, size_t sz)
{
	return sz > bsz ? malloc(sz) : b;
}

static inline void *
_lzrealloc(void *b, size_t bsz, void *pt, size_t sz)
{
	if(pt != b) /* most likely, use system realloc() */
		return realloc(pt, sz);
	else if(sz == 0) /* emulate the system realloc() behavior */
		return NULL;
	else if(sz <= bsz) /* buffer is big enough, do nothing */
		return pt;
	else { /* i.e., pt == b && sz > bsz: allocate memory and copy data */
		b = malloc(sz);
		return b ? memcpy(b, pt, bsz) : b;
	}
}

static inline void
_lzfree(void *b, void *pt)
{
	if(pt != b)
		free(pt);
}

#ifndef lzbuf
#define lzbuf _lzbuf
#endif

#define lzmalloc(sz)      _lzmalloc(lzbuf, sizeof(lzbuf), sz)
#define lzrealloc(pt, sz) _lzrealloc(lzbuf, sizeof(lzbuf), pt, sz)
#define lzfree(pt)        _lzfree(lzbuf, pt)

#ifdef __cplusplus
}
#endif

#endif /* _LUX_LAZYBUF_H_ */
