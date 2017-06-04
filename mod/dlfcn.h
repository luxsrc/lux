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
#ifndef _LUX_DLFCN_H_
#define _LUX_DLFCN_H_

#ifndef _GNU_SOURCE
#	define _GNU_SOURCE
#	define _GNU_SOURCE_BY_ME
#endif

#include <dlfcn.h>

#ifdef _GNU_SOURCE_BY_ME
#	undef _GNU_SOURCE_BY_ME
#	undef _GNU_SOURCE
#	ifdef __USE_GNU
#		undef __USE_GNU
#	endif
#endif

#ifndef HAVE_DLMOPEN
	typedef long int Lmid_t; /* from glibc */

	extern void *dlmopen(Lmid_t, const char *, int);

#	define LM_ID_BASE   0 /* initial namespace */
#	define LM_ID_NEWLM -1 /* For dlmopen: request new namespace.  */
#endif

#ifndef HAVE_DLFUNC
	struct __dlfunc_arg { int __dlfunc_dummy; };
	typedef	void (*dlfunc_t)(struct __dlfunc_arg); /* from BSD */

	extern dlfunc_t dlfunc(void *, const char *);
#endif

#endif /* _LUX_DLFCN_H_ */
