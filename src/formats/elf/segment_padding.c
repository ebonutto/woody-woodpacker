#include "woody.h"

#include <elf.h> // Elf64_Ehdr, Elf64_Phdr
#include <stdio.h>
#include <string.h>
#define IN_BOUNDS(filesize, offset, size) \
	((off_t)(offset) <= filesize && (off_t)(size) <= filesize - (off_t)(offset))

unsigned char stub_bin[] = {
  0xe8, 0x00, 0x00, 0x00, 0x00, 0x5b, 0xb8, 0x01, 0x00, 0x00, 0x00, 0xbf,
  0x01, 0x00, 0x00, 0x00, 0x48, 0x8d, 0x73, 0x20, 0xba, 0x0d, 0x00, 0x00,
  0x00, 0x0f, 0x05, 0xb8, 0x3c, 0x00, 0x00, 0x00, 0x48, 0x31, 0xff, 0x0f,
  0x05, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x57, 0x6f, 0x72, 0x6c,
  0x64, 0x0a
};

unsigned int stub_bin_len = 50;


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
			printf("X\n");
			if (phdr[i].p_flags & PF_W)
				printf("W\n");
			if (phdr[i].p_flags & PF_R)
				printf("R\n");
			break;
		}
	}
	if (i + 1 >= ehdr->e_phnum)
		return (1);

	uint64_t cave_size = phdr[i + 1].p_offset - (phdr[i].p_offset + phdr[i].p_filesz);
	uint64_t cave_vaddr = phdr[i].p_vaddr + phdr[i].p_filesz;
	uint64_t cave_offset = phdr[i].p_offset + phdr[i].p_filesz;

	printf("%lu\n", phdr[i + 1].p_offset);
	printf("%lu\n", cave_size);

	ctx->oep = ehdr->e_entry; // size_t -> uintN_t

	printf("entry = %ld, vrtual = %ld, cave = %ld\n", ehdr->e_entry, phdr->p_vaddr, cave_vaddr);
	memcpy(ctx->map + cave_offset, stub_bin, stub_bin_len);

	ehdr->e_entry = cave_vaddr;

	phdr[i].p_filesz += stub_bin_len;
	phdr[i].p_memsz  += stub_bin_len;

	return (0);
}
