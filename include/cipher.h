#ifndef CIPHER_H
#define CIPHER_H

#include <stddef.h> // size_t

void xor(unsigned char *buf, size_t len, const unsigned char *key, size_t key_len);

#endif
