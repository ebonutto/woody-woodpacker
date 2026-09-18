#include "woody.h"

#include <stdint.h> // uintX_t

void compress_segment(t_woody_ctx *ctx, uint64_t offset, uint64_t size)
{
	(void)ctx;
	(void)offset;
	(void)size;
	// if (ctx->compression == RLE)
	// 	rle_compress(ctx->map + offset, );
}

void encrypt_segment(t_woody_ctx *ctx, uint64_t offset, uint64_t size)
{
	if (ctx->cipher == XOR)
		xor(ctx->map + offset, size, (unsigned char *)ctx->key,
	            ctx->key_size);
}
