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
#include <lux/dlfcn.h>
#include <lux/lazybuf.h>
#include <stdlib.h> /* for NULL */
#include "mod.h"

static const char preamble_fmt[] = "\
typedef unsigned whole;\n\
typedef int      integer;\n\
\n\
typedef %s   fast;\n\
typedef %s2  fast2;\n\
typedef %s4  fast4;\n\
typedef %s8  fast8;\n\
typedef %s16 fast16;\n\
\n\
typedef %s   real;\n\
typedef %s2  real2;\n\
typedef %s4  real4;\n\
typedef %s8  real8;\n\
typedef %s16 real16;\n\
\n\
typedef %s   extended;\n\
typedef %s2  extended2;\n\
typedef %s4  extended4;\n\
typedef %s8  extended8;\n\
typedef %s16 extended16;\n\
\n\
#define K(x) x %s\n\
\n\
";

cl_program
mkpro(struct opencl *ego, const struct LuxOopencl *opts,
      size_t ndev, cl_device_id *dev)
{
	cl_program pro;
	cl_int     err;
	size_t     i;

	const char *path;

	char buf[1024];
	const char *src[16] = {buf, NULL};

	const char *fn = prectostr(ego->fastsz);
	const char *rn = prectostr(ego->realsz);
	const char *xn = prectostr(ego->extendedsz);
	snprintf(buf, sizeof(buf), preamble_fmt,
	         fn, fn, fn, fn, fn,
	         rn, rn, rn, rn, rn,
	         xn, xn, xn, xn, xn,
	         ego->realsz>4 ? ego->realsz>8 ? "## L" : "" : "## f");

	path = dlfname(opts->base ? (void *)opts->base : (void *)LUX_MKMOD);
	for(i = 0; opts->src[i]; ++i) {
		if(strlen(opts->src[i]) < 256) { /* UGLY HACK */
			const char *s = getsrc(path, opts->src[i]);
			if(!s) {
				lux_error("Failed to load source \"%s/%s\"\n",
				          path, opts->src[i]);
				exit(1);
			}
			src[i+1] = s;
		} else
			src[i+1] = opts->src[i];
	}

	pro = clCreateProgramWithSource(ego->super.ctx, i+1, src, NULL, &err);
	if(!pro || err) {
		lux_error("Failed to create program %p [%d]\n", pro, err);
		exit(1);
	}

	snprintf(buf, sizeof(buf),
	         "-cl-kernel-arg-info %s", opts->flags ? opts->flags : "");
	err = clBuildProgram(pro, ndev, dev, buf, NULL, NULL);
	if(err != CL_SUCCESS) {
		char   lazybuf[8192], *buf = lazybuf;
		size_t sz = sizeof(lazybuf);

		lux_error("Failed to build program\n");
		for(i = 0; i < ndev; ++i) {
			getinfo(ProgramBuild, retry,
			        pro, dev[i], CL_PROGRAM_BUILD_LOG);
			lux_error("%s\n", buf);
		}

		lzfree(buf);
		exit(1);
	}

	return pro;
}
