#include "woody.h"
#include "cipher.h"

#include <string.h> // memcpy(), memset()

void woody_init(t_woody_ctx *ctx)
{
	memset(ctx, 0, sizeof(*ctx));

	ctx->pack_method = CODE_CAVE_INJECTION;
	ctx->cipher = XOR;
	ctx->compression = RLE;
}

int woody_init_key(t_woody_ctx *ctx)
{
	if (ctx->key_size)
		return (0);

	return (generate_key(ctx));
}

int woody_pack(t_woody_ctx *ctx)
{
	int ret;

	if (map_file(ctx) != 0)
		return (1);

	ret = dispatch_format(ctx);
	if (ret == 0)
		ret = write_woody(ctx);

	unmap_file(ctx);
	return (ret);
}
