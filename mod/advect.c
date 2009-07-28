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
	Lux_task super;
	size_t n;
	double c, *rho, *rhs;
};

#define TASK ((struct advect *)task)

static inline void
icond(double *rho, size_t n)
{
	size_t i;
	for(i = 0; i < n; ++i)
		rho[i] = 0.0;
}

static inline void
bcond(double *rhs, const double *rho, size_t n, double g)
{
	rhs[0] = -g * (rho[0] - 1.0);

	(void)n; /* silence unused variable warning */
}

static inline void
flux(double *rhs, const double *rho, size_t n, double g)
{
	size_t i;
	for(i = 1; i < n; ++i)
		rhs[i] = -g * (rho[i] - rho[i-1]);
}

static inline void
Euler(double *rho, const double *rhs, size_t n, double dt)
{
	size_t i;
	for(i = 0; i < n; ++i)
		rho[i] += dt * rhs[i];
}

static inline void
print(const double *rho, size_t n)
{
	size_t i;
	for(i = 0; i < n-1; ++i)
		lux_print("%7.5f\t", rho[i]);
	lux_print("%7.5f\n", rho[i]);
}

static void
exec(Lux_task *task)
{
	size_t  n   = TASK->n;
	double  dx  = 1.0 / n;
	double *rho = TASK->rho;
	double *rhs = TASK->rhs;

	size_t  h;
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
	adv->rho = (double *)malloc(sizeof(double) * adv->n);
	adv->rhs = (double *)malloc(sizeof(double) * adv->n);

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
