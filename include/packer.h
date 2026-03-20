#ifndef PACKER_H
#define PACKER_H

/* Includes */
#include "args.h"
#include <sys/types.h>

/* Structures */
typedef struct {
	s_args     args;
	off_t      file_size;
	void       *map;
} s_packer_ctx;

/* Prototypes */
void init_packer_ctx(s_packer_ctx *ctx);
void packer(s_packer_ctx *ctx);
void free_packer_ctx(s_packer_ctx *ctx);

#endif
