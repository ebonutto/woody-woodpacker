#ifndef FORMAT_H
#define FORMAT_H

/* Includes */
#include "woody.h"

/* Types */
typedef int (*t_packer_fn)(t_woody_ctx *ctx);

/* Structures */
typedef struct s_format {
	const unsigned char magic[8];
	size_t magic_len;
	const char *name;
	t_packer_fn pack;
} t_format;

#endif
