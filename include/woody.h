#ifndef WOODY_H
#define WOODY_H

/* Includes */
#include <sys/types.h> // off_t

/* Structures */
typedef struct s_woody_ctx {
	char *progname;
	char *filename;
	char *compression;
	char *cipher;
	// key
	off_t filesize;
	void *map;
	size_t oep;
} t_woody_ctx;

/* Prototypes */
void xor_rolling(unsigned char *buf, size_t len, const unsigned char *key, size_t key_len);

int parse_args(t_woody_ctx *ctx, int argc, char **argv);

int map_file(t_woody_ctx *ctx);
void unmap_file(t_woody_ctx *ctx);

int dispatch_format(t_woody_ctx *ctx);
int write_woody(t_woody_ctx *ctx);

int pack_elf(t_woody_ctx *ctx);
int segment_padding(t_woody_ctx *ctx);

int woody(t_woody_ctx *ctx);

#endif
