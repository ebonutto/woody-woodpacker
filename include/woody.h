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

/* woody */
void woody_init(t_woody_ctx *ctx);
int woody_setup(t_woody_ctx *ctx);
void woody_cleanup(t_woody_ctx *ctx);
int woody_run(t_woody_ctx *ctx);

#endif
