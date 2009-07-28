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
#include <lux/task.h>
#include <stdlib.h>

struct advect {
	Lux_task  super;
	Lux_whole n;
	Lux_real  c, *rho, *rhs;
};

#define TASK ((struct advect *)task)

static inline void
icond(Lux_real *rho, Lux_whole n)
{
	Lux_whole i;
	for(i = 0; i < n; ++i)
		rho[i] = 0.0;
}

static inline void
bcond(Lux_real *rhs, const Lux_real *rho, Lux_whole n, Lux_real g)
{
	rhs[0] = -g * (rho[0] - 1.0);

	(void)n; /* silence unused variable warning */
}

static inline void
flux(Lux_real *rhs, const Lux_real *rho, Lux_whole n, Lux_real g)
{
	Lux_whole i;
	for(i = 1; i < n; ++i)
		rhs[i] = -g * (rho[i] - rho[i-1]);
}

static inline void
Euler(Lux_real *rho, const Lux_real *rhs, Lux_whole n, Lux_real dt)
{
	Lux_whole i;
	for(i = 0; i < n; ++i)
		rho[i] += dt * rhs[i];
}

static inline void
print(const Lux_real *rho, Lux_whole n)
{
	Lux_whole i;
	for(i = 0; i < n-1; ++i)
		lux_print("%7.5f\t", rho[i]);
	lux_print("%7.5f\n", rho[i]);
}

static void
exec(Lux_task *task)
{
	Lux_whole n   = TASK->n;
	Lux_real  dx  = 1.0 / n;
	Lux_real *rho = TASK->rho;
	Lux_real *rhs = TASK->rhs;

	Lux_whole h;
	for(h = 0; h < n; ++h) {
		bcond(rhs, rho, n, TASK->c / dx);
		flux (rhs, rho, n, TASK->c / dx);
		Euler(rho, rhs, n, dx / TASK->c);
		print(rho, n);
	}
}

void *
LUXC(void)
{
	struct advect *adv = (struct advect *)malloc(sizeof(struct advect));
	adv->super.exec = exec;

	adv->n   = 10;
	adv->c   = 1.0;
	adv->rho = (Lux_real *)malloc(sizeof(Lux_real) * adv->n);
	adv->rhs = (Lux_real *)malloc(sizeof(Lux_real) * adv->n);

	icond(adv->rho, adv->n);
	print(adv->rho, adv->n);

	return adv;
}

void
LUXD(Lux_task *task)
{
	free(TASK->rhs);
	free(TASK->rho);
	free(task);
}
