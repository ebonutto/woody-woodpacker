#include "woody.h"

#include <elf.h> // Elf64_Ehdr, Elf64_Phdr
#include <stdio.h>
#include <string.h>
#define IN_BOUNDS(filesize, offset, size) \
	((off_t)(offset) <= filesize && (off_t)(size) <= filesize - (off_t)(offset))

unsigned char hello_bin[] = {
  0xe8, 0x00, 0x00, 0x00, 0x00, 0x5b, 0xb8, 0x01, 0x00, 0x00, 0x00, 0xbf,
  0x01, 0x00, 0x00, 0x00, 0x48, 0x8d, 0x73, 0x1f, 0xba, 0x0e, 0x00, 0x00,
  0x00, 0x0f, 0x05, 0x48, 0xc7, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
  0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64,
  0x21, 0x0a
};
unsigned int hello_bin_len = 50;

int segment_padding(t_woody_ctx *ctx)
{
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;

	if (!IN_BOUNDS(ctx->filesize, 0, sizeof(Elf64_Ehdr))) {
		fprintf(stderr, "%s: Error: %s: file too small\n",
		        ctx->progname, ctx->filename);
		return (1);
	}
	ehdr = (Elf64_Ehdr *)ctx->map;
	if (ehdr->e_phoff && !IN_BOUNDS(ctx->filesize, ehdr->e_phoff, ehdr->e_phnum * sizeof(Elf64_Phdr))) { //! * ehdr->e_phoff can be 0 and is too ehdr->e_phnum
		fprintf(stderr, "%s: Error: %s: file too small\n",
		        ctx->progname, ctx->filename);
		return (1);
	}
	phdr = (Elf64_Phdr *)(ctx->map + ehdr->e_phoff);
	int i;
	for (i = 0; i < ehdr->e_phnum; i++) {
		if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags & PF_X) {
			break ;
		}
	}
	if (i + 1 >= ehdr->e_phnum)
		return (1);

	printf("i = %d\n", i);

	uint64_t cave_size = phdr[i + 1].p_offset - (phdr[i].p_offset + phdr[i].p_filesz);
	uint64_t cave_offset = phdr[i].p_offset + phdr[i].p_filesz;
	uint64_t cave_vaddr = cave_offset - phdr[i].p_offset + phdr[i].p_vaddr;

	printf("0 - e_entry = %lu\n", ehdr->e_entry);
	printf("1 - p_offset = %lu\n", phdr[i].p_offset);
	printf("2 - p_vaddr = %lu\n", phdr[i].p_vaddr);
	printf("3 - cave_size = %lu\n", cave_size);
	printf("4 - cave_offset = %lu\n", cave_offset);
	printf("5 - cave_vaddr = %lu\n", cave_vaddr);

	unsigned char payload[hello_bin_len];
	memcpy(payload, hello_bin, hello_bin_len);

	// Patch OEP
	uint64_t    oep_marker;
	size_t      j;
	oep_marker = 0xFFFFFFFFFFFFFFFF;
	j = 0;
	while (j + 8 <= hello_bin_len) {
		if (*(uint64_t *)(payload + j) == oep_marker) {
			*(uint64_t *)(payload + j) =  ehdr->e_entry;
			break;
		}
		j++;
	}

	// Injection
	memcpy(ctx->map + cave_offset, payload, hello_bin_len);

	ehdr->e_entry = cave_vaddr;
	phdr[i].p_filesz += hello_bin_len;
	phdr[i].p_memsz  += hello_bin_len;

	return (0);
}
