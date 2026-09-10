#include "woody.h"

#include <elf.h> // PT_LOAD, PT_X, Elf64_Ehdr, Elf64_Phdr
#include <stdint.h> // uint64_t
#include <stdio.h> // stderr, fprintf()
#include <string.h> // memcpy()

#define OEP_MARKER 0x4141414141414141ULL
#define ENC_MARKER 0x4242424242424242ULL
#define SIZE_MARKER 0x4343434343434343ULL
#define KEY_MARKER 0x4444444444444444ULL
#define KEY_SIZE_MARKER 0x4545454545454545ULL

#define IN_BOUNDS(filesize, size, offset) \
	((uint64_t)(offset) <= (uint64_t)(filesize) && \
	 (uint64_t)(size) <= (uint64_t)(filesize) - (uint64_t)(offset))

unsigned char stub_bin[] = {
	0x48, 0x8d, 0x1d, 0xf9, 0xff, 0xff, 0xff, 0x52,
	0xb8, 0x01, 0x00, 0x00, 0x00, 0xbf, 0x01, 0x00,
	0x00, 0x00, 0x48, 0x8d, 0xb3, 0x93, 0x00, 0x00,
	0x00, 0xba, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x05,
	0x5a, 0x48, 0x8b, 0x05, 0x4e, 0x00, 0x00, 0x00,
	0x48, 0x01, 0xd8, 0x48, 0x8b, 0x0d, 0x4c, 0x00,
	0x00, 0x00, 0x48, 0x8d, 0x35, 0x4d, 0x00, 0x00,
	0x00, 0x4c, 0x8b, 0x15, 0x4b, 0x00, 0x00, 0x00,
	0x4d, 0x31, 0xc9, 0x48, 0x85, 0xc9, 0x74, 0x1a,
	0x46, 0x8a, 0x1c, 0x0e, 0x44, 0x30, 0x18, 0x48,
	0xff, 0xc0, 0x49, 0xff, 0xc1, 0x4d, 0x39, 0xd1,
	0x7c, 0x03, 0x4d, 0x31, 0xc9, 0x48, 0xff, 0xc9,
	0xeb, 0xe1, 0x48, 0x8b, 0x05, 0x05, 0x00, 0x00,
	0x00, 0x48, 0x01, 0xd8, 0xff, 0xe0, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x43, 0x43,
	0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x68, 0x65,
	0x6c, 0x6c, 0x6f, 0x05, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x2e, 0x2e, 0x2e, 0x2e, 0x57,
	0x4f, 0x4f, 0x44, 0x59, 0x2e, 0x2e, 0x2e, 0x2e,
	0x0a
};
unsigned int stub_bin_len = 161;

static int check_elf_header(t_woody_ctx *ctx)
{
	if ((uint64_t)ctx->filesize < (uint64_t)sizeof(Elf64_Ehdr)) {
		fprintf(stderr, "%s: Error: %s: file too small\n",
			ctx->progname, ctx->filename);
		return (1);
	}
	return (0);
}

static int check_phdr_table(t_woody_ctx *ctx, Elf64_Ehdr *ehdr)
{
	uint64_t size;

	if (!(ehdr->e_phnum && ehdr->e_phoff)) {
		fprintf(stderr, "%s: Error: %s: no program header\n",
		        ctx->progname, ctx->filename);
		return (1);
	}

	size = (uint64_t)ehdr->e_phnum * sizeof(Elf64_Phdr); // Pq cast ici ?
	if (!IN_BOUNDS(ctx->filesize, size, ehdr->e_phoff)) {
		fprintf(stderr, "%s: Error: %s: file too small\n",
		        ctx->progname, ctx->filename);
		return (1);
	}

	return (0);
}

static int find_exec_segment(Elf64_Ehdr *ehdr, Elf64_Phdr *phdr) // int32_t ??
{
	int i;

	for (i = 0; i < ehdr->e_phnum; i++) {
		if (phdr[i].p_type == PT_LOAD && (phdr[i].p_flags & PF_X))
			return (i);
	}
	return (-1);
}

static int get_code_cave(t_woody_ctx *ctx, Elf64_Phdr *phdr, int index,
                         uint64_t *cave_offset, uint64_t *cave_vaddr)
{
	uint64_t cave_size;

	if (phdr[index].p_offset > (uint64_t)ctx->filesize ||
            phdr[index].p_filesz > (uint64_t)ctx->filesize - phdr[index].p_offset) {
		fprintf(stderr, "%s: Error: %s: corrupted segment\n",
		        ctx->progname, ctx->filename);
		return (1);
	}

	*cave_offset = phdr[index].p_offset + phdr[index].p_filesz;
	*cave_vaddr = phdr[index].p_vaddr + phdr[index].p_filesz;

	if (phdr[index + 1].p_offset < *cave_offset) { // is phdr[index + 1] > phdr[index] ?
		fprintf(stderr, "%s: Error: %s: corrupted segment layout\n",
		        ctx->progname, ctx->filename);
		return (1);
	}

	cave_size = phdr[index + 1].p_offset - *cave_offset;

	if (cave_size < stub_bin_len) {
		fprintf(stderr, "%s: Error: %s: code cave too small (%lu < %u)\n",
		        ctx->progname, ctx->filename, cave_size, stub_bin_len);
		return (1);
	}

	if (!IN_BOUNDS(ctx->filesize, stub_bin_len, *cave_offset)) {
		fprintf(stderr, "%s: Error: %s: cave write out of bounds\n",
		        ctx->progname, ctx->filename);
		return (1);
	}

	return (0);
}

static int patch_key(unsigned char *payload, size_t payload_len,
	const unsigned char *key, size_t key_size)
{
	size_t i;
	uint64_t marker;

	if (key_size > MAX_KEY_SIZE)
		return (1);
	for (i = 0; i + sizeof(marker) + MAX_KEY_SIZE <= payload_len; i++)
	{
		memcpy(&marker, payload + i, sizeof(marker));
		if (marker == KEY_MARKER) {
			memcpy(payload + i, key, key_size);
			return (0);
		}
	}
	return (1);
}

static int patch_marker(unsigned char *payload, size_t len, uint64_t marker,
                        uint64_t value)
{
	size_t j;
	uint64_t current;

	for (j = 0; j + sizeof(marker) <= len; j++) {
		memcpy(&current, payload + j, sizeof(current));
		if (current == marker) {
			memcpy(payload + j, &value, sizeof(value));
			return (0);
		}
	}
	return (1);
}

static int patch_stub(t_woody_ctx *ctx, Elf64_Ehdr *ehdr, uint64_t cave_vaddr,
                      uint64_t enc_vaddr, uint64_t enc_size,
                      unsigned char *payload)
{
	uint64_t oep, enc;

	memcpy(payload, stub_bin, stub_bin_len);

	oep = ehdr->e_entry - cave_vaddr;
	if (patch_marker(payload, stub_bin_len, OEP_MARKER, oep)) {
		fprintf(stderr, "%s: Error: %s: invalid stub (oep)\n",
			ctx->progname, ctx->filename);
		return (1);
	}

	enc = enc_vaddr - cave_vaddr;
	if (patch_marker(payload, stub_bin_len, ENC_MARKER, enc)) {
		fprintf(stderr, "%s: Error: %s: invalid stub (enc)\n",
			ctx->progname, ctx->filename);
		return (1);
	}

	if (patch_marker(payload, stub_bin_len, SIZE_MARKER, enc_size)) {
		fprintf(stderr, "%s: Error: %s: invalid stub (size)\n",
			ctx->progname, ctx->filename);
		return (1);
	}

	if (patch_key(payload, stub_bin_len, ctx->key, ctx->key_size)) {
		fprintf(stderr, "%s: Error: %s: invalid stub (key)\n",
			ctx->progname, ctx->filename);
		return (1);
	}

	if (patch_marker(payload, stub_bin_len, KEY_SIZE_MARKER, ctx->key_size)) {
		fprintf(stderr, "%s: Error: %s: invalid stub (key_size)\n",
			ctx->progname, ctx->filename);
		return (1);
	}

	return (0);
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
	phdr[index].p_flags |= PF_W;
}

static void encrypt_segment(unsigned char *map, uint64_t offset,
                             uint64_t size, const unsigned char *key,
                             size_t key_len)
{
	uint64_t k;

	for (k = 0; k < size; k++)
		map[offset + k] ^= key[k % key_len];
}

int segment_padding(t_woody_ctx *ctx)
{
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	int index;
	uint64_t cave_offset, cave_vaddr;
	uint64_t enc_vaddr, enc_size;
	unsigned char payload[stub_bin_len];

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
		return (1);
	}

	if (get_code_cave(ctx, phdr, index, &cave_offset, &cave_vaddr))
		return (1);

	enc_vaddr = phdr[index].p_vaddr;
	enc_size  = phdr[index].p_filesz;

	encrypt_segment(ctx->map, phdr[index].p_offset, enc_size, ctx->key, ctx->key_size);

	if (patch_stub(ctx, ehdr, cave_vaddr, enc_vaddr, enc_size, payload))
		return (1);

	write_stub(ctx, cave_offset, payload);
	update_elf(ehdr, phdr, index, cave_vaddr);

	return (0);
}
