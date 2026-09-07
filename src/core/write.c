#include "woody.h"

#include <sys/types.h> // ssize_t

#include <fcntl.h> // O_CREAT, O_TRUNC, O_WRONLY, open()
#include <stdio.h> // perror()
#include <stddef.h> // size_t
#include <unistd.h> // close(), write()

static int write_all(int fd, const void *buf, size_t len)
{
	const unsigned char *p;
	ssize_t w;

	p = buf;
	while (len > 0) {
		w = write(fd, p, len);
		if (w < 0) {
			perror("write()");
			return (1);
		}
		p += w;
		len -= (size_t)w;
	}
	return (0);
}

int write_woody(t_woody_ctx *ctx)
{
	int fd;
	int ret;

	fd = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (fd < 0) {
		perror("open()");
		return (1);
	}

	ret = write_all(fd, ctx->map, (size_t)ctx->filesize);

	if (close(fd) != 0 && ret == 0) {
		perror("close()");
		ret = 1;
	}
	return (ret);
}
