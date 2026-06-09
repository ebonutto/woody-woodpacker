#include "woody.h"

#include <stdio.h> // stderr, fprintf()

static int parse_flag(t_woody_ctx *ctx, int argc, char **argv)
{
	(void)ctx;
	(void)argc;
	(void)argv;
	return (0);
}

int parse_args(t_woody_ctx *ctx, int argc, char **argv)
{
	int i;

	ctx->progname = argv[0];
	i = 1;
	while (i < argc) {
		if (argv[i][0] == '-')
			parse_flag(ctx, argc, argv);
		else
			ctx->filename = argv[i];
		i++;
	}
	if (!ctx->filename) {
		fprintf(stderr, "%s: Error: missing filename\n", ctx->progname);
		return (1);
	}
	return (0);
}
