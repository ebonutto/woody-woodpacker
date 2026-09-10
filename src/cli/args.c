#include "woody.h"

#include <stdio.h> // stderr, fprintf()
#include <string.h> // strcmp(), strlen(), memcpy()

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
