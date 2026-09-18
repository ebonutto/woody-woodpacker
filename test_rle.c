#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 65536

static size_t rle_compress(const unsigned char *src, size_t src_len,
                            unsigned char *dst)
{
	size_t i, out;
	unsigned char run;

	i = 0;
	out = 0;
	while (i < src_len) {
		run = 1;
		while (i + run < src_len && src[i + run] == src[i] && run < 255)
			run++;
		dst[out++] = src[i];
		dst[out++] = run;
		i += run;
	}
	return (out);
}

static unsigned char *read_file(const char *path, size_t *out_len)
{
	int fd;
	unsigned char *buf;
	size_t capacity;
	size_t len;
	ssize_t n;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		perror("open");
		return (NULL);
	}

	capacity = BUF_SIZE;
	len = 0;
	buf = malloc(capacity);
	if (!buf) {
		close(fd);
		return (NULL);
	}

	while (1) {
		if (len + BUF_SIZE > capacity) {
			capacity *= 2;
			unsigned char *tmp = realloc(buf, capacity);
			if (!tmp) {
				free(buf);
				close(fd);
				return (NULL);
			}
			buf = tmp;
		}
		n = read(fd, buf + len, BUF_SIZE);
		if (n < 0) {
			perror("read");
			free(buf);
			close(fd);
			return (NULL);
		}
		if (n == 0)
			break;
		len += (size_t)n;
	}

	close(fd);
	*out_len = len;
	return (buf);
}

int main(int argc, char **argv)
{
	unsigned char *original;
	unsigned char *compressed;
	size_t original_len;
	size_t compressed_len;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return (1);
	}

	original = read_file(argv[1], &original_len);
	if (!original)
		return (1);

	if (original_len == 0) {
		fprintf(stderr, "empty file\n");
		free(original);
		return (1);
	}

	/* Pire cas RLE : 2x la taille source (aucune répétition). */
	compressed = malloc(original_len * 2);
	if (!compressed) {
		free(original);
		return (1);
	}

	compressed_len = rle_compress(original, original_len, compressed);

	printf("original:   %zu octets\n", original_len);
	printf("compressed: %zu octets\n", compressed_len);
	printf("ratio:      %.2f%% de la taille originale\n",
	       100.0 * (double)compressed_len / (double)original_len);

	if (compressed_len < original_len)
		printf("=> RLE gagnant sur ce fichier\n");
	else
		printf("=> RLE PERDANT sur ce fichier (le résultat est plus gros)\n");

	free(original);
	free(compressed);
	return (0);
}
