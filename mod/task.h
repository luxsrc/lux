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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Lux_task_s *Lux_task;

struct Lux_task_s {
	int (*exec)(Lux_task);
};

#ifdef __cplusplus
}
#endif

#endif /* _LUX_TASK_H_ */
