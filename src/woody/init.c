#include "woody.h"

#include <string.h> // memset()

void woody_init(t_woody_ctx *ctx)
{
	memset(ctx, 0, sizeof(*ctx));
}
