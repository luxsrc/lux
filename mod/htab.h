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
#include <stdlib.h> /* for malloc() */

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

static inline void *
hadd(struct htab *h, uintptr_t key, size_t sz)
{
	struct htab_node *node = (struct htab_node *)malloc(sz);
	if(node) {
		const size_t k = WANG(key) % h->count;
		node->next     = h->node[k];
		node->key      = key;
		h->node[k]     = node;
	}
	return node;
}

static inline void *
hpop(struct htab *h, uintptr_t key)
{
	struct htab_node **pnode = h->node + WANG(key) % h->count;
	while(*pnode) {
		struct htab_node *node = *pnode;
		if(node->key == key) {
			*pnode = node->next;
			return node;
		}
		pnode = &node->next;
	}
	return NULL;
}

#define HADD(h, k, t) ((t *)hadd(h, (uintptr_t)(k), sizeof(t)))
#define HPOP(h, k, t) ((t *)hpop(h, (uintptr_t)(k)))

#endif /* _LUX_HTAB_H_ */
