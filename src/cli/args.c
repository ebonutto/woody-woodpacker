#include "woody.h"

#include <stdio.h> // stderr, fprintf()
#include <string.h> // strcmp(), strlen(), memcpy()

static int parse_pack_method(t_woody_ctx *ctx, int *i, int argc, char **argv)
{
	const char *method;

	if (++(*i) >= argc) {
		fprintf(stderr, "%s: Error: missing pack method\n", ctx->progname);
		return (1);
	}

	method = argv[*i];

	if (strcmp(method, "code_cave_injection") == 0) {
		ctx->pack_method = CODE_CAVE_INJECTION;
		return (0);
	}

	fprintf(stderr, "%s: Error: invalid pack method: %s\n",
	        ctx->progname, method);
	return (1);
}

static int parse_compression(t_woody_ctx *ctx, int *i, int argc, char **argv)
{
	const char *compression;

	if (++(*i) >= argc) {
		fprintf(stderr, "%s: Error: missing compression\n", ctx->progname);
		return (1);
	}

	compression = argv[*i];

	if (strcmp(compression, "none") == 0) {
		ctx->compression = COMPRESSION_NONE;
		return (0);
	}

	if (strcmp(compression, "rle") == 0) {
		ctx->compression = COMPRESSION_RLE;
		return (0);
	}

	fprintf(stderr, "%s: Error: invalid compression: %s\n",
	        ctx->progname, compression);
	return (1);
}

static int parse_cipher(t_woody_ctx *ctx, int *i, int argc, char **argv)
{
	const char *cipher;

	if (++(*i) >= argc) {
		fprintf(stderr, "%s: Error: missing cipher\n", ctx->progname);
		return (1);
	}

	cipher = argv[*i];

	if (strcmp(cipher, "none") == 0) {
		ctx->cipher = CIPHER_NONE;
		return (0);
	}

	if (strcmp(cipher, "xor") == 0) {
		ctx->cipher = CIPHER_XOR;
		return (0);
	}

	if (strcmp(cipher, "rc4") == 0) {
		ctx->cipher = CIPHER_RC4;
		return (0);
	}

	fprintf(stderr, "%s: Error: invalid cipher: %s\n",
	        ctx->progname, cipher);
	return (1);
}

static int parse_key(t_woody_ctx *ctx, int *i, int argc, char **argv)
{
	const char *key;

	if (++(*i) >= argc) {
		fprintf(stderr, "%s: Error: missing key\n", ctx->progname);
		return (1);
	}

	if (ctx->key_size) {
		fprintf(stderr, "%s: Error: multiple keys\n", ctx->progname);
		return (1);
	}

	key = argv[*i];
	ctx->key_size = strlen(key);
	if (ctx->key_size == 0 || ctx->key_size > MAX_KEY_SIZE) {
		fprintf(stderr, "%s: Error: invalid key size (1-256 bytes)\n",
		        ctx->progname);
		return (1);
	}

	memcpy(ctx->key, key, ctx->key_size);
	return (0);
}

static int parse_option(t_woody_ctx *ctx, int *i, int argc, char **argv)
{
	const char *option = argv[*i];

	if (strcmp(option, "-p") == 0 || strcmp(option, "--pack-method") == 0)
		return (parse_pack_method(ctx, i, argc, argv));

	if (strcmp(option, "-z") == 0 || strcmp(option, "--compression") == 0)
		return (parse_compression(ctx, i, argc, argv));

	if (strcmp(option, "-c") == 0 || strcmp(option, "--cipher") == 0)
		return (parse_cipher(ctx, i, argc, argv));

	if (strcmp(option, "-k") == 0 || strcmp(option, "--key") == 0)
		return (parse_key(ctx, i, argc, argv));

	fprintf(stderr, "%s: Error: invalid option: %s\n",
	        ctx->progname, option);
	return (1);
}

int parse_args(t_woody_ctx *ctx, int argc, char **argv)
{
	ctx->progname = argv[0];

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (parse_option(ctx, &i, argc, argv))
				return (1);
		} else if (ctx->filename) {
			fprintf(stderr, "%s: Error: multiple filenames\n",
			        ctx->progname);
			return (1);
		} else
			ctx->filename = argv[i];
	}

	if (!ctx->filename) {
		fprintf(stderr, "%s: Error: missing filename\n",
		        ctx->progname);
		return (1);
	}
	return (0);
}
