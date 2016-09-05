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
#include <lux.h>
#include <lux/lazybuf.h>
#include <stdlib.h> /* for NULL */
#include "mod.h"

cl_platform_id
lsplf(Lux_opencl *ego, unsigned iplf)
{
	cl_platform_id p[PLF_COUNT];
	cl_uint        i, n;

	char   lazybuf[1024], *buf = lazybuf;
	size_t sz = sizeof(lazybuf);

	check(GetPlatformIDs, PLF_COUNT, p, &n);

	lux_print("%d platform%s found:\n", n, n > 1 ? "s are" : " is");
	for(i = 0; i < n; ++i) {
		getinfo(Platform, retry_name,    p[i], CL_PLATFORM_NAME);
		lux_print("%s\t%d. %s ", i == iplf ? "*" : "", i, buf);

		getinfo(Platform, retry_vendor,  p[i], CL_PLATFORM_VENDOR);
		lux_print("by %s: ", buf);

		getinfo(Platform, retry_version, p[i], CL_PLATFORM_VERSION);
		lux_print("%s\n", buf);
	}

	lzfree(buf);
	return p[iplf < n ? iplf : n];

	(void)ego; /* silence unused variable warning */
}

int
lsdev(Lux_opencl *ego, unsigned iplf, unsigned idev, cl_device_type devtype)
{
	cl_platform_id p[PLF_COUNT];
	cl_device_id   d[DEV_COUNT];
	cl_uint        i, n;

	char   lazybuf[1024], *buf = lazybuf;
	size_t sz = sizeof(lazybuf);

	check(GetPlatformIDs, PLF_COUNT, p, &n);

	if(iplf >= n)
		lux_fatal("iplf == %u >= %u == nplf", iplf, n);

	check(GetDeviceIDs, p[iplf], devtype, DEV_COUNT, d, &n);

	if(idev >= n)
		lux_fatal("idev == %u >= %u == nplf", idev, n);

	lux_print("%d device%s found:\n", n, n > 1 ? "s are" : " is");
	for(i = 0; i < n; ++i) {
		getinfo(Device, retry_name,    d[i], CL_DEVICE_NAME);
		lux_print("%s\t%d. %s ", i == idev ? "*" : "", i, buf);

		getinfo(Device, retry_vendor,  d[i], CL_DEVICE_VENDOR);
		lux_print("by %s: ", buf);

		getinfo(Device, retry_version, d[i], CL_DEVICE_VERSION);
		lux_print("%s\n", buf);
	}

	lzfree(buf);
	return EXIT_SUCCESS;

	(void)ego; /* silence unused variable warning */
}
