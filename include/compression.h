#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stddef.h> // size_t

size_t rle_compress(unsigned char *dst, const unsigned char *src, size_t len);

#endif
