#ifndef ARGS_H
#define ARGS_H

/* Macros */
#define FLAG_H 0

/* Structures */
typedef struct {
	const char *prog_name;
	const char *file_name;
	long       flags;
} s_args;

/* Functions */
s_args parse_args(int argc, char **argv);

#endif
