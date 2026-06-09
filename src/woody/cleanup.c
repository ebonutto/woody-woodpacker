#include "woody.h"

#include <sys/mman.h> // munmap

void woody_cleanup(t_woody_ctx *ctx)
{
	munmap(ctx->map, ctx->filesize);
}
