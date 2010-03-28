#include <lux.h>
#include <lux/assert.h>
#include <lux/tpool.h>
#include <stdio.h>
#include <stddef.h>

int
main()
{
	struct tpool *tp = mktpool();
	int i, n = 8;

	for(i = 0; i < n; ++i) {
		ptrdiff_t p = i+1;
		enqueue(tp, (void *)p);
	}

	fputs("Test: ", stdout);
	for(i = 0; i < n + 2; ++i) {
		ptrdiff_t p = (ptrdiff_t)dequeue(tp);
		printf("%zd ", p);
		lux_assert(p == i < n ? i+1 : 0);
	}
	fputs("DONE\n", stdout);

	for(i = 0; i < n; ++i) {
		ptrdiff_t p = i + 1;
		enqueue(tp, (void *)p);
	}

	fputs("Test: ", stdout);
	for(i = 0; i < n + 2; ++i) {
		ptrdiff_t p = (ptrdiff_t)dequeue(tp);
		printf("%zd ", p);
		lux_assert(p == i < n ? i+1 : 0);
	}
	fputs("DONE\n", stdout);

	free(tp);

	return 0;
}
