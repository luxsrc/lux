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
#include "src.h"
#include <lux/sim.h>
#include <lux/switch.h>
#include <string.h> /* for strcmp() */
#include <stdlib.h> /* for EXIT_SUCCESS and EXIT_FAILURE */

#define UNLESS(e) CASE(!(e))
#define FLAG(s)   UNLESS(strcmp(argv[1], s))

int
main(int argc, char *argv[])
{
	Lux_sim *sim = NULL;

	lux_setup();

	SWITCH {
	CASE(argc <= 1)
		return version();
	FLAG("--help")
		return usage(EXIT_SUCCESS);
	FLAG("--version")
		return version();
	UNLESS(sim = (Lux_sim *)lux_load("sim", argc-1, argv+1))
		return unknown(argv[1]);
	DEFAULT
		lux_print("\n\
  ,--.                     a flexible and extendable framework\n\
  |  ,--.,--,--.  ,--.         for scientific computation\n\
  |  |  ||  |\\  `'  /\n\
  |  |  ''  ;/  /.  \\                alpha version\n\
  `--'`----''--'  `--`             commit '" LUX_VERSION "'\n\n");

		sim->exec(sim);
		lux_unload(sim);
	}

	return EXIT_SUCCESS;
}
