#include <stdio.h>
#include <fcntl.h>  // open()

#include <sys/syscall.h>  // stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "packer.h"
#include <sys/mman.h>

void init_packer_ctx(s_packer_ctx *packer_ctx)
{
	int         fd;
	struct stat st;

	fd = open(packer_ctx->args.file_name, O_RDONLY);
	if (fd < 0) {
		perror(packer_ctx->args.file_name);
		exit(1);
	}

	if (stat(packer_ctx->args.file_name, &st) < 0) {
		perror("stat() failed");
		close(fd);
		exit(1);
	}

	if (!S_ISREG(st.st_mode)) {
		fprintf(stderr, "%s: Error: %s is not a regular file !\n", packer_ctx->args.prog_name, packer_ctx->args.file_name);
		close(fd);
		exit(1);
	}

	packer_ctx->map = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (!packer_ctx->map) {
		close(fd);
		exit(1);
	}

	packer_ctx->file_size = st.st_size;
	close(fd);
}

void free_packer_ctx(s_packer_ctx *packer_ctx)
{
	munmap(packer_ctx->map, packer_ctx->file_size);
}

#include "args.h"
int main(int argc, char **argv)
{
	s_packer_ctx packer_ctx;

	packer_ctx.args = parse_args(argc, argv);

	init_packer_ctx(&packer_ctx);
	//woody_woodpacker(&ctx);

	//pack(&ctx);

	free_packer_ctx(&packer_ctx);
	return (0);
}
