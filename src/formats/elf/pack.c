#include "woody.h"

int pack_elf(t_woody_ctx *ctx)
{

	// if () // Check if exec and 64 or 32
	segment_padding(ctx);
	write_woody(ctx);
	return (0);
}
