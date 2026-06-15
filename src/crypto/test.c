//void RC4_set_key(RC4_KEY *key, int len, const unsigned char *data)
//{
//    register RC4_INT tmp;
//    register int id1, id2;
//    register RC4_INT *d;
//    unsigned int i;

//    d = &(key->data[0]);
//    key->x = 0;
//    key->y = 0;
//    id1 = id2 = 0;

//#define SK_LOOP(d, n)                         
//    {                                         
//        tmp = d[(n)];                         
//        id2 = (data[id1] + tmp + id2) & 0xff; 
//        if (++id1 == len)                     
//            id1 = 0;                          
//        d[(n)] = d[id2];                      
//        d[id2] = tmp;                         
//    }

//    for (i = 0; i < 256; i++)
//        d[i] = i;
//    for (i = 0; i < 256; i += 4) {
//        SK_LOOP(d, i + 0);
//        SK_LOOP(d, i + 1);
//        SK_LOOP(d, i + 2);
//        SK_LOOP(d, i + 3);
//    }
//}

//static int rc4_init(t_rc4_ctx *ctx, const uint8_t *key, const size_t len)
//{
//	uint8_t *S;
//	size_t  i;
//	uint8_t j, tmp;
//	size_t  k;

//	if (!ctx || !key || len == 0)
//		return (0);

//	S = ctx->S;
//	ctx->x = 0;
//	ctx->y = 0;

//	for (i = 0; i < 256; i++)
//		S[i] = i;

//	for (i = j = k = 0; i < 256; i++) {
//		tmp = S[i];
//		j += tmp + key[k];
//		if (++k == len)
//			k = 0;
//		S[i] = S[j];
//		S[j] = tmp;
//	}

//	return (1);
//}


//int main(void)
//{
//	register RC4_INT tmp;
//	register int id1, id2;
//	register RC4_INT *d;
//	unsigned int i;

//	d = &(key->data[0]);
//	key->x = 0;
//	key->y = 0;
//	id1 = id2 = 0;

//#define SK_LOOP(d, n)                         
//	{                                         
//		tmp = d[(n)];                         
//		id2 = (data[id1] + tmp + id2) & 0xff; 
//		if (++id1 == len)                     
//		id1 = 0;                          
//		d[(n)] = d[id2];                      
//		d[id2] = tmp;                         
//	}

//	for (i = 0; i < 256; i++)
//		d[i] = i;
//	for (i = 0; i < 256; i += 4) {
//		SK_LOOP(d, i + 0);
//		SK_LOOP(d, i + 1);
//		SK_LOOP(d, i + 2);
//		SK_LOOP(d, i + 3);
//	}
//	return (0);
//}
