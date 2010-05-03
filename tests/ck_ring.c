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
#ifndef LUX_ASSERTION
#define LUX_ASSERTION 1
#endif

#include <lux.h>
#include <lux/assert.h>
#include <lux/dynamic.h>
#include <lux/ring.h>
#include <stdlib.h>
#include <stdio.h>

struct node {
	struct slist_node super;
	int value;
};

static inline struct node *
mknode(int value)
{
	return dynamic(struct node, {RING_INIT(&_->super), value});
}

int
main()
{
	int i, n = 10, offset;

	struct node *h = mknode(-1);

	offset = 100;
	for(i = 0; i < n; ++i)
		ring_ins(&h->super, &mknode(offset+i)->super);
	for(i = 0; i < n+1; ++i) {
		struct node *s = (struct node *)ring_pop(&h->super);
		if(s == h) {
			printf("empty\n");
			lux_assert(s->value == -1);
		} else {
			printf("%d ", s->value);
			lux_assert(s->value == offset+(n-1-i));
			free(s);
		}
	}

	offset = 110;
	for(i = 0; i < n; ++i)
		ring_ins(&h->super, &mknode(offset+i)->super);
	for(i = 0; i < n+1; ++i) {
		struct node *s = (struct node *)ring_pop(&h->super);
		if(s == h) {
			printf("empty\n");
			lux_assert(s->value == -1);
		} else {
			printf("%d ", s->value);
			lux_assert(s->value == offset+(n-1-i));
			free(s);
		}
	}

	free(h);

	return 0;
}
