#include "woody.h"


int woody(t_woody_ctx *ctx)
{
	int ret;

	if (map_file(ctx) != 0)
		return (1);
	ret = dispatch_format(ctx);
	munmap(ctx->map, ctx->filesize); //TODO unmap_file()
	return (0);
}
