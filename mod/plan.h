/*
 * Copyright 2017 Chi-kwan Chan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _LUX_PLAN_H_
#define _LUX_PLAN_H_

#include <lux/algo.h>

#ifdef __cplusplus
extern "C" {
#endif

struct plan_s {
	void (*algo)(Lux_spec, Lux_conf);
	Lux_spec spec;
};

static inline void
lp_apply(struct plan_s p, Lux_conf c)
{
	p.algo(p.spec, c);
}

#ifdef __cplusplus
}
#endif

#endif /* _LUX_PLAN_H_ */
