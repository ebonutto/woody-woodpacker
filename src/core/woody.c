#include "woody.h"

#include <sys/mman.h> // MAP_FAILED, MAP_PRIVATE, PROT_READ, PROT_WRITE, munmap(), mmap()


int woody(t_woody_ctx *ctx)
{
	int ret;

	if (map_file(ctx) != 0)
		return (1);
	ret = dispatch_format(ctx);
	munmap(ctx->map, ctx->filesize); //TODO unmap_file()
	return (ret);
}
