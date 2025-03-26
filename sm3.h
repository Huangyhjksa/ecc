//
// Created by 24208 on 2023/11/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define SWAP32 _byteswap_ulong

#define ASSIGN_UINT32(n, data, offset)                                      \
    {                                                                       \
        put_unaligned_uint32((data) + (offset), SWAP32((uint32_t) (n)));    \
    }

#define GET_UINT32(data, offset)                                            \
    (                                                                       \
        SWAP32(get_unaligned_uint32((data) + (offset)))                     \
    )


#define SHL(x, n) (((x) & 0xffffffff) << n)
#define ROTL(x, n) (SHL((x), n) | ((x) >> (32 - n)))

#define P0(x) ((x) ^ ROTL((x), 9) ^ ROTL((x), 17))
#define P1(x) ((x) ^ ROTL((x), 15) ^ ROTL((x), 23))

#define FF0(x, y, z) ((x) ^ (y) ^ (z))
#define FF1(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define GG0(x, y, z) ((x) ^ (y) ^ (z))
#define GG1(x, y, z) (((x) & (y)) | ((~(x)) & (z)))

#define WP(t)                                                       \
    (                                                               \
        temp = W[t] ^ W[t + 4]                                      \
    )

#define F0(a, b, c, d, e, f, g, h, Tj, w, wp)                       \
    do                                                              \
    {                                                               \
        SS1 = ROTL((ROTL((a), 12) + (e) + (Tj)), 7);                \
        SS2 = SS1 ^ ROTL((a), 12);                                  \
        TT1 = FF0((a), (b), (c)) + (d) + SS2 + (wp);                \
        TT2 = GG0((e), (f), (g)) + (h) + SS1 + (w);                 \
        (d) = P0(TT2);                                              \
        (f) = ROTL((f), 19);                                        \
        (b) = ROTL((b), 9);                                         \
        (h) = TT1;                                                  \
    } while (0)

#define F1(a, b, c, d, e, f, g, h, Tj, w, wp)                       \
    do                                                              \
    {                                                               \
        SS1 = ROTL((ROTL((a), 12) + (e) + (Tj)), 7);                \
        SS2 = SS1 ^ ROTL((a), 12);                                  \
        TT1 = FF1((a), (b), (c)) + (d) + SS2 + (wp);                \
        TT2 = GG1((e), (f), (g)) + (h) + SS1 + (w);                 \
        (d) = P0(TT2);                                              \
        (f) = ROTL((f), 19);                                        \
        (b) = ROTL((b), 9);                                         \
        (h) = TT1;                                                  \
    } while (0)

typedef struct sm3_context {

    uint32_t total[2];              // The number of Bytes processed.
    uint32_t state[8];              // The intermediate digest state.
    uint8_t buffer[64];             // The data block being processed.
} sm3_context;


void sm3_encryption(uint8_t *message, uint64_t message_leagth, uint32_t *output);
void iterate_process(sm3_context *ctx, const unsigned char data[64]);

void hash_start(sm3_context *ctx);
void hash_update(sm3_context *ctx, uint8_t *input, size_t ilen);
void hash_finish(sm3_context *ctx, uint32_t *output);


//uint64_t get_message_length(char *plainaddr);
void put_unaligned_uint32(void *buffer, uint32_t data);
uint32_t get_unaligned_uint32(const void *p);

#ifndef SHA1_V3_SGA1_H
#define SHA1_V3_SGA1_H

#endif //SHA1_V3_SGA1_H
