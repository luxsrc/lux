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
#ifndef _LUX_ALGO_H_
#define _LUX_ALGO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Type-safe container for a pointer to problem specific specification */
typedef struct {
	const void *spec;
} Lux_spec;

/* Type-safe container for a pointer to problem specific configuration */
typedef struct {
	const void *conf;
} Lux_conf;

/* Type-safe pointer to a problem specific algorithm/function */
typedef void (*Lux_algo)(Lux_spec, Lux_conf);

#ifdef __cplusplus
}
#endif

#endif /* _LUX_ALGO_H_ */
