//
// Created by 24208 on 2023/10/26.
//

#include "ellipticcurve_calculation.h"

extern LONG_NUM n;
extern LONG_NUM p;
extern LONG_NUM a;
extern LONG_NUM b;
extern LONG_NUM Gx;
extern LONG_NUM Gy;


void affine_coordinates_addition(LONG_NUM Px, LONG_NUM Py, LONG_NUM Qx, LONG_NUM Qy, LONG_NUM Rx, LONG_NUM Ry) {

    LONG_NUM x_diff, y_diff, x_inverse;
    LONG_NUM lamda;
    memset(x_diff, 0x00, sizeof (x_diff));
    memset(y_diff, 0x00, sizeof (y_diff));
    memset(x_inverse, 0x00, sizeof (x_inverse));
    memset(lamda, 0x00, sizeof (lamda));

//    \lamda = \frac{y_2 - y_1}{x_2 - x_1}
    mod_sub(Qx, Px, p, x_diff);
    mod_sub(Qy, Py, p, y_diff);
    inverse(x_diff, p, x_inverse);
    mod_mul(y_diff, x_inverse, p, lamda);

    LONG_NUM x_sum, x_diff2;
    LONG_NUM lamda_square, lamda_prod;
    memset(x_sum, 0x00, sizeof (x_sum));
    memset(x_diff2, 0x00, sizeof (x_diff2));
    memset(lamda_square, 0x00, sizeof (lamda_square));
    memset(lamda_prod, 0x00, sizeof (lamda_prod));

//    x_3 = \lamda^2 - x_1 - x_2
    add(Px, Qx, x_sum);
    mul(lamda, lamda, lamda_square);
    mod_sub(lamda_square, x_sum, p, Rx);

//    y_3 = \lamda * (x_1 - x_3) - y_1
    mod_sub(Px, Rx, p, x_diff2);
    mul(lamda, x_diff2, lamda_prod);
    mod_sub(lamda_prod, Py, p, Ry);

}


void affine_coordinates_doubling(LONG_NUM Px, LONG_NUM Py, LONG_NUM Rx, LONG_NUM Ry) {

    LONG_NUM TWO_NUM = { 1, 2 };
    LONG_NUM THREE_NUM = { 1, 3 };

    LONG_NUM x_square, y_inverse;
    LONG_NUM lamda;
    memset(x_square, 0x00, sizeof (x_square));
    memset(y_inverse, 0x00, sizeof (y_inverse));
    memset(lamda, 0x00, sizeof (lamda));

    LONG_NUM temp1, temp2;
    memset(temp1, 0x00, sizeof (temp1));
    memset(temp2, 0x00, sizeof (temp2));

//    \lamda = \frac{3 * x_1^2 - 3}{2 & y_1}
    mul(Px, Px, x_square);
    mul(x_square, THREE_NUM, temp1);
    add(temp1, a, temp2);

    mul(Py, TWO_NUM, temp1);
    inverse(temp1, p, y_inverse);
    mod_mul(temp2, y_inverse, p, lamda);

    LONG_NUM x_sum, x_diff;
    LONG_NUM lamda_square, lamda_prod;
    memset(x_sum, 0x00, sizeof (x_sum));
    memset(x_diff, 0x00, sizeof (x_diff));
    memset(lamda_square, 0x00, sizeof (lamda_square));
    memset(lamda_prod, 0x00, sizeof (lamda_prod));

//    x_3 = \lamda^2 - x_1 - x_2
    add(Px, Px, x_sum);
    mul(lamda, lamda, lamda_square);
    mod_sub(lamda_square, x_sum, p, Rx);

//    y_3 = \lamda * (x_1 - x_3) - y_1
    mod_sub(Px, Rx, p, x_diff);
    mul(lamda, x_diff, lamda_prod);
    mod_sub(lamda_prod, Py, p, Ry);

}

void point_mul_binary(LONG_NUM k, LONG_NUM Px, LONG_NUM Py, LONG_NUM Rx, LONG_NUM Ry) {

    LONG_NUM temp_x, temp_y;
    LONG_NUM Qx, Qy;
    memset(temp_x, 0x00, sizeof (temp_x));
    memset(temp_y, 0x00, sizeof (temp_y));
    memset(Qx, 0x00, sizeof (Qx));
    memset(Qy, 0x00, sizeof (Qy));

    number_copy(Qx, Px);
    number_copy(Qy, Py);

    uint32_t *kptr, *mkptr;
    kptr = k + 1;
    mkptr = k + k[0];

    char k_binary[33];

    memset(k_binary, 0x00, sizeof (k_binary));
    _ultoa(*mkptr, k_binary, 2);

    int k_length = (int)strlen(k_binary);

    for (int i = 1; i < k_length; i++) {

        affine_coordinates_doubling(Qx, Qy, temp_x, temp_y);
        number_copy(Qx, temp_x);
        number_copy(Qy, temp_y);

        if (k_binary[i] == '1') {

            affine_coordinates_addition(Px, Py, Qx, Qy, temp_x, temp_y);
            number_copy(Qx, temp_x);
            number_copy(Qy, temp_y);
        }
    }
    mkptr = k + k[0] - 1;

    while (kptr <= mkptr) {

        memset(k_binary, 0x00, sizeof (k_binary));
        _ultoa(*mkptr, k_binary, 2);

        int idx = 0;
        k_length = (int)strlen(k_binary);

        for (int j = 32; j > 0; j--) {

//            Q = 2 * Q
            affine_coordinates_doubling(Qx, Qy, temp_x, temp_y);
            number_copy(Qx, temp_x);
            number_copy(Qy, temp_y);

            if (j <= k_length) {
                if (k_binary[idx] == '1') {
//                    Q = P + Q
                    affine_coordinates_addition(Px, Py, Qx, Qy, temp_x, temp_y);
                    number_copy(Qx, temp_x);
                    number_copy(Qy, temp_y);

                }
                idx++;
            }

        }

        mkptr--;
    }

    number_copy(Rx, Qx);
    number_copy(Ry, Qy);

}

void jacobian_coordinates_doubling(LONG_NUM Px, LONG_NUM Py, LONG_NUM Pz, LONG_NUM Rx, LONG_NUM Ry, LONG_NUM Rz) {

    LONG_NUM TWO_NUM = { 1, 2 };
    LONG_NUM THREE_NUM = { 1, 3 };
    LONG_NUM FOUR_NUM = { 1, 4 };
    LONG_NUM EIGHT_NUM = { 1, 8 };

    LONG_NUM A, B, C, D;
    memset(A, 0x00, sizeof (A));
    memset(B, 0x00, sizeof (B));
    memset(C, 0x00, sizeof (C));
    memset(D, 0x00, sizeof (D));

    LONG_NUM y_square, y_quartic, z_square, z_quartic;
    LONG_NUM temp1, temp2, temp3;
    memset(y_square, 0x00, sizeof (y_square));
    memset(y_quartic, 0x00, sizeof (y_quartic));
    memset(z_square, 0x00, sizeof (z_square));
    memset(z_quartic, 0x00, sizeof (z_quartic));
    memset(temp1, 0x00, sizeof (temp1));
    memset(temp2, 0x00, sizeof (temp2));
    memset(temp3, 0x00, sizeof (temp3));

//    A = 4 * X_1 * Y_1^2
    mul(Py, Py, y_square);
    mul(y_square, Px, temp1);
    mod_mul(temp1, FOUR_NUM, p, A);

//    B = 8 * Y_1^4
    mul(y_square, y_square, y_quartic);
    mod_mul(y_quartic, EIGHT_NUM, p, B);

//    C = 3 * X_1^2 + a * Z_1^4
    memset(temp1, 0x00, sizeof (temp1));
    memset(temp2, 0x00, sizeof (temp2));
    mul(Px, Px, temp1);
    mod_mul(temp1, THREE_NUM, p, temp2);
    mul(Pz, Pz, z_square);
    mod_mul(z_square, z_square, p, z_quartic);
    mod_mul(z_quartic, a, p, temp3);
    mod_add(temp2, temp3, p, C);

//    D = -2 * A + C^2
    memset(temp1, 0x00, sizeof (temp1));
    memset(temp2, 0x00, sizeof (temp2));
    mul(C, C, temp1);
    mul(A, TWO_NUM, temp2);
    mod_sub(temp1, temp2, p, D);

//    X_3 = D
    number_copy(Rx, D);

//    Y_3 = C * (A - D) - B
    memset(temp1, 0x00, sizeof (temp1));
    memset(temp2, 0x00, sizeof (temp2));
    mod_sub(A, D, p, temp1);
    mul(C, temp1, temp2);
    mod_sub(temp2, B, p, Ry);

//    Z_3 = 2 * Y_1 * Z_1
    memset(temp3, 0x00, sizeof (temp3));
    mul(Py, Pz, temp3);
    mod_mul(temp3, TWO_NUM, p, Rz);

}

void mixed_jacobian_affine(LONG_NUM Px, LONG_NUM Py, LONG_NUM Pz, LONG_NUM Qx, LONG_NUM Qy, LONG_NUM Rx, LONG_NUM Ry, LONG_NUM Rz) {

    LONG_NUM TWO_NUM = { 1, 2 };

    LONG_NUM A, B, C, D;
    memset(A, 0x00, sizeof (A));
    memset(B, 0x00, sizeof (B));
    memset(C, 0x00, sizeof (C));
    memset(D, 0x00, sizeof (D));

    LONG_NUM z_square, C_square, C_cube;
    memset(z_square, 0x00, sizeof (z_square));
    memset(C_square, 0x00, sizeof (C_square));
    memset(C_cube, 0x00, sizeof (C_cube));

    LONG_NUM temp1, temp2, temp3;
    memset(temp1, 0x00, sizeof (temp1));
    memset(temp2, 0x00, sizeof (temp2));
    memset(temp3, 0x00, sizeof (temp3));

//    A = X_2 * Z_1^2
    mul(Pz, Pz, z_square);
    mod_mul(Qx, z_square, p, A);

//    B = Y_2 * Z_1^3
    mul(z_square, Pz, temp1);
    mod_mul(Qy, temp1, p, B);

//    C = A - X_1
    mod_sub(A, Px, p, C);

//    D = B - Y_1
    mod_sub(B, Py, p, D);

//    X_3 = D^2 - (C^3 + 2 * X_1 * C^2)
    memset(temp1, 0x00, sizeof (temp1));
    mul(C, C, C_square);
    mul(C, C_square, C_cube);
    mod_mul(Px, C_square, p, temp1);
    mod_mul(temp1, TWO_NUM, p, temp2);
    add(C_cube, temp2, temp3);
    memset(temp1, 0x00, sizeof (temp1));
    mul(D, D, temp1);
    mod_sub(temp1, temp3, p, Rx);

//    Y_3 = D * (X_1 * C^2 - X_3) - Y_1 * C^3
    memset(temp1, 0x00, sizeof (temp1));
    memset(temp2, 0x00, sizeof (temp2));
    memset(temp3, 0x00, sizeof (temp3));
    mul(Px, C_square, temp1);
    mod_sub(temp1, Rx, p, temp2);
    mul(D, temp2, temp3);
    memset(temp1, 0x00, sizeof (temp1));
    mul(Py, C_cube, temp1);
    mod_sub(temp3, temp1, p, Ry);

//    Z_3 = Z_1 * C
    mod_mul(Pz, C, p, Rz);

}

void jacobian_affine_point_mul(LONG_NUM k, LONG_NUM Px, LONG_NUM Py, LONG_NUM Rx, LONG_NUM Ry) {

    LONG_NUM temp_x, temp_y, temp_z;
    LONG_NUM Qx, Qy, Qz;
    memset(temp_x, 0x00, sizeof (temp_x));
    memset(temp_y, 0x00, sizeof (temp_y));
    memset(temp_z, 0x00, sizeof (temp_z));
    memset(Qx, 0x00, sizeof (Qx));
    memset(Qy, 0x00, sizeof (Qy));
    memset(Qz, 0x00, sizeof (Qz));

    number_copy(Qx, Px);
    number_copy(Qy, Py);
    Qz[0] = 1;
    Qz[1] = 1;


    uint32_t *kptr, *mkptr;
    kptr = k + 1;
    mkptr = k + k[0];

    char k_binary[33];

    memset(k_binary, 0x00, sizeof (k_binary));
    _ultoa(*mkptr, k_binary, 2);

    int k_length = (int)strlen(k_binary);

    for (int i = 1; i < k_length; i++) {
        jacobian_coordinates_doubling(Qx, Qy, Qz, temp_x, temp_y, temp_z);
        number_copy(Qx, temp_x);
        number_copy(Qy, temp_y);
        number_copy(Qz, temp_z);

        if (k_binary[i] == '1') {
            mixed_jacobian_affine(Qx, Qy, Qz, Px, Py, temp_x, temp_y, temp_z);
            number_copy(Qx, temp_x);
            number_copy(Qy, temp_y);
            number_copy(Qz, temp_z);
        }
    }
    mkptr = k + k[0] - 1;

    while (kptr <= mkptr) {

        memset(k_binary, 0x00, sizeof (k_binary));
        _ultoa(*mkptr, k_binary, 2);

        int idx = 0;
        k_length = (int)strlen(k_binary);

        for (int j = 32; j > 0; j--) {
//            Q = 2 * Q
            jacobian_coordinates_doubling(Qx, Qy, Qz, temp_x, temp_y, temp_z);
            number_copy(Qx, temp_x);
            number_copy(Qy, temp_y);
            number_copy(Qz, temp_z);

            if (j <= k_length) {
                if (k_binary[idx] == '1') {
//                    Q = P + Q
                    mixed_jacobian_affine(Qx, Qy, Qz, Px, Py, temp_x, temp_y, temp_z);
                    number_copy(Qx, temp_x);
                    number_copy(Qy, temp_y);
                    number_copy(Qz, temp_z);
                }
                idx++;
            }

        }

        mkptr--;
    }

    LONG_NUM z2, z3, z2_inverse, z3_inverse;
    memset(z2, 0x00, sizeof (z2));
    memset(z3, 0x00, sizeof (z3));
    memset(z2_inverse, 0x00, sizeof (z2_inverse));
    memset(z3_inverse, 0x00, sizeof (z3_inverse));

    mod_mul(Qz, Qz, p, z2);
    mod_mul(z2, Qz, p, z3);
    inverse(z2, p, z2_inverse);
    inverse(z3, p, z3_inverse);

    mod_mul(Qx, z2_inverse, p, Rx);
    mod_mul(Qy, z3_inverse, p, Ry);

}

void naf_trans(LONG_NUM k, uint32_t w, int *k_w_width, int *length) {

    LONG_NUM ONE_NUM = { 1, 1 };

    LONG_NUM temp_k;
    memset(temp_k, 0x00, sizeof (temp_k));
    number_copy(temp_k, k);

    LONG_NUM temp1, temp2;
    memset(temp1, 0x00, sizeof (temp1));
    memset(temp2, 0x00, sizeof (temp2));

    uint32_t w_width = (uint32_t) pow(2, w);
    int i = 0;

    while (number_compare(temp_k, ONE_NUM) >= 0) {

        if ((temp_k[1] & 1) == 1) {

//            k_w_width[i] = (int)w_width / 2 - (int)(k % w_width);
//            k = k - k_w_width[i];
            k_w_width[i] = (int)(temp_k[1] & (w_width - 1));

            if (k_w_width[i] >= w_width / 2) {
                k_w_width[i] = k_w_width[i] - (int)w_width;
                temp1[0] = 1;
                temp1[1] = -k_w_width[i];
                if (temp1[1] > 15) {
                    printf("here");
                }
                add(temp_k, temp1, temp2);
                number_copy(temp_k, temp2);

            }
            else {
                temp1[0] = 1;
                temp1[1] = k_w_width[i];
                sub(temp_k, temp1, temp2);
                number_copy(temp_k, temp2);
            }
        }
        else {
            k_w_width[i] = 0;
        }

        shift_right(temp_k);
        i++;
    }
    *length = i;
}

void naf_point_mul(LONG_NUM k, LONG_NUM Px, LONG_NUM Py, LONG_NUM Rx, LONG_NUM Ry) {

    LONG_NUM THREE_NUM = { 1, 3 };
    LONG_NUM FIVE_NUM = { 1, 5 };
    LONG_NUM SEVEN_NUM = { 1, 7 };

    LONG_NUM temp_x, temp_y, temp_z;
    LONG_NUM Qx, Qy, Qz;
    memset(temp_x, 0x00, sizeof (temp_x));
    memset(temp_y, 0x00, sizeof (temp_y));
    memset(temp_z, 0x00, sizeof (temp_z));
    memset(Qx, 0x00, sizeof (Qx));
    memset(Qy, 0x00, sizeof (Qy));
    memset(Qz, 0x00, sizeof (Qz));

    uint32_t w = 4;
    LONG_NUM pre_x[8], pre_y[8];
    for (int i = 0; i < 8; i++) {
        memset(pre_x[i], 0x00, sizeof (pre_x[i]));
        memset(pre_y[i], 0x00, sizeof (pre_y[i]));
    }

    number_copy(pre_x[1], Px);
    number_copy(pre_y[1], Py);
    jacobian_affine_point_mul(THREE_NUM, Px, Py, pre_x[3], pre_y[3]);
    jacobian_affine_point_mul(FIVE_NUM, Px, Py, pre_x[5], pre_y[5]);
    jacobian_affine_point_mul(SEVEN_NUM, Px, Py, pre_x[7], pre_y[7]);
    for (int i = 0; i < 8; i += 2) {
        number_copy(pre_x[i], pre_x[i + 1]);
        mod_sub(p, pre_y[i + 1], p, pre_y[i]);
    }

//    for (int i = 0; i < 8; ++i) {
//        printf("\n%dth:\n", i);
//        display_number(pre_x[i]);
//        display_number(pre_y[i]);
//    }

    int k_w_width[256];
    int k_length;
    memset(k_w_width, 0x00, sizeof (k_w_width));
    naf_trans(k, w, k_w_width, &k_length);

    number_copy(Qx, pre_x[k_w_width[k_length - 1]]);
    number_copy(Qy, pre_y[k_w_width[k_length - 1]]);
    Qz[0] = 1;
    Qz[1] = 1;


    for (int i = k_length - 2; i >= 0; i--) {

        jacobian_coordinates_doubling(Qx, Qy, Qz, temp_x, temp_y, temp_z);
        number_copy(Qx, temp_x);
        number_copy(Qy, temp_y);
        number_copy(Qz, temp_z);

        int index = k_w_width[i];

        if (index != 0) {
            if (index > 0) {
                mixed_jacobian_affine(Qx, Qy, Qz, pre_x[index], pre_y[index], temp_x, temp_y, temp_z);
                number_copy(Qx, temp_x);
                number_copy(Qy, temp_y);
                number_copy(Qz, temp_z);
            }
            else {
                mixed_jacobian_affine(Qx, Qy, Qz, pre_x[-index - 1], pre_y[-index - 1], temp_x, temp_y, temp_z);
                number_copy(Qx, temp_x);
                number_copy(Qy, temp_y);
                number_copy(Qz, temp_z);
            }
        }

    }


//    int idx;
//    for (int i = k_length - 2; i >= 0; i--) {
//        jacobian_coordinates_doubling(Qx, Qy, Qz, temp_x, temp_y, temp_z);
//        number_copy(Qx, temp_x);
//        number_copy(Qy, temp_y);
//        number_copy(Qz, temp_z);
//
//        idx = k_w_width[i];
//        if (idx != 0) {
//
//            if (idx > 0) {
//                mixed_jacobian_affine(Qx, Qy, Qz, pre_x[idx], pre_y[idx], temp_x, temp_y, temp_z);
//                number_copy(Qx, temp_x);
//                number_copy(Qy, temp_y);
//                number_copy(Qz, temp_z);
//            }
//            else {
//                mixed_jacobian_affine(Qx, Qy, Qz, pre_x[-idx - 1], pre_y[-idx - 1], temp_x, temp_y, temp_z);
//                number_copy(Qx, temp_x);
//                number_copy(Qy, temp_y);
//                number_copy(Qz, temp_z);
//            }
//
//        }
//    }
//    mkptr = k + k[0] - 1;
//
//    while (kptr <= mkptr) {
//
//        memset(k_w_width, 0x00, sizeof (k_w_width));
//        naf_trans(*mkptr, w, k_w_width, &k_length);
//
//        for (int i = k_length - 2; i >= 0; i--) {
//            jacobian_coordinates_doubling(Qx, Qy, Qz, temp_x, temp_y, temp_z);
//            number_copy(Qx, temp_x);
//            number_copy(Qy, temp_y);
//            number_copy(Qz, temp_z);
//
//            idx = k_w_width[i];
//            if (idx != 0) {
//
//                if (idx > 0) {
//                    mixed_jacobian_affine(Qx, Qy, Qz, pre_x[idx], pre_y[idx], temp_x, temp_y, temp_z);
//                    number_copy(Qx, temp_x);
//                    number_copy(Qy, temp_y);
//                    number_copy(Qz, temp_z);
//                }
//                else {
//                    mixed_jacobian_affine(Qx, Qy, Qz, pre_x[-idx - 1], pre_y[-idx - 1], temp_x, temp_y, temp_z);
//                    number_copy(Qx, temp_x);
//                    number_copy(Qy, temp_y);
//                    number_copy(Qz, temp_z);
//                }
//
//            }
//        }
//
//        mkptr--;
//    }

    LONG_NUM z2, z3, z2_inverse, z3_inverse;
    memset(z2, 0x00, sizeof (z2));
    memset(z3, 0x00, sizeof (z3));
    memset(z2_inverse, 0x00, sizeof (z2_inverse));
    memset(z3_inverse, 0x00, sizeof (z3_inverse));

    mod_mul(Qz, Qz, p, z2);
    mod_mul(z2, Qz, p, z3);
    inverse(z2, p, z2_inverse);
    inverse(z3, p, z3_inverse);

    mod_mul(Qx, z2_inverse, p, Rx);
    mod_mul(Qy, z3_inverse, p, Ry);

}