#include "woody.h"

#include "format.h"

#include <stddef.h> // NULL
#include <stdio.h>  // stderr, fprintf()
#include <string.h> // memcmp()


int dispatch_format(t_woody_ctx *ctx)
{
	static const t_format formats[] = {
		{{0x7F, 'E', 'L', 'F' }, 4, "ELF", pack_elf},
		{{0x00, 0x00, 0x00, 0x00 }, 0, NULL, NULL}
	};
	size_t         max_magic;
	const t_format *fmt;

	max_magic = 0;
	for (fmt = formats; fmt->magic_len; fmt++)
		if (fmt->magic_len > max_magic)
			max_magic = fmt->magic_len;
	if (ctx->filesize < (off_t)max_magic) {
		fprintf(stderr, "%s: Error: %s: file too small\n",
		        ctx->progname, ctx->filename);
	}
	for (fmt = formats; fmt->magic_len; fmt++) {
		if (memcmp(ctx->map, fmt->magic, fmt->magic_len) == 0)
			return (fmt->pack(ctx));
	}
	fprintf(stderr, "%s: Error: %s: Unknown format\n",
	        ctx->progname, ctx->filename);
	return (1);
}
