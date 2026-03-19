#include <stdio.h>
#include <fcntl.h>  // open()


/*
 * Functions
 */

//int read_file(const char *filename)
//{
	
//	return (1);
//}

#include <sys/syscall.h>  // stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "packer.h"

void read_file(t_packer *ctx)
{
	const char  *prog_name = ctx->prog_name;
	const char  *filename = ctx->filename;
	int         fd;
	struct stat st;

	/* Open file, if not quit */
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror("filename");
		exit(1);
	}

	/* Init stat */
	if (stat(filename, &st) < 0) {
		perror("stat() failed");
		close(fd);
		exit(1);
	}

	/* Check if filename is a directory */
	if (!S_ISREG(st.st_mode)) {
		fprintf(stderr, "%s: Error: %s is not a regular file !\n", prog_name, filename);
		close(fd);
		exit(1);
	}

	ctx->fsize = st.st_size;

	malloc(sizeof(char) * ctx->fsize);
}


int main(int argc, char **argv)
{

	if (argc < 2) {
		fprintf(stderr, "%s: Error: no file to pack !\n", argv[0]);
		return (1);
	}

	/* Init context */
	t_packer ctx = {argv[0], argv[1], 0};

	read_file(&ctx);

	return (0);
}
