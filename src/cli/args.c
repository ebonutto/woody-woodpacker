#include "woody.h"

#include <stdio.h> // stderr, fprintf()
#include <string.h> // strcmp()

static int parse_flag(t_woody_ctx *ctx, int *i, int argc, char **argv)
{
	// const char *flag = argv[*i];

	// if (strcmp(flag, "-c") == 0 || strcmp(flag, "--cipher") == 0) {
	// 	if (*i + 1 >= argc) {
	// 		fprintf(stderr, "%s: Error: missing argument after %s\n",
	// 		        ctx->progname);
	// 		return (1);
	// 	}
	// 	(*i)++;
	// 	if (parse_cipher(ctx, argv[*i]) != 0)
	// 		return (1);
	// } else if (strcmp(flag, "-z") == 0 || strcmp(flag, "--compression") == 0) {
	// 	if (*i + 1 >= argc) {
	// 		fprintf(stderr, "%s: Error: missing argument after %s\n",
	// 		        ctx->progname);
	// 		return (1);
	// 	}
	// 	(*i)++;
	// 	if (parse_cipher(ctx, argv[*i]) != 0)
	// 		return (1);
	// }
	(void)i;
	(void)ctx;
	(void)argc;
	(void)argv;
	return (0);
}

int parse_args(t_woody_ctx *ctx, int argc, char **argv)
{
	ctx->progname = argv[0];

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-')
			parse_flag(ctx, &i, argc, argv);
		else
			ctx->filename = argv[i];
		i++;
	}

	if (!ctx->filename) {
		fprintf(stderr, "%s: Error: missing filename\n",
		        ctx->progname);
		return (1);
	}

	return (0);
}
