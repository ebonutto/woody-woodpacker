#include "args.h"
#include <stdlib.h>  // stdlib()
#include <stdio.h>

static void set_flag(long *flags, long flag)
{
	*flags |= (1 << flag);
}

static void parse_flag(s_args *args, const char *arg)
{
	if (arg[1] == 'h')
		set_flag(&(args->flags), FLAG_H);
}

static void parse_file(s_args *args, const char *arg)
{
	args->file_name = arg;
}

s_args parse_args(int argc, char **argv)
{
	int    i;
	char   *arg;
	s_args args;

	args.prog_name = argv[0];

	if (argc < 2) {
		fprintf(stderr, "%s: Error: no file to pack !\n", args.prog_name);
		exit(1);
	}

	i = 0;
	while (i < argc) {
		arg = argv[i];
		if (arg[0] == '-')
			parse_flag(&args, arg);
		else
			parse_file(&args, arg);

		i++;
	}

	return (args);
}
