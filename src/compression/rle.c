#include "compression.h"

#include <stddef.h> // size_t

size_t rle_compress(unsigned char *dst, const unsigned char *src, size_t len)
{
	size_t i, out;
	unsigned char run;

	i = 0;
	out = 0;
	while (i < len) {
		run = 1;
		while (i + run < len && src[i + run] == src[i] && run < 255)
			run++;
		dst[out++] = src[i];
		dst[out++] = run;
		i += run;
	}
	return (out);
}
