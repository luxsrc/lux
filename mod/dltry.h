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
#ifndef _LUX_DLTRY_H_
#define _LUX_DLTRY_H_

#include <lux/exception.h>

#include <dlfcn.h> /* for dlopen(), dlsym(), and dlerror() */

#ifdef __cplusplus
extern "C" {
#endif

static inline void *
dltryopen(const char *restrict filename, int flags)
{
	int   e = exception;
	void *h = dlopen(filename, flags);
	if(!h) {
		(void)dlerror(); /* clear error message */
		exception = e;   /* restore previous failure code */
	}
	return h;
}

static inline void *
dltrysym(void *restrict handle, const char *restrict symname)
{
	int   e = exception;
	void *s = dlsym(handle, symname);
	if(!s) {
		(void)dlerror(); /* clear error message */
		exception = e;   /* restore previous failure code */
	}
	return s;
}

#ifdef __cplusplus
}
#endif

#endif /* _LUX_DLTRY_H_ */
