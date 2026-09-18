#ifndef CIPHER_H
#define CIPHER_H

#include "woody.h"

#include <stddef.h> // size_t

int generate_key(t_woody_ctx *ctx);

void xor(unsigned char *buf, size_t len, const unsigned char *key,
         size_t key_len);

#endif
