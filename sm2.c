//
// Created by 24208 on 2023/11/10.
//

#include "sm2.h"

extern LONG_NUM n;
extern LONG_NUM p;
extern LONG_NUM a;
extern LONG_NUM b;
extern LONG_NUM Gx;
extern LONG_NUM Gy;


void sm2_digital_signature() {

    p[0] = 8; p[1] = 0x08f1dfc3; p[2] = 0x722edb8b; p[3] = 0x5c45517d; p[4] = 0x45728391; p[5] = 0xbf6ff7de; p[6] = 0xe8b92435; p[7] = 0x4c044f18; p[8] = 0x8542d69e;

    a[0] = 8; a[1] = 0x3937e498; a[2] = 0xec65228b; a[3] = 0x6831d7e0; a[4] = 0x2f3c848b; a[5] = 0x73bbfeff; a[6] = 0x2417842e; a[7] = 0xfa32c3fd; a[8] = 0x787968b4;

    b[0] = 8; b[1] = 0x27c5249a; b[2] = 0x6e12d1da; b[3] = 0xb16ba06e; b[4] = 0xf61d59a5; b[5] = 0x484bfe48; b[6] = 0x9cf84241; b[7] = 0xb23b0c84; b[8] = 0x63e4c6d3;

    n[0] = 8; n[1] = 0xc32e79b7; n[2] = 0x5ae74ee7; n[3] = 0x0485628d; n[4] = 0x29772063; n[5] = 0xbf6ff7dd; n[6] = 0xe8b92435; n[7] = 0x4c044f18; n[8] = 0x8542d69e;

    Gx[0] = 8; Gx[1] = 0x7fedd43d; Gx[2] = 0x4c4e6c14; Gx[3] = 0xadd50bdc; Gx[4] = 0x32220b3b; Gx[5] = 0xc3cc315e; Gx[6] = 0x746434eb; Gx[7] = 0x1b62eab6; Gx[8] = 0x421debd6;
    Gy[0] = 8; Gy[1] = 0xe46e09a2; Gy[2] = 0xa85841b9; Gy[3] = 0xbfa36ea1; Gy[4] = 0xe5d7fdfc; Gy[5] = 0x153b70c4; Gy[6] = 0xd47349d2; Gy[7] = 0xcbb42c07; Gy[8] = 0x0680512b;

    LONG_NUM m;
    memset(m, 0x00, sizeof (m));

    unsigned char fm[STR_SIZE] = "abcdefghijklmnop";
    str_to_num(m, fm, (int) strlen((char *)fm));
    printf("The plaintext:\n");
    display_number(m);


    LONG_NUM d = { 8, 0x15897263, 0x0c23661d, 0x171b1b65, 0x2a519a55, 0x3dff7979, 0x068c8d80, 0xbd433c6c, 0x128b2fa8 };

    LONG_NUM Px, Py;
    memset(Px, 0x00, sizeof (Px));
    memset(Py, 0x00, sizeof (Py));

    naf_point_mul(d, Gx, Gy, Px, Py);
    printf("Public key (Px, Py):\n");
    display_number(Px);
    display_number(Py);

    uint8_t ID_A[] = "Asuncion@huang";
    uint8_t ENTL_A[2] = { 0 };
    ENTL_A[0] = 0x1f;
    ENTL_A[1]= 0x00;

    uint8_t za_str[512];
    memset(za_str, 0x00, sizeof (za_str));
    za_str[0] = ENTL_A[0], za_str[1] = ENTL_A[1];
    strcpy((char *)(za_str + 2), (char *)ID_A);
    num_to_str(a, za_str + 2 + strlen((char *)ID_A));
    num_to_str(b, za_str + 2 + strlen((char *)ID_A) + 32);
    num_to_str(Gx, za_str + 2 + strlen((char *)ID_A) + 64);
    num_to_str(Gy, za_str + 2 + strlen((char *)ID_A) + 96);
    num_to_str(Px, za_str + 2 + strlen((char *)ID_A) + 128);
    num_to_str(Py, za_str + 2 + strlen((char *)ID_A) + 160);

    LONG_NUM ZA, r, s;
    memset(ZA, 0x00, sizeof (ZA));
    memset(r, 0x00, sizeof (r));
    memset(s, 0x00, sizeof (s));
    sm3_encryption(za_str, 2 + strlen((char *)ID_A) + 192, ZA);
    for (int i = 7; i > 1; i--) {
        ZA[i + 1] = ZA[i];
    }
    ZA[0] = 8;
    printf("ZA:\n");
    display_number(ZA);

    printf("\nsignature:\n");
    sm2_signature(fm, ZA, d, r, s);
    printf("\nverification:\n");
    sm2_verification(fm, Px, Py, ZA, r, s);

}


void sm2_signature(unsigned char* m, LONG_NUM ZA, LONG_NUM d, LONG_NUM r, LONG_NUM s) {

    LONG_NUM ONE_NUM = { 1, 1 };

    LONG_NUM temp_r, temp_s;

    uint8_t M_hat[STR_SIZE * 2];
    memset(M_hat, 0x00, sizeof (M_hat));
    num_to_str(ZA, M_hat);

    for (int i = 0; i < strlen((char *)m); ++i) {
        M_hat[i + 32] = m[i];
    }

    LONG_NUM e;
    memset(e, 0x00, sizeof (e));
    sm3_encryption(M_hat, 32 + strlen((char *)m), e);
    for (int i = 7; i > 1; i--) {
        e[i + 1] = e[i];
    }
    e[0] = 8;
//    e[1] = 0xb9effe76;
//    e[2] = 0x5d42e3d9;
//    e[3] = 0x682d48bb;
//    e[4] = 0x9a87e6fc;
//    e[5] = 0x5c377fb1;
//    e[6] = 0x28476e00;
//    e[7] = 0xcd82b8b0;
//    e[8] = 0xb524f552;
    printf("e:\n");
    display_number(e);

    LONG_NUM x1, y1;
    memset(x1, 0x00, sizeof (x1));
    memset(y1, 0x00, sizeof (y1));

    LONG_NUM rk;
    memset(rk, 0x00, sizeof (rk));

    LONG_NUM d1, inverse_d, rd, k_rd;
    memset(d1, 0x00, sizeof (d1));
    memset(inverse_d, 0x00, sizeof (inverse_d));
    memset(rd, 0x00, sizeof (rd));
    memset(k_rd, 0x00, sizeof (k_rd));

    do {

        memset(temp_r, 0x00, sizeof (temp_r));
        memset(temp_s, 0x00, sizeof (temp_s));
//    Random nunber k generate
//        LONG_NUM k;
//
//        LONG_NUM temp;
//        memset(k, 0x00, sizeof (k));
//        memset(temp, 0x00, sizeof (temp));
//        unsigned char random_str[STR_SIZE];
//        memset(random_str, 0x00, sizeof (random_str));
//        random_generate(random_str, STR_SIZE);
//        temp[0] = 8;
//        str_to_num(temp, random_str, STR_SIZE);
//        mod_n(temp, n, k);
//        printf("random number k:\n");
//        display_number(k);

        LONG_NUM k = { 8, 0x1fb2f96f, 0x260dbaae, 0xdd72b727, 0xc176d925, 0x4817663f, 0x94f94e93, 0x385c175c, 0x6cb28d99 };

//        k[0] = 8; k[1] = 0x1fb2f96f, k[2] = 0x260dbaae, k[3] = 0xdd72b727, k[4] = 0xc176d925, k[5] = 0x4817663f, k[6] = 0x94f94e93, k[7] = 0x385c175c, k[8] = 0x6cb28d99;

//        (x1, y1) = [k] * G
        naf_point_mul(k, Gx, Gy, x1, y1);
        printf("(x1, y1):\n");
        display_number(x1);
        display_number(y1);

        mod_add(e, x1, n, temp_r);
        add(temp_r, k, rk);
        printf("(r):\n");
        display_number(temp_r);
        if (temp_r[0] == 0 || number_compare(rk, n) == 0) {
            printf("rk:\n");
            display_number(rk);
            continue;
        }

        mod_add(ONE_NUM, d, n, d1);
        inverse(d1, n, inverse_d);
        mul(temp_r, d, rd);
        mod_sub(k, rd, n, k_rd);
        mod_mul(inverse_d, k_rd, n, temp_s);
        printf("(s):\n");
        display_number(temp_s);


    } while(temp_r[0] == 0 || number_compare(rk, n) == 0 || temp_s[0] == 0);

    printf("(r, s):\n");
    display_number(temp_r);
    display_number(temp_s);

    number_copy(r, temp_r);
    number_copy(s, temp_s);

}

void sm2_verification(unsigned char* m, LONG_NUM Px, LONG_NUM Py, LONG_NUM ZA, LONG_NUM r, LONG_NUM s) {

    LONG_NUM temp_r, temp_s;
    memset(temp_r, 0x00, sizeof (temp_r));
    memset(temp_s, 0x00, sizeof (temp_s));
    number_copy(temp_r, r);
    number_copy(temp_s, s);

    if (number_compare(temp_r, n) >= 0 || temp_r[0] == 0) {
        printf("Verification fail!\n");
        return;
    }
    if (number_compare(temp_s, n) >= 0 || temp_s[0] == 0) {
        printf("Verification fail!\n");
        return;
    }

    printf("(r', s'):\n");
    display_number(temp_r);
    display_number(temp_s);

    uint8_t M_hat[STR_SIZE * 2];
    memset(M_hat, 0x00, sizeof (M_hat));
    num_to_str(ZA, M_hat);

    for (int i = 0; i < strlen((char *)m); ++i) {
        M_hat[i + 32] = m[i];
    }
//    strcat((char *)(M_hat + 32), (char *)m);

    LONG_NUM e;
    memset(e, 0x00, sizeof (e));
    sm3_encryption(M_hat, 32 + strlen((char *)m), e);
    for (int i = 7; i > 1; i--) {
        e[i + 1] = e[i];
    }
    e[0] = 8;
//    e[1] = 0xb9effe76;
//    e[2] = 0x5d42e3d9;
//    e[3] = 0x682d48bb;
//    e[4] = 0x9a87e6fc;
//    e[5] = 0x5c377fb1;
//    e[6] = 0x28476e00;
//    e[7] = 0xcd82b8b0;
//    e[8] = 0xb524f552;
    printf("e:\n");
    display_number(e);

    LONG_NUM temp_t;
    memset(temp_t, 0x00, sizeof (temp_t));
    mod_add(temp_s, temp_r, n, temp_t);
    if (temp_t[0] == 0) {
        printf("Verification fail!\n");
        return;
    }

    LONG_NUM temp1x, temp1y, temp2x, temp2y, x1, y1;
    naf_point_mul(temp_s, Gx, Gy, temp1x, temp1y);
    naf_point_mul(temp_t, Px, Py, temp2x, temp2y);
    affine_coordinates_addition(temp1x, temp1y, temp2x, temp2y, x1, y1);
    printf("(x1, y1):\n");
    display_number(x1);
    display_number(y1);

    LONG_NUM R;
    memset(R, 0x00, sizeof (R));
    mod_add(e, x1, n, R);

    printf("R:\n");
    display_number(R);

    if(number_compare(R, temp_r) == 0) {
        printf("Verification success!");
    }

}

void sm2_cryption() {

    LONG_NUM m;
    memset(m, 0x00, sizeof (m));

    unsigned char fm[STR_SIZE] = "abcdefghijklmnop";
    str_to_num(m, fm, (int) strlen((char *)fm));
    printf("The plaintext:\n");
    display_number(m);

    LONG_NUM C1x, C1y, C2, C3;
    memset(C1x, 0x00, sizeof (C1x));
    memset(C1y, 0x00, sizeof (C1y));
    memset(C2, 0x00, sizeof (C2));
    memset(C3, 0x00, sizeof (C3));

    init_random_seed();

    unsigned char random_str[STR_SIZE];
    memset(random_str, 0x00, sizeof (random_str));

//    // TO DO
//    // Random nunber generate
//    LONG_NUM temp;
//    memset(temp, 0x00, sizeof (temp));
//    LONG_NUM private_key_d;
//    memset(private_key_d, 0x00, sizeof (private_key_d));
//    random_generate(random_str, STR_SIZE);
//    temp[0] = 8;
//    str_to_num(temp, random_str, STR_SIZE);
//    mod_n(temp, n, private_key_d);
//    printf("private key d:\n");
//    display_number(private_key_d);
    LONG_NUM private_key_d = { 8, 0x20bb0da0, 0x08ddbc29, 0xb89463f2, 0x34aa7f7c, 0x3fbf3535, 0x5e2efe28, 0xa00637bd, 0x1649ab77 };

    LONG_NUM Px, Py;
    memset(Px, 0x00, sizeof (Px));
    memset(Py, 0x00, sizeof (Py));

    naf_point_mul(private_key_d, Gx, Gy, Px, Py);
    printf("Public key (Px, Py):\n");
    display_number(Px);
    display_number(Py);


    printf("\n\nencryption:\n");
    sm2_encryption(fm, Px, Py, C1x, C1y, C2, C3);
    printf("\n\ndecryption:\n");
    sm2_decryption(C1x, C1y, C2, C3, private_key_d);

}


void sm2_encryption(unsigned char* m, LONG_NUM Px, LONG_NUM Py, LONG_NUM C1x, LONG_NUM C1y, LONG_NUM C2, LONG_NUM C3) {

    // Random nunber k generate
    LONG_NUM k;
//    LONG_NUM temp;
//    memset(k, 0x00, sizeof (k));
//    memset(temp, 0x00, sizeof (temp));
//    unsigned char random_str[STR_SIZE];
//    memset(random_str, 0x00, sizeof (random_str));
//    random_generate(random_str, STR_SIZE);
//    temp[0] = 8;
//    str_to_num(temp, random_str, STR_SIZE);
//    mod_n(temp, n, k);
//    printf("random number k:\n");
//    display_number(k);

    k[0] = 8; k[1] = 0x49dd7b4f, k[2] = 0x18e5388d, k[3] = 0x5546d490, k[4] = 0x8afa1742, k[5] = 0x3d957514, k[6] = 0x5b92fd6c, k[7] = 0x6ecfc2b9, k[8] = 0x4c62eefd;

//    C1 = [k] * G
    naf_point_mul(k, Gx, Gy, C1x, C1y);
    printf("(C1x, C1y):\n");
    display_number(C1x);
    display_number(C1y);

//    C2 = M xor t
//    t = KDF(x2 || y2, len)
    LONG_NUM x2, y2;
    memset(x2, 0x00, sizeof (x2));
    memset(y2, 0x00, sizeof (y2));
    naf_point_mul(k, Px, Py, x2, y2);

//    printf("(x2, y2):\n");
//    display_number(x2);
//    display_number(y2);

    LONG_NUM t;
    memset(t, 0x00, sizeof (t));
    uint8_t xy[STR_SIZE * 3], t_str[STR_SIZE];
    memset(xy, 0x00, sizeof (xy));
    memset(t_str, 0x00, sizeof (t_str));
    num_to_str(x2, xy);

    num_to_str(y2, xy + STR_SIZE);

//    printf("After loading (x2, y2)\n");
//    for (int i = 0; i < 2 * STR_SIZE; ++i) {
//        uint8_t tmp = (uint8_t) xy[i];
//        printf("%x ", tmp);
//    }
//    printf("\n");

    KDF(xy, 128, t_str);

//    printf("After loading t\n");
//    for (int i = 0; i < 16; ++i) {
//        uint8_t tmp = t_str[i];
//        printf("%02x ", tmp);
//    }
//    printf("\n");


    str_to_num(t, t_str, STR_SIZE);


    printf("t:\n");
    display_number(t);

    LONG_NUM fm;
    memset(fm, 0x00, sizeof (fm));
    fm[0] = NUMBER_LENGTH;
    str_to_num(fm, m, (int) strlen((char *)m));

//    printf("m:\n");
//    display_number(fm);

    C2[0] = t[0];
    for (int i = 1; i <= t[0]; ++i) {
        C2[i] = t[i] ^ fm[i];
    }

    printf("C2:\n");
    display_number(C2);

//    C3 = Hash(x2 || M || y2)
    unsigned char xmy[STR_SIZE * 16];
    memset(xmy, 0x00, sizeof (xmy));

    uint64_t xmy_length = 0;
    num_to_str(x2, xmy);
    xmy_length += STR_SIZE;
    strcat((char*)(xmy + xmy_length), (char*)m);
    xmy_length += strlen((char*)m);
    num_to_str(y2, xmy + xmy_length);
    xmy_length += STR_SIZE;

    for (int i = 0; i < xmy_length; ++i) {
        printf("%02x ", xmy[i]);
    }
    printf("\n");

////    verify xmy
//    printf("xmy\n");
//    LONG_NUM temp;
//    memset(temp, 0x00, sizeof (temp));
//    str_to_num(temp, xmy, xmy_length);
//    display_number(temp);

    sm3_encryption(xmy, xmy_length, C3);

    for (int i = 8; i > 0; i--) {
        C3[i] = C3[i - 1];
    }
    C3[0] = 8;
    printf("C3:\n");
    display_number(C3);


}


void sm2_decryption(LONG_NUM C1x, LONG_NUM C1y, const LONG_NUM C2, LONG_NUM C3, LONG_NUM d) {

    LONG_NUM right, left;
    memset(right, 0x00, sizeof (right));
    memset(left, 0x00, sizeof (left));

    LONG_NUM temp1, temp2, temp3;
    memset(temp1, 0x00, sizeof (temp1));
    memset(temp2, 0x00, sizeof (temp2));
    memset(temp3, 0x00, sizeof (temp3));

//    verify C1: y^2 = x^3 + a * x + b
    mod_mul(C1y, C1y, p, left);

    mod_mul(C1x, C1x, p, temp1);
    mod_mul(C1x, temp1, p, temp2);
    mod_mul(a, C1x, p, temp1);
    add(temp1, temp2, temp3);
    mod_add(temp3, b, p, right);

    if (number_compare(right, left) != 0) {
        printf("C1 error!\n");
        return;
    }

    LONG_NUM x2, y2;
    memset(x2, 0x00, sizeof (x2));
    memset(y2, 0x00, sizeof (y2));
    naf_point_mul(d, C1x, C1y, x2, y2);

//    printf("(x2, y2):\n");
//    display_number(x2);
//    display_number(y2);

    LONG_NUM t;
    memset(t, 0x00, sizeof (t));
    uint8_t xy[STR_SIZE * 2], t_str[STR_SIZE];
    memset(xy, 0x00, sizeof (xy));
    memset(t_str, 0x00, sizeof (t_str));
    num_to_str(x2, xy);
    num_to_str(y2, xy + STR_SIZE);

//    verify xy
//    printf("After loading (x2, y2)\n");
//    for (int i = 0; i < 2 * STR_SIZE; ++i) {
//        uint8_t tmp = (uint8_t) xy[i];
//        printf("%x ", tmp);
//    }
//    printf("\n");

    KDF(xy, 128, t_str);

//    printf("After loading t\n");
//    for (int i = 0; i < 16; ++i) {
//        uint8_t tmp = t_str[i];
//        printf("%02x ", tmp);
//    }
//    printf("\n");

    str_to_num(t, t_str, STR_SIZE);


    if (t[0] == 0) {
        printf("t is a string with all zeros\n");
        return;
    }
    printf("t:\n");
    display_number(t);

    LONG_NUM m;
    uint8_t m_str[STR_SIZE];
    memset(m, 0x00, sizeof (m));
    memset(m_str, 0x00, sizeof (m_str));
    m[0] = C2[0];
    for (int i = 1; i <= m[0]; i++) {
        m[i] = t[i] ^ C2[i];
//        printf("%08x ", m[i]);
    }
//    printf("Decryption yields the plaintext m:\n");
//    display_number(m);
    num_to_str(m, m_str);

    unsigned char xmy[STR_SIZE * 16];
    memset(xmy, 0x00, sizeof (xmy));

    uint64_t xmy_length = 0;
    num_to_str(x2, xmy);
    xmy_length += STR_SIZE;
    strcat((char*)(xmy + xmy_length), (char*)m_str);
    xmy_length += strlen((char*)m_str);
    num_to_str(y2, xmy + xmy_length);
    xmy_length += STR_SIZE;

    printf("string(x2 || m || y2):\n");
    for (int i = 0; i < xmy_length; ++i) {
        printf("%02x ", xmy[i]);
    }
    printf("\n");

    LONG_NUM c3_verify;
    memset(c3_verify, 0x00, sizeof (c3_verify));
    sm3_encryption(xmy, xmy_length, c3_verify);


    for (int i = 7; i > 1; i--) {
        c3_verify[i + 1] = c3_verify[i];
    }
    c3_verify[0] = 8;
    printf("c3_verify:\n");
    display_number(c3_verify);


    if (number_compare(C3, c3_verify) == 0) {

        printf("Decryption yields the plaintext m:\n");
        display_number(m);
    }
    else {
        printf("Decryption error!\n");
    }

}

void KDF(const uint8_t* Z, int klen, uint8_t* k) {

    uint32_t check_code[8];
    uint32_t check_length = 32;
    uint8_t cnt[4] = { 0 };
    uint32_t ct = 0x00000001;

    int times = (klen + 255) / 256;

    uint8_t group[STR_SIZE * 3];
    for (int i = 0; i < 2 * STR_SIZE; ++i) {
        group[i] = Z[i];
    }

    for (int i = 0; i < times; i++) {

        cnt[0] = (ct >> 24) & 0xff;
        cnt[1] = (ct >> 16) & 0xff;
        cnt[2] = (ct >> 8) & 0xff;
        cnt[3] = (ct) & 0xff;

        memcpy(group + 2 * STR_SIZE, cnt, 4);

//        printf("\n<=== input ===>\n");
//        for (int j = 0; j < 68; j++) {
//            printf("%02x ", group[j]);
//        }
//        printf("\n<=== input ===>\n");

        sm3_encryption(group, 2 * STR_SIZE + 4, check_code);

//        printf("\n<=== check_code ===>\n");
//        for (int j = 0; j < 8; j++) {
//            printf("%08x ", check_code[i]);
//        }
//        printf("\n<=== check_code ===>\n");

        if (i == times - 1) {
            if (klen % 256 != 0) {
                check_length = (klen % 256) / 8;
            }
        }

        memcpy(k + i * 32, (uint8_t *) check_code, check_length);

        ct++;

    }

//    uint8_t group[STR_SIZE * 3];
//    for (int i = 0; i < message_length; ++i) {
//        group[i] = Z[i];
//    }
//
//    uint32_t v = 256;
//    uint32_t klenv = (message_length * 8 + v - 1) / v;
//
//    uint32_t Ha[64][8];
//
//    for (int ct = 1,  i = 0; i < klenv; ++ct, ++i) {
//        group[message_length] = (ct & 0xff000000) >> 24;
//        group[message_length + 1] = (ct & 0x00ff0000) >> 16;
//        group[message_length + 2] = (ct & 0x0000ff00) >> 8;
//        group[message_length + 3] = (ct & 0x000000ff);
//
//        sm3_encryption(group, message_length + 4, Ha[i]);
//
//        if (i == klenv - 1) {
//            if ((message_length * 8) % v != 0) {
//
//                int remain = (message_length * 8) % (int)v;
//
//                LONG_NUM temp;
//                temp[0] = 8;
//                for (int j = 0; j < 8; j++) {
//                    temp[j + 1] = Ha[i][j];
//                }
//
//                for (int j = 0; j < remain; j++) {
//                    shift_right(temp);
//                }
//
//                for (int j = 0; j < 8; j++) {
//                    Ha[i][j] = temp[j + 1];
//                }
//
//            }
//        }
//
//        num_to_str(Ha[i], k + i * 32);
//
//    }

}