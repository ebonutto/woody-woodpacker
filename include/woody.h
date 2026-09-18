#ifndef WOODY_H
#define WOODY_H

/* Includes */
#include <sys/types.h> // off_t

#include <stddef.h> // size_t
#include <stdint.h> // uintX_t

/* Defines */
#define MAX_KEY_SIZE 32

/* Enums */
typedef enum e_pack_method {
	CODE_CAVE_INJECTION
} t_pack_method;

typedef enum e_cipher {
	RC4,
	XOR
} t_cipher;

typedef enum e_compression {
	RLE
} t_compression;

/* Structures */
typedef struct s_woody_ctx {
	char *progname;
	char *filename;
	t_pack_method pack_method;
	t_cipher cipher;
	t_compression compression;
	unsigned char key[MAX_KEY_SIZE];
	size_t key_size;
	off_t filesize;
	void *map;
	size_t oep;
} t_woody_ctx;

/* Prototypes */
void encrypt_segment(t_woody_ctx *ctx, uint64_t offset, uint64_t size);
void xor(unsigned char *buf, size_t len, const unsigned char *key, size_t key_len);

int parse_args(t_woody_ctx *ctx, int argc, char **argv);

int map_file(t_woody_ctx *ctx);
void unmap_file(t_woody_ctx *ctx);

int dispatch_format(t_woody_ctx *ctx);

int pack_elf(t_woody_ctx *ctx);
int code_cave_injection(t_woody_ctx *ctx);

int write_woody(t_woody_ctx *ctx);

void woody_init_key(t_woody_ctx *ctx);
void woody_init(t_woody_ctx *ctx);
int woody_pack(t_woody_ctx *ctx);
int pack_elf64(t_woody_ctx *ctx);

#endif
