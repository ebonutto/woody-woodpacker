#ifndef PACKER_H
#define PACKER_H

#include <sys/types.h>

typedef struct s_packer {
	const char *prog_name;
	const char *filename;
	off_t      fsize;
	void  *file;
} t_packer;


#endif
