#include "woody.h"

#include "format.h"

#include <stddef.h> // NULL
#include <stdio.h>  // stderr, fprintf()
#include <string.h> // memcmp()


#define MAX_MAGIC 4


int dispatch_format(t_woody_ctx *ctx)
{
	static const t_format formats[] = {
		{{0x7F, 'E', 'L', 'F' }, 4, "ELF", pack_elf},
		{{0x00, 0x00, 0x00, 0x00 }, 0, NULL, NULL}
	};
	const t_format *fmt;

	if (ctx->filesize < (off_t)MAX_MAGIC) {
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
