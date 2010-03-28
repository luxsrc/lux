#include <lux.h>
#include <lux/thread.h>
#include <lux/tpool.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

struct task {
	Lux_task super;
	int id;
};

void
exec(Lux_task *t)
{
	printf("[%p] %d\n", pthread_self(), ((struct task *)t)->id);
	sleep(1);
}

int
main()
{
	int i, n = 8;

	struct tpool *q = mktpool(4);
	struct task  *t = malloc(n * sizeof(struct task));
	for(i = 0; i < n; ++i) {
		t[i].super.exec = exec;
		t[i].id         = i + 1;
	}

	for(i = 0; i < n; ++i)
		enqueue(q, &t[i].super);

	for(i = 0; i < n; ++i)
		enqueue(q, &t[i].super);

	tpool_wait(q);

	free(t);
	free(q);

	return 0;
}
