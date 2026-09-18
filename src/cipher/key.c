#include "woody.h"

#include <sys/types.h> // ssize_t
#include <sys/random.h> // getrandom()
#include <stdio.h> // perror()

int generate_key(t_woody_ctx *ctx)
{
	ssize_t ret;

	ret = getrandom(ctx->key, MAX_KEY_SIZE, 0);
	if (ret != MAX_KEY_SIZE) {
		perror("getrandom()");
		return (1);
	}

	ctx->key_size = MAX_KEY_SIZE;
	return (0);
}
