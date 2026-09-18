#include "woody.h"

#include <elf.h> // EI_CLASS, EI_NIDENT
#include <stdio.h> // stderr, fprintf()

int pack_elf(t_woody_ctx *ctx)
{
	unsigned char *e_ident;

	if (ctx->filesize < EI_NIDENT) {
		fprintf(stderr, "%s: Error: %s: file too small\n",
		        ctx->progname, ctx->filename);
		return (1);
	}

	e_ident = (unsigned char *)ctx->map;

	if (e_ident[EI_CLASS] == ELFCLASS64)
		return (pack_elf64(ctx));
	// if (e_ident[EI_CLASS] == ELFCLASS32)
	// 	return (pack_elf32(ctx));

	fprintf(stderr, "%s: Error: %s: unknown ELF class\n",
	        ctx->progname, ctx->filename);
	return (1);
}
