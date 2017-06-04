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
#ifndef _LUX_EXCEPTION_H_
#define _LUX_EXCEPTION_H_

#include <lux.h>    /* for LUX_EXCEPTION_BIT */
#include <errno.h>  /* for errno */
#include <string.h> /* for strerror() */

#ifdef __cplusplus
extern "C" {
#endif

#define EXCEPTION ((1U << LUX_EXCEPTION_BIT) - 1)
#define exception errno

static inline const char *
strexception(int e)
{
	e &= EXCEPTION;

	switch(e) {
	case EXCEPTION:
		return "Generic exception";
	default:
		return strerror(e);
	}
}

#ifdef __cplusplus
}
#endif

#endif /* _LUX_EXCEPTION_H_ */
