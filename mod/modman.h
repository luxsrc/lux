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
#ifndef _LUX_MODMAN_H_
#define _LUX_MODMAN_H_

#include <lux/failed.h>
#include <lux/htab.h>
#include <lux/load.h>

static inline void *
vmkmod(struct htab *ltab, const char *restrict name, const void *opts)
{
	struct load_node *node = vload(name, opts);

	if(node)
		hadd(ltab, (uintptr_t)node->ins, (struct htab_node *)node);
	else
		failed = FNOMOD;

	return node ? node->ins : node;
}

static inline void
rmmod(struct htab *ltab, void *ins)
{
	struct load_node *node = (struct load_node *)hpop(ltab, (uintptr_t)ins);

	if(node)
		uload(node);
	else
		failed = FNOMOD;
}

#endif /* _LUX_MODMAN_H_ */
