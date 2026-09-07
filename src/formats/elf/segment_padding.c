#include "woody.h"

#include <elf.h> // Elf64_Ehdr, Elf64_Phdr
#include <stdio.h> // stderr, fprintf()
#include <string.h>

#define IN_BOUNDS(filesize, size, offset) \
	((off_t)(offset) <= filesize && (off_t)(size) <= filesize - (off_t)(offset))

unsigned char stub_bin[] = {
0x48, 0x8d, 0x1d, 0xf9, 0xff, 0xff, 0xff, 0x52, 0xb8, 0x01, 0x00, 0x00, 0x00, 0xbf, 0x01, 0x00, 0x00, 0x00, 0x48, 0x8d, 0x73, 0x52, 0xba, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x5a, 0x48, 0x8b, 0x05, 0x05, 0x00, 0x00, 0x00, 0x48, 0x01, 0xd8, 0xff, 0xe0, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x2e, 0x2e, 0x2e, 0x2e, 0x57, 0x4f, 0x4f, 0x44, 0x59, 0x2e, 0x2e, 0x2e, 0x2e, 0x0a
};

// U"\x48\x8d\x1d\xf9\xff\xff\xff\x52\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x8d\x73\x52\xba\x0e\x00\x00\x00\x0f\x05\x5a\x48\x8b\x05\x05\x00\x00\x00\x48\x01\xd8\xff\xe0\x41\x41\x41\x41\x41\x41\x41\x41\x42\x42\x42\x42\x42\x42\x42\x42\x43\x43\x43\x43\x43\x43\x43\x43\x44\x44\x44\x44\x44\x44\x44\x44\x45\x45\x45\x45\x45\x45\x45\x45\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a"
unsigned int stub_bin_len = 96;
unsigned int stub_bin_len = sizeof(stub_bin) / sizeof(stub_bin[0]);

static int check_elf_header(t_woody_ctx *ctx)
{
	if (!IN_BOUNDS(ctx->filesize, sizeof(Elf64_Ehdr), 0)) {
		fprintf(stderr, "%s: Error: %s: file too small\n",
			ctx->progname, ctx->filename);
		return (1);
	}
	return (0);
}

static int check_phdr_table(t_woody_ctx *ctx, Elf64_Ehdr *ehdr)
{
	if (!(ehdr->e_phoff && ehdr->e_phnum)) {
		fprintf(stderr, "%s: Error: %s: no program headers\n",
		        ctx->progname, ctx->filename);
		return (1);
	}
	if (!IN_BOUNDS(ctx->filesize, (uint64_t)ehdr->e_phnum * sizeof(Elf64_Phdr), ehdr->e_phoff)) {
		fprintf(stderr, "%s: Error: %s: file too small\n",
		        ctx->progname, ctx->filename);
		return (1);
	}
	return (0);
}

static int find_exec_segment(Elf64_Ehdr *ehdr, Elf64_Phdr *phdr)
{
	for (int i = 0; i < ehdr->e_phnum; i++) {
		if (phdr[i].p_type == PT_LOAD && (phdr[i].p_flags & PF_X))
			return (i);
	}
	return (-1);
}

static int get_code_cave(t_woody_ctx *ctx, Elf64_Phdr *phdr, int index,
                         uint64_t *cave_offset, uint64_t *cave_vaddr,
                         uint64_t *cave_size)
{
	if (phdr[i].p_offset > (uint64_t)ctx->filesize || phdr[i].p_filesz > (uint64_t)ctx->filesize - phdr[i].p_offset) {
		fprintf(stderr, "%s: Error: %s: corrupted segment\n",
		        ctx->progname, ctx->filename);
		return (1);
	}

	uint64_t cave_offset, cave_vaddr;

	cave_offset = phdr[i].p_offset + phdr[i].p_filesz;
	cave_vaddr = phdr[i].p_vaddr + phdr[i].p_filesz;

	if (phdr[i + 1].p_offset < cave_offset) {
		fprintf(stderr, "%s: Error: %s: corrupted segment layout\n",
		        ctx->progname, ctx->filename);
		return (1);
	}

	uint64_t cave_size = phdr[i + 1].p_offset - cave_offset;

	if (cave_size < stub_bin_len) {
		fprintf(stderr, "%s: Error: %s: code cave too small (%lu < %u)\n",
		        ctx->progname, ctx->filename, cave_size, stub_bin_len);
		return (1);
	}

	if (!IN_BOUNDS(ctx->filesize, stub_bin_len, cave_offset)) {
		fprintf(stderr, "%s: Error: %s: cave write out of bounds\n",
		        ctx->progname, ctx->filename);
		return (1);
	}
}

static void write_stub(t_woody_ctx *ctx, uint64_t cave_offset,
                       unsigned char *payload)
{
	memcpy(ctx->map + cave_offset, payload, stub_bin_len);
}

static void update_elf(Elf64_Ehdr *ehdr, Elf64_Phdr *phdr, int index,
                       uint64_t cave_vaddr)
{
	ehdr->e_entry = cave_vaddr;
	phdr[index].p_filesz += stub_bin_len;
	phdr[index].p_memsz += stub_bin_len;
}

int segment_padding(t_woody_ctx *ctx)
{
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	int index;
	unsigned char payload[stub_bin_len];
	uint64_t cave_offset, cave_vaddr, cave_size;

	if (check_elf_header(ctx))
		return (1);

	ehdr = (Elf64_Ehdr *)ctx->map;

	if (check_phdr_table(ctx, ehdr))
		return (1);

	phdr = (Elf64_Phdr *)(ctx->map + ehdr->e_phoff);

	index = find_exec_segment(ehdr, phdr);
	if (index < 0) {
		fprintf(stderr, "%s: Error: %s: no usable code cave found\n",
		        ctx->progname, ctx->filename);
	}

	if (get_code_cave(ctx, phdr, index, &cave_offset, &cave_vaddr,
                          &cave_size))
		return (1);

	unsigned char payload[stub_bin_len];
	memcpy(payload, stub_bin, stub_bin_len);

	uint64_t oep_marker = 0x4141414141414141ULL;
	size_t j = 0;
	int found = 0;

	while (j + 8 <= stub_bin_len) {
		if (*(uint64_t *)(payload + j) == oep_marker) {
		    *(uint64_t *)(payload + j) = ehdr->e_entry - cave_vaddr;
			found = 1;
			break;
		}
		j++;
	}

	if (!found)
		return (1);

	write_stub(ctx, cave_offset, payload);
	update_elf(ehdr, phdr, index, cave_vaddr);

	return (0);
}
