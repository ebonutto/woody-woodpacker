#include "woody.h"

#include <sys/mman.h> // MAP_PRIVATE, PROT_READ, PROT_WRITE, mmap()
#include <sys/stat.h> // struct stat, stat()
#include <unistd.h> // close();

#include <fcntl.h> // O_RDONLY, open()
#include <stdio.h> // stderr, fprintf(), perror()

int woody_setup(t_woody_ctx *ctx)
{
	int fd;
	struct stat st;

	fd = open(ctx->filename, O_RDONLY);
	if (fd < 0) {
		perror(ctx->filename);
		return (1);
	}
	if (stat(ctx->filename, &st) < 0) {
		perror("stat()");
		close(fd);
		return (1);
	}
	if (!S_ISREG(st.st_mode)) {
		fprintf(stderr, "%s: Error: %s is not a regular file !\n", ctx->progname, ctx->filename);
		close(fd);
		return (1);
	}
	ctx->map = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (!ctx->map) {
		close(fd);
		return (1);
	}
	ctx->filesize = st.st_size;
	close(fd);
	return (0);
}
