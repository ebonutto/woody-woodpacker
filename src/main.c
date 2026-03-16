#include <unistd.h> // syscall
#include <sys/syscall.h> // SYS_write

#include <stdio.h>  // fprintf(), stderr
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


/*
 * Functions
 */

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "%s: missing file operand\n", argv[0]);
		return (1);
	}

	const char *filename = argv[1];
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror(filename);
		return (1)
	}
	

	return (0);
}
