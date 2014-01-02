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
#ifndef _LUX_CHECK_H_
#define _LUX_CHECK_H_

#include <lux/failed.h>

/*
 * Fail if E returns a non-zero value; useful in checking errno or failed
 *
 * Note that we explicitly cast E to int.  This is intentional because
 * this macro checks for failure code, which has type int.
 */
#ifndef LUX_CHECK_INFAILURE
#define LUX_CHECK_INFAILURE 0
#endif

#define lux_check_failure_code(E, L) do {                                   \
	int __f__ = (int)(E);                                               \
	if(__f__ != LUX_CHECK_INFAILURE) {                                  \
		lux_error("lux_check_failure_code(" #E ", " #L ") failed "  \
		          "with error code %d on line %d in \"%s\"\n",      \
		          __f__, __LINE__, __FILE__);                       \
		goto L;                                                     \
	}                                                                   \
} while(0)

/*
 * Fail if E returns zero; useful in checking memory allocations
 *
 * Unlike lux_check_failure_code(), we avoid explicitly casting E to
 * any type.
 */
#ifndef LUX_CHECK_UNSUCCESS
#define LUX_CHECK_UNSUCCESS 0
#endif

#define lux_check_func_success(E, L) do {                                  \
	int __fsv__ = failed;                                              \
	if((E) == LUX_CHECK_UNSUCCESS) {                                   \
		lux_error("lux_check_func_success(" #E ", " #L ") failed " \
		          "with error code %d on line %d in \"%s\"\n",     \
		          failed, __LINE__, __FILE__);                     \
		failed = __fsv__;                                          \
		goto L;                                                    \
	}                                                                  \
} while(0)

#endif /* _LUX_CHECK_H_ */
