/*
 * Copyright (C) 2011 Chi-kwan Chan
 * Copyright (C) 2011 Nordic Institute for Theoretical Physics
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
#ifndef _LUX_CONFIG_H_
#define _LUX_CONFIG_H_

#include <lux/match.h>
#include <lux/switch.h>

#define CONFIG(v, s) CASE((val = match(s, arg))) p->v

#endif /* _LUX_CONFIG_H_ */
