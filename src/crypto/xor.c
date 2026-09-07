#include <stddef.h> // size_t

void xor_rolling(unsigned char *buf, size_t len, const unsigned char *key, size_t key_len)
{
	for (size_t i = 0; i < len; i++)
		buf[i] ^= key[i % key_len];
}
