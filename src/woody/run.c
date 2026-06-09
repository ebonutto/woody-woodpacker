#include "woody.h"

int woody_run(t_woody_ctx *ctx)
{
	if (woody_setup(ctx))
		return (1);
	woody_cleanup(ctx);
	return (0);
}
