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
#ifndef _LUX_INSTANCE_H_
#define _LUX_INSTANCE_H_

#include <lux/task.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LuxSinstance *Lux_instance;

struct LuxSinstance {
	void     (*log)(Lux_instance, const char *, ...);
	Lux_task (*que)(Lux_instance, Lux_task [], Lux_task);
};

#ifdef __cplusplus
}
#endif

#endif /* _LUX_INSTANCE_H_ */
