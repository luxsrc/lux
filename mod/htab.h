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
#ifndef _LUX_HTAB_H_
#define _LUX_HTAB_H_

#include <lux/hash.h>

#if HAVE_STDDEF_H
#include <stddef.h> /* for NULL and size_t */
#else
#include <stdlib.h> /* for NULL and size_t */
#endif

#define HTAB_COUNT 256                  /* default number of nodes */
#define HTAB_NULL  {HTAB_COUNT, {NULL}} /* remainder is initialized to 0,
                                           see c-faq 1.30. */
struct htab_node {
	struct htab_node *next;
	uintptr_t         key;
};

struct htab {
	size_t            count;            /* number of nodes */
	struct htab_node *node[HTAB_COUNT]; /* flexible array member */
};

static inline void
hadd(struct htab *h, uintptr_t k, struct htab_node *n)
{
	const size_t i = WANG(k) % h->count;
	n->next    = h->node[i];
	n->key     = k;
	h->node[i] = n;
}

static inline struct htab_node *
hpop(struct htab *h, uintptr_t k)
{
	struct htab_node **p = h->node + WANG(k) % h->count;
	while(*p) {
		struct htab_node *n = *p;
		if(n->key == k) {
			*p = n->next;
			return n;
		}
		p = &n->next;
	}
	return NULL;
}

static inline struct htab_node *
hget(struct htab *h, uintptr_t k)
{
	struct htab_node **p = h->node + WANG(k) % h->count;
	while(*p) {
		struct htab_node *n = *p;
		if(n->key == k) {
			/* Compare to hpop(): do not do `*p = n->next;` */
			return n;
		}
		p = &n->next;
	}
	return NULL;
}

#endif /* _LUX_HTAB_H_ */
