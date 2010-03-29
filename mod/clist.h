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
#ifndef _LUX_CLIST_H_
#define _LUX_CLIST_H_
/*
 * Circular singly linked list
 */
struct cnode {
	struct cnode *next;
};

static inline struct cnode *
clist_init(struct cnode *c)
{
	return c->next = c;
}

static inline struct cnode *
clist_ins(struct cnode *c, struct cnode *n)
{
	n->next = c->next;
	c->next = n;
	return c;
}

static inline struct cnode *
clist_del(struct cnode *c)
{
	struct cnode *n = c->next;
	c->next = n->next;
	return n;
}

#endif /* _LUX_CLIST_H_ */
