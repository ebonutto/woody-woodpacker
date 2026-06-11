#include "woody.h"

#include <elf.h>

int segment_padding(t_woody_ctx *ctx)
{
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;

	if (!IN_BOUNDS(ctx->filesize, 0, sizeof(Elf64_Ehdr))) {
		fprintf(stderr, "%s: Error: %s: file too small\n",
		        ctx->progname, ctx->filename);
		return (1);
	}
	ehdr = (Elf64_Ehdr *)ctx->map;
	if (!IN_BOUNDS(ctx->filesize, ehdr->e_phoff, sizeof(Elf64_Phdr))) { //! * 
		fprintf(stderr, "%s: Error: %s: file too small\n",
		        ctx->progname, ctx->filename);
		return (1);
	}
	phdr = (Elf64_Phdr *)(ehdr + ehdr->e_phoff);
	(void)

	return (0);
}
