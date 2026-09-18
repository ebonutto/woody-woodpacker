#include "woody.h"

#include <stdint.h> // uintX_t

void compress_segment(t_woody_ctx *ctx, uint64_t offset, uint64_t size)
{
	(void)ctx;
	(void)offset;
	(void)size;
}

void encrypt_segment(t_woody_ctx *ctx, uint64_t offset, uint64_t size)
{
	xor(ctx->map + offset, size, (unsigned char *)ctx->key, ctx->key_size);
}
