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
#include <lux.h>
#include <stdio.h>

static const char banner[] = "\
  ,--.              portable high-performance\n\
  |  |--.--.--,--,  computing on heterogeneous\n\
  |  |  |  |\\   /   distributed systems\n\
  |  |  '  |/   \\\n\
  `---`--`-'--`--`  Pre-release '" LUX_VERSION "'";

int
main(int argc, char *argv[])
{
	lux_print("%s\n", banner);

	return 0;

	(void)argc; /* silence unused variable warning */
	(void)argv; /* silence unused variable warning */
}
