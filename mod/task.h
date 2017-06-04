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
#ifndef _LUX_TASK_H_
#define _LUX_TASK_H_

#include <lux/plan.h>
#include <lux/algo.h>

#ifdef __cplusplus
extern "C" {
#endif

struct task_s {
	struct plan_s plan;
	Lux_conf conf;
};

static inline void
lt_exec(struct task_s t)
{
	lp_apply(t.plan, t.conf);
}

#ifdef __cplusplus
}
#endif

#endif /* _LUX_TASK_H_ */
