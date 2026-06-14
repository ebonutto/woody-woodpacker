#include "woody.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>

int write_output(t_woody_ctx *ctx)
{
	int fd;

	printf("Hummm\n");
	fd = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (fd < 0) {
		perror("open woody");
		return (1);
	}
	if (write(fd, ctx->map, ctx->filesize) != (ssize_t)ctx->filesize) {
		perror("write");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int pack_elf(t_woody_ctx *ctx)
{
	printf("heyy\n");
	segment_padding(ctx);
	write_output(ctx);
	munmap(ctx->map, ctx->filesize);
	return (0);
}
