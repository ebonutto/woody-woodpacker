#include "woody.h"

#include <stdio.h> // stderr, fprintf()


int dispatch_format(t_woody_ctx *ctx)
{
	const unsigned char ELF_MAGIC[4] = {0x7F, 'E', 'L', 'F'};

	magic = (unsigned char *)ctx->map;
	if (memcmp(ctx->map, ELF_MAGIC, 4) == 0)
		return (pack_elf(ctx));
	fprintf(stderr, "Unknown format\n");
	return (1);
}
