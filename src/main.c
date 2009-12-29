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
#include <lux/job.h>
#include <lux/strutils.h>
#include <lux/switch.h>
#include <string.h> /* for strcmp() */
#include <stdlib.h> /* for EXIT_SUCCESS */

#define FLAG(s) CASE(!strcmp(arg, s))

static const char banner[] = "\
  ,--.                     a flexible and extendable framework\n\
  |  ,--.,--,--.  ,--.         for scientific computation\n\
  |  |  ||  |\\  `'  /\n\
  |  |  ''  ;/  /.  \\                alpha version\n\
  `--'`----''--'  `--`             commit '" LUX_VERSION "'\n";

int
main(int argc, char *argv[])
{
	int      status = EXIT_SUCCESS;
	Lux_job *sim    = NULL;

	lux_setup();

	argv[0] = (char *)basename(argv[0]);
	if(!strcmp(argv[0], "lux")) {
		++argv;
		--argc;
	}

	/* Parse argumnet list */
	for(; argv && argc; ++argv, --argc) {
		const char *arg = *argv; /* != NULL for sure */
		SWITCH {
		FLAG("--help")
			return usage();
		FLAG("--version")
			return version();
		CASE(!sim) /* try loading arg as a sim */
			sim = (Lux_job *)lux_load("sim", arg);
			if(sim) /* TODO: parse stdin; interactive mode? */
				lux_print("\n%s\n",
				          sim->banner ? sim->banner : banner);
			else
				return unknown(arg);
		DEFAULT
			status = sim->conf(sim, arg);
			if(status)
				return unknown(arg);
		}
	}

	/* Perform the simulation */
	if(sim) {
		if(status == EXIT_SUCCESS)
			status = sim->init(sim);
		if(status == EXIT_SUCCESS)
			status = sim->exec(sim);
		lux_unload(sim);
		return status;
	} else
		return usage();
}
