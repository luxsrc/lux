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
#ifndef _LUX_ASSERT_H_
#define _LUX_ASSERT_H_

#include <lux.h> /* for lux_fatal() */
/*
 * Run-time assertion similar to C's standard assert() macro
 *
 * lux_assert() is only enabled if LUX_ASSERTION is defined, which is
 * only defined if "--enable-assertion" is passed at configure time.
 * When assertion is disabled, the expreission inside lux_assert() is
 * not seen by compiler and hence has no runtime overhead.
 */
#define LUX_ASSERTION_FAILED 0

#define lux_always_assert(E) do {	                         \
	if((E) == LUX_ASSERTION_FAILED)                          \
		lux_fatal("%s:%d: assertion `" #E "` failed.\n", \
		          __LINE__, __FILE__);                   \
} while(0)

#if USE_ASSERTION
#define lux_assert(E) lux_always_assert(E)
#else
#define lux_assert(E) /* do nothing */
#endif

#endif /* _LUX_ASSERT_H_ */
