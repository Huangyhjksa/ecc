//
// Created by 24208 on 2023/11/17.
//

#include "sm3.h"

void sm3_encryption(uint8_t *message, uint64_t message_length, uint32_t *output) {

//    FILE *fp = NULL;
//    if ((fp = fopen(inputfileaddr, "rb")) == NULL) {
//
//        printf("open %s faied!\n", inputfileaddr);
//        system("PAUSE");
//        return;
//    }
//
//    uint8_t message[BUFFER_SIZE];
//    memset(message, 0x00, sizeof (message));
//    char *ptr = (char *)message;
//    while (fread(ptr, sizeof (char), 64, fp)) {
//        ptr += 64;
//    }

    sm3_context ctx;
    memset(&ctx, 0, sizeof(sm3_context));

//    clock_t start, end;
//    start = clock();
//    int rounds = 999999;
//    for (int i = 0; i < rounds; i++)
//    {
//        hash_start(&ctx);
//        hash_update(&ctx, message, message_length);
//        hash_finish(&ctx, output);
//    }
//
//    end = clock();
//    printf("v = %f Mbps\n", ((double)message_length * 8 * rounds / 1000) / ((double)(end - start)));
//    printf("time = %f ms\n", ((double)(end - start)));

    hash_start(&ctx);
    hash_update(&ctx, message, message_length);
    hash_finish(&ctx, output);

//    fclose(fp);

}


void hash_start(sm3_context *ctx) {

    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x7380166f;
    ctx->state[1] = 0x4914b2b9;
    ctx->state[2] = 0x172442d7;
    ctx->state[3] = 0xda8a0600;
    ctx->state[4] = 0xa96f30bc;
    ctx->state[5] = 0x163138aa;
    ctx->state[6] = 0xe38dee4d;
    ctx->state[7] = 0xb0fb0e4e;

}

void hash_update(sm3_context *ctx, uint8_t *input, size_t ilen) {

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xffffffff;

    if (ctx->total[0] < (uint32_t) ilen) {
        ctx->total[1]++;
    }

    while (ilen >= 64) {
        memcpy((void *)(ctx->buffer), input, 64);
        iterate_process(ctx, ctx->buffer);
        input += 64;
        ilen -= 64;
    }

    if (ilen > 0) {
        memcpy((void *) (ctx->buffer), input, ilen);
    }

}


void hash_finish(sm3_context *ctx, uint32_t *output) {

    uint32_t used;
    uint32_t high, low;

    used = ctx->total[0] & 0x3f;
    ctx->buffer[used++] = 0x80;

    if (used <= 56) {
        memset(ctx->buffer + used, 0, 56 - used);   // Enough room for padding + length in current block
    } else {
        memset(ctx->buffer + used, 0, 64 - used);   // Not enough, need an extra block
        iterate_process(ctx, ctx->buffer);
        memset(ctx->buffer, 0, 56);
    }

    high = (ctx->total[0] >> 29) | (ctx->total[1] <<  3);
    low  = (ctx->total[0] <<  3);
    ASSIGN_UINT32(high, ctx->buffer, 56);
    ASSIGN_UINT32(low, ctx->buffer, 60);

    iterate_process(ctx, ctx->buffer);

//    ASSIGN_UINT32(ctx->state[0], output, 0);
//    ASSIGN_UINT32(ctx->state[1], output, 1);
//    ASSIGN_UINT32(ctx->state[2], output, 2);
//    ASSIGN_UINT32(ctx->state[3], output, 3);
//    ASSIGN_UINT32(ctx->state[4], output, 4);
//    ASSIGN_UINT32(ctx->state[5], output, 5);
//    ASSIGN_UINT32(ctx->state[6], output, 6);
//    ASSIGN_UINT32(ctx->state[7], output, 7);

    for (int i = 0; i < 8; ++i) {
        output[7 - i] = ctx->state[i];
    }

}

void iterate_process(sm3_context *ctx, const unsigned char data[64]) {

    uint32_t SS1, SS2, TT1, TT2, temp;
    uint32_t A, B, C, D, E, F, G, H;
    uint32_t W[68];

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

    W[0]  = GET_UINT32(data,  0);
    W[1]  = GET_UINT32(data,  4);
    W[2]  = GET_UINT32(data,  8);
    W[3]  = GET_UINT32(data, 12);
    W[4]  = GET_UINT32(data, 16);
    W[5]  = GET_UINT32(data, 20);
    W[6]  = GET_UINT32(data, 24);
    W[7]  = GET_UINT32(data, 28);
    W[8]  = GET_UINT32(data, 32);
    W[9]  = GET_UINT32(data, 36);
    W[10] = GET_UINT32(data, 40);
    W[11] = GET_UINT32(data, 44);
    W[12] = GET_UINT32(data, 48);
    W[13] = GET_UINT32(data, 52);
    W[14] = GET_UINT32(data, 56);
    W[15] = GET_UINT32(data, 60);


    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];
    E = ctx->state[4];
    F = ctx->state[5];
    G = ctx->state[6];
    H = ctx->state[7];

    int i;
    for (i = 16; i < 68; i++) {
        W[i] = P1(W[i - 16] ^ W[i - 9] ^ ROTL(W[i - 3], 15)) ^ ROTL(W[i - 13], 7) ^ W[i - 6];
    }

    for (i = 0; i < 16; i += 8) {
        F0(A, B, C, D, E, F, G, H, ROTL(0x79cc4519,      (i)),      W[i],      WP((i)));

//        printf("\n TT2 = %08x", TT2);
//        printf("\n <== %d ==> \n", i);
//        printf("%08x %08x %08x %08x %08x %08x %08x %08x\n", H, A, B, C, D, E, F, G);

        F0(H, A, B, C, D, E, F, G, ROTL(0x79cc4519,  (i + 1)),  W[i + 1],  WP((i + 1)));
        F0(G, H, A, B, C, D, E, F, ROTL(0x79cc4519,  (i + 2)),  W[i + 2],  WP((i + 2)));
        F0(F, G, H, A, B, C, D, E, ROTL(0x79cc4519,  (i + 3)),  W[i + 3],  WP((i + 3)));
        F0(E, F, G, H, A, B, C, D, ROTL(0x79cc4519,  (i + 4)),  W[i + 4],  WP((i + 4)));
        F0(D, E, F, G, H, A, B, C, ROTL(0x79cc4519,  (i + 5)),  W[i + 5],  WP((i + 5)));
        F0(C, D, E, F, G, H, A, B, ROTL(0x79cc4519,  (i + 6)),  W[i + 6],  WP((i + 6)));
        F0(B, C, D, E, F, G, H, A, ROTL(0x79cc4519,  (i + 7)),  W[i + 7],  WP((i + 7)));
    }


    for (i = 16; i < 64; i += 8) {
        F1(A, B, C, D, E, F, G, H, ROTL(0x7a879d8a,      (i)),      W[i],      WP((i)));
        F1(H, A, B, C, D, E, F, G, ROTL(0x7a879d8a,  (i + 1)),  W[i + 1],  WP((i + 1)));
        F1(G, H, A, B, C, D, E, F, ROTL(0x7a879d8a,  (i + 2)),  W[i + 2],  WP((i + 2)));
        F1(F, G, H, A, B, C, D, E, ROTL(0x7a879d8a,  (i + 3)),  W[i + 3],  WP((i + 3)));
        F1(E, F, G, H, A, B, C, D, ROTL(0x7a879d8a,  (i + 4)),  W[i + 4],  WP((i + 4)));
        F1(D, E, F, G, H, A, B, C, ROTL(0x7a879d8a,  (i + 5)),  W[i + 5],  WP((i + 5)));
        F1(C, D, E, F, G, H, A, B, ROTL(0x7a879d8a,  (i + 6)),  W[i + 6],  WP((i + 6)));
        F1(B, C, D, E, F, G, H, A, ROTL(0x7a879d8a,  (i + 7)),  W[i + 7],  WP((i + 7)));
    }

    ctx->state[0] ^= A;
    ctx->state[1] ^= B;
    ctx->state[2] ^= C;
    ctx->state[3] ^= D;
    ctx->state[4] ^= E;
    ctx->state[5] ^= F;
    ctx->state[6] ^= G;
    ctx->state[7] ^= H;


}

//uint64_t get_message_length(char *plainaddr) {
//
//    uint64_t size;
//    FILE *fp = NULL;
//    if ((fp = fopen(plainaddr, "rb")) == NULL) {
//
//        printf("open %s faied!", plainaddr);
//        exit(0);
//    }
//
//    fseek(fp, 0, SEEK_END);
//    size = ftell(fp);
//    fseek(fp, 0, SEEK_SET);
//    fclose(fp);
//    return size;
//}

inline void put_unaligned_uint32(void *buffer, uint32_t data) {
    memcpy(buffer, &data, sizeof(data));
}

inline uint32_t get_unaligned_uint32(const void *p) {
    uint32_t r;
    memcpy(&r, p, sizeof(r));
    return r;
}