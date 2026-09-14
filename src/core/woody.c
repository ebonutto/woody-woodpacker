#include "woody.h"

#include <string.h> // memset()

void woody_init(t_woody_ctx *ctx)
{
	memset(&ctx, 0, sizeof(ctx));

	ctx->pack_method = CODE_CAVE_INJECTION;
	ctx->cipher = RC4;
	ctx->compression = RLE;
}

int woody_pack(t_woody_ctx *ctx)
{
	int ret;

	if (map_file(ctx) != 0)
		return (1);

	ret = dispatch_format(ctx);

	unmap_file(ctx);
	return (ret);
}
