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
#include <lux/dlfcn.h>

#if !HAVE_DLMOPEN /* always uses base namespace if dlmopen() is not defined */
Lmid_t
dllmid(void *mod)
{
	return LM_ID_BASE;
	(void)mod; /* silence unused parameter warning */
}
#elif HAVE_DLINFO /* otherwise, try using dlinfo() to get namespace */
Lmid_t
dllmid(void *mod)
{
	Lmid_t namespace;
	if(dlinfo(mod, RTLD_DI_LMID, &namespace) == 0)
		return namespace;
	else
		return 0;
}
#else /* give up */
#error cannot implment dllmid() when dlinfo() is missing
#endif
