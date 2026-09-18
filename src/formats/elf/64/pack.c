#include "woody.h"

#include <stdio.h> // stderr, fprintf()

int pack_elf64(t_woody_ctx *ctx)
{
	switch (ctx->pack_method) {
	case CODE_CAVE_INJECTION:
		return (code_cave_injection(ctx));
	}

	fprintf(stderr, "%s: Error: %s: invalid elf64 pack method\n",
	        ctx->progname, ctx->filename);
	return (1);
}
