#include "woody.h"

#include <string.h> // memset()


int main(int argc, char **argv)
{
	t_woody_ctx ctx;

	memset(&ctx, 0, sizeof(ctx));
	if (parse_args(&ctx, argc, argv) != 0)
		return (1);
	return (woody(&ctx));
}
