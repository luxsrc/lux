/*
 * Copyright (C) 2016 Chi-kwan Chan
 * Copyright (C) 2016 Steward Observatory
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
#ifndef _LUX_FILE_H_
#define _LUX_FILE_H_

typedef struct LuxSfile Lux_file;

struct LuxSfile {
	void  (*close   )(Lux_file *);
	void  (*write_pa)(Lux_file *, const char *, int, const void *);
	void *(*read_pa )(Lux_file *, const char *);
};

#endif /* _LUX_FILE_H_ */
