//
// Created by 24208 on 2023/11/2.
//

#include "optimizer.h"


void montgomery_exp(LONG_NUM M, LONG_NUM e, LONG_NUM n, LONG_NUM x) {

    LONG_NUM ONE_NUM = { 1, 1 };

    LONG_NUM r, inverse_r, prime_n;
    //    LONG_NUM n_bits_e;
    LONG_NUM temp_n;
    //    memset(n_bits_e, 0, sizeof (n_bits_e));
    memset(r, 0, sizeof(r));
    memset(inverse_r, 0, sizeof(inverse_r));
    memset(prime_n, 0, sizeof(prime_n));
    memset(temp_n, 0, sizeof(temp_n));

    number_copy(temp_n, n);

    //    r = 2^{sw}
    int n_bits = get_bits(temp_n);
    int n_words = (n_bits + 31) / 32;
    //    int remain = n_bits - (n_words - 1) * 32;
    r[0] = n_words + 1;
    r[n_words + 1] = (uint32_t)(1U);

    //    printf("\nr:\n");
    //    display_number(r);
    //    mod_exp(TWO_NUM, n_bits_e, temp_n, r);
    //    LONG_NUM factor;
    //    gcd(r, temp_n, factor);
    //    if (number_compare(factor, ONE_NUM) == 0) {
    //        printf("\ntrue\n");
    //    } else {
    //        printf("\nfalse\n");
    //    }

    inverse(r, temp_n, inverse_r);

    DOUBLE_LONG_NUM prod_r, prod_r_1;
    LONG_NUM rem_n;
    memset(prod_r, 0, sizeof(prod_r));
    memset(prod_r_1, 0, sizeof(prod_r_1));

    mul(r, inverse_r, prod_r);
    sub(prod_r, ONE_NUM, prod_r_1);
    division_v2(prod_r_1, temp_n, prime_n, rem_n);
    //    printf("\n===============================\n");
    //    display_number(prod_r_1);
    //    display_number(temp_n);
    //    display_number(prime_n);
    //    printf("\n===============================\n");

    LONG_NUM hat_M, hat_x;
    memset(hat_M, 0, sizeof(hat_M));
    memset(hat_x, 0, sizeof(hat_x));

    LONG_NUM temp_M;
    memset(temp_M, 0, sizeof(temp_M));
    number_copy(temp_M, M);

    mod_mul(M, r, temp_n, hat_M);
    mod_mul(ONE_NUM, r, temp_n, hat_x);

    LONG_NUM temp_x;
    memset(temp_x, 0, sizeof(temp_x));

    uint32_t* eptr, * meptr;
    eptr = e + 1;
    meptr = e + e[0];

    char binary_e[33];
    int e_length = 0;
    //   to do
    //   Converting exponential e to binary expanded form, and store it in array binary_e
    while (eptr <= meptr) {

        memset(binary_e, 0, sizeof(binary_e));
        _ultoa(*meptr, binary_e, 2);

        e_length = (int)strlen(binary_e);
        for (int j = 32; j > 0; j--) {
            if (meptr < e + e[0] || j <= e_length) {
                mon_pro_sos(hat_x, hat_x, temp_n, prime_n, r, temp_x);
                //                mon_pro(hat_x, hat_x, temp_n, prime_n, r, temp_x);
                number_copy(hat_x, temp_x);
            }
            if (j <= e_length) {
                if (binary_e[e_length - j] == '1') {
                    mon_pro_sos(hat_M, hat_x, temp_n, prime_n, r, temp_x);
                    number_copy(hat_x, temp_x);
                }
            }
        }
        meptr--;
    }
    //    for (int i = n_bits - 1; i >= 0; --i) {
    //
    //        mon_pro(hat_x, hat_x, n, prime_n, r, temp_x);
    //        number_copy(hat_x, temp_x);
    //
    //        if (binary_e[i] == 1) {
    //            mon_pro(hat_M, hat_x, n, prime_n, r, temp_x);
    //            number_copy(hat_x, temp_x);
    //        }
    //
    //    }
    mon_pro_sos(hat_x, ONE_NUM, temp_n, prime_n, r, temp_x);
    number_copy(x, temp_x);

    while ((x[0] > 0) && x[x[0]] == 0) {
        x[0] = x[0] - 1;
    }

}


void mon_pro(LONG_NUM hat_a, LONG_NUM hat_b, LONG_NUM n, LONG_NUM prime_n, LONG_NUM r, LONG_NUM result) {

    LONG_NUM temp_m, temp_u;
    LONG_NUM temp2, temp3, temp4;
    DOUBLE_LONG_NUM temp_t, temp1;

    memset(temp_t, 0, sizeof(temp_t));
    memset(temp_m, 0, sizeof(temp_m));
    //    memset(temp_u0, 0, sizeof (temp_u0));
    //    memset(temp_u1, 0, sizeof (temp_u1));

    //    t = \hat{a} * \hat{b}
    mul(hat_a, hat_b, temp_t);

    //    m = t * n^{\prime} mod r
    mod_n(temp_t, r, temp1);
    mod_mul(temp1, prime_n, r, temp_m);

    //    u = (t + m * n) / r
    mul(temp_m, n, temp1);
    //    division(temp1, r, temp_u0, temp3);
    add(temp_t, temp1, temp2);
    division_v2(temp2, r, temp_u, temp3);
    //    division_v2(temp_t, r, temp_u1, temp3);
    //    add(temp_u0, temp_u1, temp_u);

    if (number_compare(temp_u, n) >= 0) {
        sub(temp_u, n, temp4);
        number_copy(result, temp4);
    }
    else {
        number_copy(result, temp_u);
    }

    while ((result[0] > 0) && (result[result[0]] == 0)) {
        result[0] = result[0] - 1;
    }

}


void mon_pro_sos(LONG_NUM hat_a, LONG_NUM hat_b, LONG_NUM n, const LONG_NUM prime_n, LONG_NUM r, LONG_NUM result) {

    DOUBLE_LONG_NUM temp_t, temp_u;
    memset(temp_t, 0, sizeof(temp_t));
    memset(temp_u, 0, sizeof(temp_u));

    uint32_t* maptr, * mbptr;
    uint32_t* tptr, * midtptr, * endtptr;
    uint32_t* acir, * bcir, * tcir, * ncir;
    uint32_t hatb_words = 0;
    uint64_t carry = 0;

    maptr = hat_a + hat_a[0];
    mbptr = hat_b + hat_b[0];
    temp_t[0] = *hat_a + *hat_b;

    for (bcir = hat_b + 1, tptr = temp_t + 1; bcir <= mbptr; bcir++, tptr++) {

        carry = 0;
        hatb_words = *bcir;

        for (acir = hat_a + 1, tcir = tptr; acir <= maptr; acir++, tcir++) {
            carry = (uint64_t)*tcir + (uint64_t)hatb_words * (uint64_t)*acir + (uint64_t)(uint32_t)(carry >> WORD_BITS);
            *tcir = (uint32_t)carry;
        }
        *tcir = (uint32_t)(carry >> WORD_BITS);
    }

    uint32_t m = 0;
    uint32_t s = hat_a[0];
    midtptr = temp_t + s;
    endtptr = temp_t + 2 * s;

    for (tptr = temp_t + 1; tptr <= midtptr; tptr++) {

        carry = 0;
        m = (uint32_t)((*tptr) * (*(prime_n + 1)));

        for (tcir = tptr, ncir = n + 1; tcir < tptr + s; tcir++, ncir++) {
            carry = (uint64_t)*tcir + (uint64_t)m * (uint64_t)*ncir + (uint64_t)(uint32_t)(carry >> WORD_BITS);
            *tcir = (uint32_t)carry;
        }

        for (tcir = tptr + s; tcir <= endtptr; tcir++) {
            carry = (uint64_t)*tcir + (uint64_t)(uint32_t)(carry >> WORD_BITS);
            *tcir = (uint32_t)carry;
        }

    }
    temp_t[2 * s + 1] = (uint32_t)(carry >> WORD_BITS);

    temp_u[0] = s + 1;
    for (int j = 1; j <= s + 1; j++) {
        temp_u[j] = temp_t[j + s];
    }

    if (number_compare(temp_u, n) >= 0) {
        sub(temp_u, n, result);
    }
    else {
        number_copy(result, temp_u);
    }

    while ((result[0] > 0) && (result[result[0]] == 0)) {
        result[0] = result[0] - 1;
    }

}