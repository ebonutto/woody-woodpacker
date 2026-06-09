#include "woody.h"

int main(int argc, char **argv)
{
	t_woody_ctx ctx;

	woody_init(&ctx);
	if (parse_args(&ctx, argc, argv) != 0)
		return (1);
	return (woody_run(&ctx));
}
