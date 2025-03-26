//
// Created by 24208 on 2023/10/26.
//

#include "basic_calculation.h"

void mod_exp_quaternary(LONG_NUM M, LONG_NUM e, LONG_NUM n, LONG_NUM rem) {

    LONG_NUM ONE_NUM = { 1, 1 };

    LONG_NUM temp_C, temp1;
    memset(temp_C, 0, sizeof(temp_C));
    memset(temp1, 0, sizeof(temp1));

    LONG_NUM M_F[QUATERNARY];
    for (int i = 0; i < QUATERNARY; i++) {
        memset(M_F[i], 0, sizeof(M_F[i]));
    }
    number_copy(M_F[0], ONE_NUM);
    number_copy(M_F[1], M);
    mod_mul(M_F[1], M, n, M_F[2]);
    mod_mul(M_F[2], M, n, M_F[3]);

    char e_quaternary[17];
    int e_length = 0, idx = 0;
    memset(e_quaternary, 0, sizeof(e_quaternary));

    uint32_t* eptr, * meptr;
    eptr = e + 1;
    meptr = e + e[0];
    _ultoa(*meptr, e_quaternary, QUATERNARY);
    number_copy(temp_C, M_F[e_quaternary[0] - '0']);

    e_length = (int)strlen(e_quaternary);
    for (int i = 1; i < e_length; i++) {

        for (int k = 0; k < 2; k++) {
            mod_mul(temp_C, temp_C, n, temp1);
            number_copy(temp_C, temp1);
        }
        //        mod_mul(temp_C, temp_C, n, temp1);
        //        number_copy(temp_C, temp1);
        //        mod_mul(temp_C, temp_C, n, temp1);
        //        number_copy(temp_C, temp1);
        //        mod_mul(temp_C, temp_C, n, temp1);
        //        number_copy(temp_C, temp1);

        if (e_quaternary[i] != '0') {
            mod_mul(temp_C, M_F[e_quaternary[i] - '0'], n, temp1);
            number_copy(temp_C, temp1);
        }
    }
    meptr--;

    while (eptr <= meptr) {
        memset(e_quaternary, 0, sizeof(e_quaternary));
        _ultoa(*meptr, e_quaternary, QUATERNARY);

        idx = 0;
        e_length = (int)strlen(e_quaternary);

        for (int j = 16; j > 0; j--) {

            for (int k = 0; k < 2; k++) {
                mod_mul(temp_C, temp_C, n, temp1);
                number_copy(temp_C, temp1);
            }
            //            mod_mul(temp_C, temp_C, n, temp1);
            //            number_copy(temp_C, temp1);
            //            mod_mul(temp_C, temp_C, n, temp1);
            //            number_copy(temp_C, temp1);
            //            mod_mul(temp_C, temp_C, n, temp1);
            //            number_copy(temp_C, temp1);

            if (j <= e_length) {
                if (e_quaternary[idx] != '0') {
                    mod_mul(temp_C, M_F[e_quaternary[idx] - '0'], n, temp1);
                    number_copy(temp_C, temp1);
                }
                idx++;
            }
        }
        meptr--;
    }

    number_copy(rem, temp_C);

    while ((rem[0] > 0) && (rem[rem[0]] == 0)) {
        rem[0] = rem[0] - 1;
    }

}

void mod_exp_binary(LONG_NUM M, LONG_NUM e, LONG_NUM n, LONG_NUM rem) {

    LONG_NUM temp_a, temp_M;
    LONG_NUM temp1, temp2;
    memset(temp_a, 0, sizeof(temp_a));
    memset(temp_M, 0, sizeof(temp_M));
    memset(temp1, 0, sizeof(temp1));
    memset(temp2, 0, sizeof(temp2));

    temp_a[0] = 1, temp_a[1] = 1;
    number_copy(temp_M, M);

    uint32_t* eptr, * meptr;
    eptr = e + 1;
    meptr = e + e[0];

    char binary_e[33];
    int i = 0;
    while (eptr <= meptr) {
        memset(binary_e, 0, sizeof(binary_e));
        _ultoa(*eptr, binary_e, 2);

        i = (int)strlen(binary_e) - 1;
        for (int j = 32; j > 0; j--) {

            if (i >= 0) {
                if (binary_e[i] == '1') {
                    mod_mul(temp_a, temp_M, n, temp1);
                    number_copy(temp_a, temp1);
                }
                i--;
            }
            if (eptr < meptr || i >= 0) {
                mod_mul(temp_M, temp_M, n, temp2);
                number_copy(temp_M, temp2);
            }
        }
        eptr++;
    }

    number_copy(rem, temp_a);

    while ((rem[0] > 0) && (rem[rem[0]] == 0)) {
        rem[0] = rem[0] - 1;
    }

}

void mod_exp(LONG_NUM M, LONG_NUM e, LONG_NUM n, LONG_NUM rem) {

    LONG_NUM temp_M, temp_e, temp_C, temp;
    memset(temp_M, 0, sizeof(temp_M));
    memset(temp_e, 0, sizeof(temp_e));
    memset(temp_C, 0, sizeof(temp_C));
    memset(temp, 0, sizeof(temp));

    number_copy(temp_M, M);
    number_copy(temp_e, e);

    char binary_e[STR_SIZE << 5];
    memset(binary_e, 0, sizeof(binary_e));
    for (int i = 0; i < STR_SIZE; i++) {
        binary_e[i] = '0';
    }

    uint32_t* eptr = temp_e + 1, * meptr = temp_e + temp_e[0];
    int idx = 0;

    // problem, won't use anymore
    while (eptr <= meptr) {
        //_ultoa_s(*eptr, binary_e + idx, 2);
        idx += 32;
        eptr++;
    }

    int e_bin_length = (int)strlen(binary_e);
    if (binary_e[e_bin_length - 1] == '1') {
        number_copy(temp_C, temp_M);
    }
    else {
        temp_C[0] = temp_C[1] = 1;
    }

    for (int i = e_bin_length - 2; i >= 0; --i) {
        mod_mul(temp_C, temp_C, n, temp);
        number_copy(temp_C, temp);

        if (binary_e[i] == '1') {
            mod_mul(temp_C, temp_M, n, temp);
            number_copy(temp_C, temp);
        }
        memset(temp, 0, sizeof(temp));
    }

    number_copy(rem, temp_C);

    while ((rem[0] > 0) && (rem[rem[0]] == 0)) {
        rem[0] = rem[0] - 1;
    }
}


int inverse(LONG_NUM a, LONG_NUM n, LONG_NUM inverse_element) {

    LONG_NUM ZERO_NUM = { 0 };
    LONG_NUM ONE_NUM = { 1, 1 };

    int flag = VALID;

    LONG_NUM u, g, v1, v3, q, t1, t3;
    LONG_NUM temp;
    memset(u, 0, sizeof(u));
    memset(g, 0, sizeof(g));
    memset(v1, 0, sizeof(v1));
    memset(v3, 0, sizeof(v3));
    memset(q, 0, sizeof(q));
    memset(t1, 0, sizeof(t1));
    memset(t3, 0, sizeof(t3));
    memset(temp, 0, sizeof(temp));

    gcd(a, n, g);

    if (number_compare(g, ONE_NUM) != 0) {
        inverse_element[0] = 0;
        flag = NO_INVERSE;
        return flag;
    }

    memset(g, 0, sizeof(g));
    number_copy(u, ONE_NUM);
    number_copy(g, a);
    v1[0] = 0;
    number_copy(v3, n);

    LONG_NUM diff, rem;
    memset(diff, 0, sizeof(diff));
    memset(rem, 0, sizeof(rem));

    while (number_compare(v3, ZERO_NUM) != 0) {

        division_v2(g, v3, q, t3);
        mod_mul(q, v1, n, temp);

        if (number_compare(u, temp) >= 0) {

            sub(u, temp, diff);
            mod_n(diff, n, rem);
            number_copy(t1, rem);

        }
        else {

            sub(temp, u, diff);
            mod_n(diff, n, rem);
            sub(n, rem, diff);
            number_copy(t1, diff);
        }

        number_copy(u, v1);
        number_copy(g, v3);
        number_copy(v1, t1);
        number_copy(v3, t3);
    }

    number_copy(inverse_element, u);

    return flag;
}


int gcd(LONG_NUM a, LONG_NUM b, LONG_NUM factor) {

    int flag = VALID;
    LONG_NUM temp_a, temp_b;
    LONG_NUM remainder, next_remainder;

    memset(temp_a, 0, sizeof(temp_a));
    memset(temp_b, 0, sizeof(temp_b));
    memset(remainder, 0, sizeof(remainder));

    number_copy(temp_a, a);
    number_copy(temp_b, b);

    if (temp_a[0] == 0) {
        number_copy(factor, temp_b);
        return flag;
    }
    if (temp_b[0] == 0) {
        number_copy(factor, temp_a);
        return flag;
    }

    LONG_NUM q;
    memset(q, 0, sizeof(q));
    flag = division_v2(temp_a, temp_b, q, remainder);

    while (remainder[0] != 0) {

        number_copy(temp_a, temp_b);
        number_copy(temp_b, remainder);
        number_copy(next_remainder, remainder);
        memset(q, 0, sizeof(q));

        flag = division_v2(temp_a, temp_b, q, remainder);

        while ((remainder[0] > 0) && (remainder[remainder[0]] == 0)) {
            remainder[0] = remainder[0] - 1;
        }
    }

    number_copy(factor, temp_b);
    return flag;
}


int mod_add(LONG_NUM a, LONG_NUM b, LONG_NUM n, LONG_NUM result) {

    LONG_NUM ZERO_NUM = { 0 };

    int flag = VALID;

    uint32_t temp1[NUMBER_LENGTH + 2];
    LONG_NUM temp2;
    memset(temp1, 0, sizeof(temp1));
    memset(temp2, 0, sizeof(temp2));

    if (number_compare(n, ZERO_NUM) == 0) {

        add(a, b, result);
    }
    else {

        add(a, b, temp1);
        flag = mod_n(temp1, n, temp2);
        number_copy(result, temp2);
    }

    return  flag;
}

void mod_sub(LONG_NUM x, LONG_NUM y, LONG_NUM n,LONG_NUM diff) {

    LONG_NUM temp;
    memset(temp, 0x00, sizeof (temp));

    if (number_compare(x, y) >= 0) {

        sub(x, y, temp);
        mod_n(temp, n, diff);
    }
    else {

        sub(y, x, diff);
        mod_n(diff, n, temp);
        sub(n, temp, diff);
    }
}

void mod_mul(LONG_NUM a, LONG_NUM b, LONG_NUM n, LONG_NUM remainder) {

    LONG_NUM ZERO_NUM = { 0 };

    LONG_NUM temp_a, temp_b;
    DOUBLE_LONG_NUM temp_r;
    memset(temp_a, 0, sizeof(temp_a));
    memset(temp_b, 0, sizeof(temp_b));
    memset(temp_r, 0, sizeof(temp_r));

    if (number_compare(a, ZERO_NUM) == 0 || number_compare(b, ZERO_NUM) == 0) {

        number_copy(remainder, ZERO_NUM);
        return;
    }

    number_copy(temp_a, a);
    number_copy(temp_b, b);

    LONG_NUM mod_a, mod_b;
    memset(mod_a, 0, sizeof(mod_a));
    memset(mod_b, 0, sizeof(mod_b));

    mod_n(temp_a, n, mod_a);
    mod_n(temp_b, n, mod_b);
    mul(mod_a, mod_b, temp_r);
    //    display_number(temp_a);
    //    display_number(temp_b);
    //    display_number(temp_r);

    mod_n(temp_r, n, remainder);

}

int mod_n(DOUBLE_LONG_NUM a, LONG_NUM n, LONG_NUM remainder) {

    LONG_NUM ZERO_NUM = { 0 };

    int flag = VALID;
    DOUBLE_LONG_NUM temp_q;
    LONG_NUM temp_r;

    memset(temp_q, 0, sizeof(temp_q));
    memset(temp_r, 0, sizeof(temp_r));

    if (number_compare(a, ZERO_NUM) == 0) {

        number_copy(remainder, ZERO_NUM);
    }
    else {

        flag = division_v2(a, n, temp_q, temp_r);
        number_copy(remainder, temp_r);
    }

    return flag;
}

int add(LONG_NUM a, LONG_NUM b, LONG_NUM sum) {

    uint32_t temps[NUMBER_LENGTH + 2];
    memset(sum, 0, UNIT_SIZE);
    memset(temps, 0, sizeof(temps));

    int flag = VALID;
    uint32_t* aptr, * bptr, * sptr = temps + 1;
    uint32_t* maptr, * mbptr;
    uint64_t carry = 0;

    if (a[0] < b[0]) {

        aptr = b + 1;
        maptr = b + b[0];
        bptr = a + 1;
        mbptr = a + a[0];
        temps[0] = b[0];
    }
    else {

        aptr = a + 1;
        maptr = a + a[0];
        bptr = b + 1;
        mbptr = b + b[0];
        temps[0] = a[0];
    }

    while (bptr <= mbptr) {

        carry = (uint64_t)*aptr + (uint64_t)*bptr + (uint64_t)(uint32_t)(carry >> WORD_BITS);
        *sptr = (uint32_t)carry;
        aptr++; bptr++; sptr++;
    }

    while (aptr <= maptr) {

        carry = (uint64_t)*aptr + (uint64_t)(uint32_t)(carry >> WORD_BITS);
        *sptr = (uint32_t)carry;
        aptr++; sptr++;
    }

    if (carry & 0x100000000) {
        *sptr = 1;
        temps[0] = temps[0] + 1;
    }

    //    if add overflow
    if (temps[0] > NUMBER_LENGTH) {
        //        printf("\nhere\n");
        temps[0] = NUMBER_LENGTH;
        flag = ADD_OVERFLOW;
    }

    while ((temps[0] > 0) && (temps[temps[0]] == 0)) {
        temps[0] = temps[0] - 1;
    }

    number_copy(sum, temps);
    return flag;
}


int sub(LONG_NUM a, LONG_NUM b, LONG_NUM diff) {

    LONG_NUM ONE_NUM = { 1, 1 };

    uint32_t* aptr, * bptr, * dptr;
    uint32_t* maptr, * mbpr;
    int flag = VALID;
    uint64_t borrow = 0;

    uint32_t temp_a[(NUMBER_LENGTH << 1) + 2];
    DOUBLE_LONG_NUM temp_b;
    memset(temp_a, 0, sizeof(temp_a));
    memset(temp_b, 0, sizeof(temp_b));
    number_copy(temp_a, a);
    number_copy(temp_b, b);

    aptr = a + 1;
    bptr = b + 1;
    dptr = diff + 1;
    maptr = a + a[0];
    mbpr = b + b[0];

    if (number_compare(temp_a, temp_b) == -1) {
        //        printf("\nhere\n");
        set_max(temp_a);
        maptr = temp_a + WORD_BITS;
        diff[0] = WORD_BITS;
        flag = SUB_OVERFLOW;

    }
    else {
        diff[0] = temp_a[0];
    }

    while (bptr <= mbpr) {

        borrow = (uint64_t)*aptr - (uint64_t)*bptr - ((borrow & 0x0000000100000000) >> WORD_BITS);
        *dptr = (uint32_t)borrow;
        aptr++; bptr++; dptr++;
    }

    while (aptr <= maptr) {

        borrow = (uint64_t)*aptr - ((borrow & 0x0000000100000000) >> WORD_BITS);
        *dptr = (uint32_t)borrow;
        aptr++; dptr++;
    }

    if ((diff[0] > 0) && (diff[diff[0]] == 0)) {
        diff[0] = diff[0] - 1;
    }
    if (flag == SUB_OVERFLOW) {
        add(diff, a, diff);
        add(diff, ONE_NUM, diff);
    }

    return flag;
}


int mul(LONG_NUM a, LONG_NUM b, DOUBLE_LONG_NUM prod) {

    int flag = VALID;

    uint32_t* aptr, * bptr, * pptr, * maptr, * mbptr;
    uint32_t* acir, * bcir, * pcir;
    uint32_t a_words;
    uint64_t carry;

    LONG_NUM temp_a, temp_b;
    DOUBLE_LONG_NUM temp_p;
    memset(temp_a, 0, sizeof(temp_a));
    memset(temp_b, 0, sizeof(temp_b));
    //    memset(prod, 0, sizeof (prod));
    memset(temp_p, 0, sizeof(temp_p));

    number_copy(temp_a, a);
    number_copy(temp_b, b);

    if (temp_a[0] == 0 || temp_b[0] == 0) {
        prod[0] = 0;
        return flag;
    }
    if (temp_a[0] < temp_b[0]) {
        aptr = temp_b;
        bptr = temp_a;
    }
    else {
        aptr = temp_a;
        bptr = temp_b;
    }
    maptr = aptr + *aptr;
    mbptr = bptr + *bptr;

    carry = 0;
    a_words = *(aptr + 1);
    //    for (bcir = bptr + 1, pcir = temp_p + 1; bcir <= mbptr; bcir++, pcir++) {
    //        carry = (uint64_t)a_words * (uint64_t)*bcir + (uint64_t)(uint32_t)(carry >> WORD_BITS);
    //        *pcir = (uint32_t)carry;
    //    //  printf("\ncarry = %llu picr1 = %u\n%", carry, *pcir);
    //    }
    //    *pcir = (uint32_t)(carry >> WORD_BITS);

    //    display_number(temp_a);
    //    display_number(temp_b);
    //    display_number(temp_p);
    //    ai*(bn-1bn-2����b1b0)
    for (acir = aptr + 1, pptr = temp_p + 1; acir <= maptr; acir++, pptr++) {
        //  printf("\nhere\n");
        carry = 0;
        a_words = *acir;

        for (bcir = bptr + 1, pcir = pptr; bcir <= mbptr; bcir++, pcir++) {
            carry = (uint64_t)a_words * (uint64_t)*bcir + (uint64_t)*pcir + (uint64_t)(uint32_t)(carry >> WORD_BITS);
            *pcir = (uint32_t)carry;
        }
        *pcir = (uint32_t)(carry >> WORD_BITS);
    }

    temp_p[0] = *aptr + *bptr;
    //    display_number(temp_p);
    while ((temp_p[0] > 0) && (temp_p[temp_p[0]] == 0)) {
        temp_p[0] = temp_p[0] - 1;
    }

    if (temp_p[0] > NUMBER_LENGTH * 2 + 1) {
        //        printf("\nhere\n");
        temp_p[0] = NUMBER_LENGTH * 2;
        while ((temp_p[0] > 0) && (temp_p[temp_p[0]] == 0)) {
            temp_p[0] = temp_p[0] - 1;
        }
        flag = MUL_OVERFLOW;
    }

    number_copy(prod, temp_p);

    return flag;
}



int division(LONG_NUM a, LONG_NUM b, DOUBLE_LONG_NUM q, LONG_NUM remainder) {

    LONG_NUM ONE_NUM = { 1, 1 };
    //    memset(q, 0, sizeof(q));
    //    memset(remainder, 0, sizeof(remainder));

    DOUBLE_LONG_NUM temp_a, temp_b;
    memset(temp_a, 0, sizeof(temp_a));
    memset(temp_b, 0, sizeof(temp_b));
    number_copy(temp_a, a);
    number_copy(temp_b, b);

    DOUBLE_LONG_NUM temp_q, temp_sub, q_t;
    memset(temp_q, 0, sizeof(temp_q));
    memset(temp_sub, 0, sizeof(temp_sub));
    memset(q_t, 0, sizeof(q_t));

    int flag = VALID;
    if (temp_b[0] == 0) {

        flag = DIV_ZERO;
    }
    else if (temp_a[0] == 0) {

        q[0] = q[1] = 0;
        remainder[0] = remainder[1] = 0;
    }
    else if (number_compare(temp_a, temp_b) == -1) {

        //        printf("\nhere\n");
        number_copy(remainder, temp_a);
        q[0] = q[1] = 0;
    }
    else if (number_compare(temp_a, temp_b) == 0) {

        q[0] = q[1] = 1;
        remainder[0] = remainder[1] = 0;
    }
    else {

        q_t[0] = temp_a[0] - temp_b[0] + 1;
        int a_bits = get_bits(temp_a);
        int b_bits = get_bits(temp_b);
        //        printf("\na_bits = %d, b_bits = %d\n", a_bits, b_bits);

        int shiftnum = a_bits - b_bits;
        int subtimes = a_bits - b_bits + 1;

        int shiftwords = shiftnum / 32;

        LONG_NUM temp;
        memset(temp, 0, sizeof(temp));

        temp[0] = temp_b[0] + shiftwords;
        for (int i = (int)temp[0]; i > shiftwords; i--) {
            temp[i] = temp_b[i - shiftwords];
        }
        number_copy(temp_b, temp);

        for (int i = 0; i < shiftnum % 32; i++) {
            //            printf("%d ", i);
            shift_left(temp_b);
        }

        for (int i = 0; i < subtimes; ++i) {

            if (number_compare(temp_a, temp_b) >= 0) {

                sub(temp_a, temp_b, temp_sub);
                number_copy(temp_a, temp_sub);

                shift_left(q_t);
                add(q_t, ONE_NUM, temp_q);
                number_copy(q_t, temp_q);
                shift_right(temp_b);
            }
            else {
                shift_left(q_t);
                shift_right(temp_b);
            }
        }
        //        printf("\ntemp_a:\n");
        //        display_number(temp_a);
        //        printf("\n========\n");

        //        memset(q, 0, sizeof (q));
        number_copy(q, q_t);
        while ((q[0] > 0) && (q[q[0]] == 0)) {
            q[0] = q[0] - 1;
        }
        //        memset(remainder, 0, sizeof (remainder));
        number_copy(remainder, temp_a);
        while ((remainder[0] > 0) && (remainder[remainder[0]] == 0)) {
            remainder[0] = remainder[0] - 1;
        }
    }
    return  flag;
}


int division_v2(LONG_NUM a, LONG_NUM b, DOUBLE_LONG_NUM q, LONG_NUM remainder) {

    memset(q, 0x00, sizeof (q));
    memset(remainder, 0x00, sizeof (remainder));

    DOUBLE_LONG_NUM temp_a, temp_b;
    memset(temp_a, 0x00, sizeof(temp_a));
    memset(temp_b, 0x00, sizeof(temp_b));
    number_copy(temp_a, a);
    number_copy(temp_b, b);

    DOUBLE_LONG_NUM temp_q, temp_sub, q_t;
    memset(temp_q, 0, sizeof(temp_q));
    memset(temp_sub, 0, sizeof(temp_sub));
    memset(q_t, 0, sizeof(q_t));

    int flag = VALID;
    if (temp_b[0] == 0) {

        flag = DIV_ZERO;

    }
    else if (temp_a[0] == 0) {

        q[0] = q[1] = 0;
        remainder[0] = remainder[0] = 0;

    }
    else if (number_compare(temp_a, temp_b) == -1) {

        q[0] = q[1] = 0;
        number_copy(remainder, temp_a);

    }
    else if (number_compare(temp_a, temp_b) == 0) {

        q[0] = q[1] = 1;
        remainder[0] = remainder[1] = 0;

    }
    else {


        if (temp_b[0] == 1) {
            division(temp_a, temp_b, q, remainder);
            return flag;
        }


        uint32_t* bptr, * mbptr;
        uint32_t bn_1 = 0, bn_2 = 0;
        uint32_t scaling_factor_d = 0;

        // bptr = temp_b + 1;
        mbptr = temp_b + temp_b[0];
        bn_1 = *mbptr;



        // TO DO
        // shift b left scaling factor bits to make V_{n - 1} < Base / 2, just thinking about the heightest two words.

        while (bn_1 < 0x80000000) {
            bn_1 = bn_1 << 1;
            scaling_factor_d++;
        }
        uint64_t shiftr_num = (int)(WORD_BITS - scaling_factor_d);

        if (scaling_factor_d > 0) {
            bn_1 = bn_1 + (uint32_t)((uint64_t) * (mbptr - 1) >> shiftr_num);

            if (temp_b[0] > 2) {
                bn_2 = (uint32_t)((uint32_t)((uint64_t)(*(mbptr - 1)) << scaling_factor_d) + (uint32_t)((uint64_t)(*(mbptr - 2)) >> shiftr_num));
            }
            else {
                bn_2 = (uint32_t)((uint64_t) * (mbptr - 1) << scaling_factor_d);
            }

        }
        else {
            bn_2 = (uint32_t)(*(mbptr - 1));
        }


        mbptr = temp_b + temp_b[0];

        uint32_t* aptr, * maptr;
        uint32_t* acir, * qptr;

        // Str r = (r_{m + n},r_{m + n - 1},r_{m + n - 2}, ... ,r_{0}) <-- (0,a_{m + n - 1},a_{m + n - 2}, ... ,a_{0})
        maptr = temp_a + temp_a[0] + 1;
        aptr = temp_a + temp_a[0] - temp_b[0] + 1;
        *maptr = 0;

        qptr = q + temp_a[0] - temp_b[0] + 1;

        uint32_t r0 = 0, r1 = 0, r2 = 0;
        uint32_t q_hat = 0;

        uint64_t kuo = 0;
        uint64_t left = 0, right = 0;
        uint64_t borrow = 0, carry = 0;



        while (aptr >= temp_a + 1) {

            // shift r left scaling factor bits the same as b
            r0 = (uint32_t)((uint32_t)((uint64_t)(*maptr) << scaling_factor_d) + (uint32_t)((uint64_t)(*(maptr - 1)) >> shiftr_num));
            r1 = (uint32_t)(((uint32_t)((uint64_t)(*(maptr - 1)) << scaling_factor_d) + (uint32_t)((uint64_t)(*(maptr - 2)) >> shiftr_num)));

            if (maptr - 3 > temp_a) {

                r2 = (uint32_t)(((uint32_t)((uint64_t)(*(maptr - 2)) << scaling_factor_d) + (uint32_t)((uint64_t)(*(maptr - 3)) >> shiftr_num)));
            }
            else {

                r2 = (uint32_t)((uint64_t)(*(maptr - 2)) << scaling_factor_d);
            }

            kuo = (uint64_t)((((uint64_t)r0 << WORD_BITS) + (uint64_t)r1) / bn_1);
            if (kuo < 0xffffffffU) {

                q_hat = (uint32_t)kuo;
            }
            else {

                q_hat = 0xffffffffU;
            }

            kuo = ((uint64_t)r0 << WORD_BITS) + (uint64_t)r1 - (uint64_t)bn_1 * (uint64_t)q_hat;

            if (kuo < 0x100000000U) {

                right = (uint64_t)(kuo << WORD_BITS) + (uint64_t)r2;
                left = (uint64_t)bn_2 * (uint64_t)q_hat;

                if (left > right) {

                    q_hat--;

                    kuo = ((uint64_t)r0 << WORD_BITS) + (uint64_t)r1 - (uint64_t)bn_1 * (uint64_t)q_hat;

                    if (kuo < 0x100000000U) {

                        right = (uint64_t)(kuo << WORD_BITS) + (uint64_t)r2;
                        left = (uint64_t)bn_2 * (uint64_t)q_hat;
                        if (left > right) {
                            q_hat--;
                        }

                    }
                }

            }

            borrow = 0x100000000U;
            carry = 0;

            for (bptr = temp_b + 1, acir = aptr; bptr <= mbptr; bptr++, acir++) {
                if (borrow >= 0x100000000U) {

                    carry = (uint64_t)q_hat * (uint64_t)*bptr + (uint64_t)(uint32_t)(carry >> WORD_BITS);
                    borrow = (uint64_t)*acir + 0x100000000U - (uint64_t)(uint32_t)carry;
                    *acir = (uint32_t)(borrow);
                }
                else {

                    carry = (uint64_t)q_hat * (uint64_t)*bptr + (uint64_t)(uint32_t)(carry >> WORD_BITS);
                    borrow = (uint64_t)*acir + (uint64_t)0x100000000U - (uint64_t)(uint32_t)carry - 1ULL;
                    *acir = (uint32_t)(borrow);
                }
            }

            if (borrow >= 0x100000000U) {
                borrow = (uint64_t)*acir + 0x100000000U - (uint64_t)(uint32_t)(carry >> WORD_BITS);
                *acir = (uint32_t)(borrow);
            }
            else {
                borrow = (uint64_t)*acir + 0x100000000U - (uint64_t)(uint32_t)(carry >> WORD_BITS) - 1ULL;
                *acir = (uint32_t)(borrow);
            }
            *qptr = q_hat;

            if (borrow < 0x100000000U) {
                carry = 0;
                for (bptr = temp_b + 1, acir = aptr; bptr <= mbptr; bptr++, acir++) {
                    carry = ((uint64_t)*acir + (uint64_t)*bptr + (uint64_t)(uint32_t)(carry >> WORD_BITS));
                    *acir = (uint32_t)carry;
                }
                *acir = *acir + (uint32_t)(carry >> WORD_BITS);
                *qptr = *qptr - 1;
            }
            qptr--; maptr--; aptr--;

        }
        q[0] = temp_a[0];
        while ((q[0] > 0) && (q[q[0]] == 0)) {
            q[0] = q[0] - 1;
        }
        //        memset(remainder, 0, sizeof (remainder));
        number_copy(remainder, temp_a);
        while ((remainder[0] > 0) && (remainder[remainder[0]] == 0)) {
            remainder[0] = remainder[0] - 1;
        }

    }


    return flag;
}



//int shift_left(LONG_NUM a, uint32_t nums) {
//
//    if (nums >NUMBER_LENGTH) {}
//
//    LONG_NUM temp_a;
//    memset(temp_a, 0, sizeof (a));
//    uint32_t cnt;
//    uint64_t carry;
//
//}

int shift_left(DOUBLE_LONG_NUM a) {

    DOUBLE_LONG_NUM temp_a;
    uint32_t* aptr, * maptr;
    uint64_t carry = 0;
    int flag = VALID;

    memset(temp_a, 0, sizeof(temp_a));
    number_copy(temp_a, a);
    while ((temp_a[0] > 0) && (temp_a[temp_a[0]] == 0)) {
        temp_a[0] = temp_a[0] - 1;
    }

    if ((uint32_t)get_bits(temp_a) >= (uint32_t)((NUMBER_LENGTH + 1) << 6)) {
        printf("\nhere\n");
        temp_a[0] = NUMBER_LENGTH * 2 + 1;
        flag = ADD_OVERFLOW;
    }

    maptr = temp_a + temp_a[0];
    for (aptr = temp_a + 1; aptr <= maptr; ++aptr) {
        carry = ((uint64_t)*aptr << 1) | (carry >> WORD_BITS);
        *aptr = (uint32_t)carry;
    }

    if (carry >> WORD_BITS) {

        if (temp_a[0] < ((NUMBER_LENGTH << 1) + 1)) {
            *aptr = 1;
            temp_a[0] = temp_a[0] + 1;
            flag = VALID;
        }
        else {
            printf("\nhere\n");
            flag = ADD_OVERFLOW;
        }
    }

    while ((temp_a[0] > 0) && (temp_a[temp_a[0]] == 0)) {
        temp_a[0] = temp_a[0] - 1;
    }
    number_copy(a, temp_a);

    return flag;
}


int shift_right(DOUBLE_LONG_NUM a) {

    int flag = VALID;
    DOUBLE_LONG_NUM temp_a;
    uint32_t* aptr;
    uint32_t current, carry = 0;

    memset(temp_a, 0, sizeof(temp_a));
    number_copy(temp_a, a);

    if (temp_a[0] == 0) {
        flag = SUB_OVERFLOW;
    }

    for (aptr = temp_a + temp_a[0]; aptr > temp_a; --aptr) {

        current = (uint32_t)((uint32_t)(*aptr >> 1) | (uint32_t)(carry << (WORD_BITS - 1)));
        carry = (uint32_t)(*aptr & 1);
        *aptr = current;
    }

    while ((temp_a[0] > 0) && (temp_a[temp_a[0]] == 0)) {
        temp_a[0] = temp_a[0] - 1;
    }
    number_copy(a, temp_a);
    return flag;
}


void set_max(LONG_NUM a) {
    uint32_t* aptr = a;
    uint32_t* maptr = a + NUMBER_LENGTH;

    while (aptr <= maptr) {
        *aptr = 0xffffffff;
        aptr++;
    }
    a[0] = NUMBER_LENGTH;
}


int number_compare(LONG_NUM a, LONG_NUM b) {
    uint32_t* maptr, * mbptr;
    int length_a = (int)a[0];
    int length_b = (int)b[0];

    if (length_a == 0 && length_b == 0) {
        return 0;
    }
    while (a[length_a] == 0 && length_a > 0) {
        --length_a;
    }
    while (b[length_b] == 0 && length_b > 0) {
        --length_b;
    }

    if (length_a == 0 && length_b == 0) {
        return 0;
    }
    if (length_a > length_b) {
        return 1;
    }
    if (length_a < length_b) {
        return -1;
    }

    maptr = a + length_a;
    mbptr = b + length_b;

    while ((*maptr == *mbptr) && (maptr > a)) {
        maptr--;
        mbptr--;
    }

    if (maptr == a) {
        return 0;
    }
    if (*maptr > *mbptr) {
        return 1;
    }
    else {
        return -1;
    }
}


void display_number(LONG_NUM a) {

    printf("%x: ", a[0]);
    for (int i = a[0]; i > 0; i--) {
        printf("%08x ", a[i]);
    }
    printf("\n");
}

int get_bits(const LONG_NUM a) {

    uint32_t bits = a[0];
    uint32_t high;

    while (a[bits] == 0 && bits > 0) {
        --bits;
    }
    high = a[bits];

    if (high == 0 && bits == 0) {
        return 0;
    }

    bits = bits << 5;
    uint32_t bit32 = 0x80000000;
    while ((high & bit32) == 0) {

        high = high << 1;
        bits--;
    }

    return (int)bits;
}


void number_copy(LONG_NUM destination, LONG_NUM source) {

    uint32_t* last_source = source + source[0];
    destination[0] = source[0];

    while ((*last_source == 0) && (destination[0] > 0)) {

        last_source = last_source - 1;
        destination[0] = destination[0] - 1;
    }

    for (int i = 1; i <= destination[0]; ++i) {
        destination[i] = source[i];
    }

}