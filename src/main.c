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
#include <lux/lazybuf.h>
#include <lux/task.h>
#include <stdlib.h> /* for EXIT_SUCCESS and EXIT_FAILURE */
#include <stdio.h>  /* for sprintf() */

int
main(int argc, char *argv[])
{
	lux_setup();

	if(argc <= 1)
		lux_print("lux ("PACKAGE_NAME") commit '"PACKAGE_VERSION"'\n");
	else {
		char lazybuf[256], *buf;
		Lux_task *task;

		buf = (char *)MALLOC(6 + strlen(argv[1]));
		if(!buf)
			return EXIT_FAILURE;
		(void)sprintf(buf, "task/%s", argv[1]);
		task = (Lux_task *)lux_load(buf);
		FREE(buf);

		if(!task)
			return EXIT_FAILURE;
		task->exec(task);
		lux_unload(task);
	}

	return EXIT_SUCCESS;
}
