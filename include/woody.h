#ifndef WOODY_H
#define WOODY_H

/* Includes */
#include <sys/types.h> // off_t

/* Structures */
typedef struct s_woody_ctx {
	char *progname;
	char *filename;
	off_t filesize;
	void *map;
} t_woody_ctx;

/* Prototypes */
/* parsing */
int parse_args(t_woody_ctx *ctx, int argc, char **argv);
int dispatch_format(t_woody_ctx *ctx);
/* woody */
int woody(t_woody_ctx *ctx);

#endif
