/*
 * Copyright (C) 2010 Chi-kwan Chan
 * Copyright (C) 2010 Harvard-Smithsonian Center for Astrophysics
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
#ifndef _LUX_STACK_H_
#define _LUX_STACK_H_

#include <lux/list.h>
#include <stddef.h>

static inline struct snode *
stack_init(struct snode *h)
{
	return h->next = NULL;
}

static inline struct snode *
stack_push(struct snode *h, struct snode *s)
{
	s->next = h->next;
	h->next = s;
	return h;
}

static inline struct snode *
stack_pop(struct snode *h)
{
	struct snode *s = h->next;
	if(s)
		h->next = s->next;
	return s;
}

#endif /* _LUX_STACK_H_ */
