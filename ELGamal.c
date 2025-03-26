//
// Created by 24208 on 2023/10/26.
//

#include "ELGamal.h"

extern LONG_NUM n;
extern LONG_NUM p;
extern LONG_NUM a;
extern LONG_NUM b;
extern LONG_NUM Gx;
extern LONG_NUM Gy;


void ELGamal() {

    LONG_NUM fm, m, c;
    memset(fm, 0x00, sizeof (fm));
    memset(m, 0x00, sizeof (m));
    memset(c, 0x00, sizeof (c));

    double run_time;
    union _LARGE_INTEGER time_start;	//开始时间
    union _LARGE_INTEGER time_over;	    //结束时间
    double dqFreq;		                //计时器频率
    LARGE_INTEGER fr;	                //计时器频率
    QueryPerformanceFrequency(&fr);
    dqFreq=(double)fr.QuadPart;

//    fm[0] = 1;
//    fm[1] = 965181398;
    unsigned char str[] = "abcdefghijklmnopqrstuvwxyz";
    fm[0] = NUMBER_LENGTH;
    str_to_num(fm, str, (int) strlen(str));
    printf("The plaintext:\n");
    display_number(fm);


    init_random_seed();

    unsigned char random_str[STR_SIZE];
    memset(random_str, 0x00, sizeof (random_str));

//    LONG_NUM temp;
//    memset(temp, 0x00, sizeof (temp));
//
//    LONG_NUM private_key_d;
//    memset(private_key_d, 0x00, sizeof (private_key_d));
//    // TO DO
//    // Random nunber generate
//    random_generate(random_str, STR_SIZE);
//    temp[0] = 8;
//    str_to_num(temp, random_str);
//
//    mod_n(temp, n, private_key_d);
//    printf("private key d:\n");
//    display_number(private_key_d);

    LONG_NUM private_key_d = {
            8, 0x20bb0da0, 0x08ddbc29, 0xb89463f2, 0x34aa7f7c, 0x3fbf3535, 0x5e2efe28, 0xa00637bd, 0x1649ab77
    };

    LONG_NUM Qx, Qy;
    memset(Qx, 0x00, sizeof (Qx));
    memset(Qy, 0x00, sizeof (Qy));

//    point_mul_binary(private_key_d, Gx, Gy, Qx, Qy);
//    jacobian_affine_point_mul(private_key_d, Gx, Gy, Qx, Qy);

    clock_t start, finish;
    double interval;
    start = clock();
    for (int i = 0; i < 10; i++) {
        naf_point_mul(private_key_d, Gx, Gy, Qx, Qy);
//        point_mul_binary(private_key_d, Gx, Gy, Qx, Qy);
    }

    finish = clock();
    interval = (double)(finish - start);
    printf("===================== \n 10 times operation takes %lf ms\n ================== \n", interval);



    printf("Public key (Qx, Qy):\n");
    display_number(Qx);
    display_number(Qy);

    LONG_NUM x_1, y_1;
    memset(x_1, 0x00, sizeof (x_1));
    memset(y_1, 0x00, sizeof (y_1));

    QueryPerformanceCounter(&time_start);
    ELGamal_encryption(Qx, Qy, x_1, y_1, fm, c);
    QueryPerformanceCounter(&time_over);
    run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
    printf("the ciphertext:\n");
    display_number(c);
    printf("run_time: %lf us\n\n",run_time);


    QueryPerformanceCounter(&time_start);
    ELGamal_decryption(x_1, y_1, private_key_d, c, m);
    QueryPerformanceCounter(&time_over);
    run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
    printf("the m after decryption:\n");
    display_number(m);
    printf("run_time: %lf us\n\n",run_time);


}

void ELGamal_encryption(LONG_NUM Qx, LONG_NUM Qy, LONG_NUM x_1, LONG_NUM y_1, LONG_NUM m, LONG_NUM c) {

//    LONG_NUM k = {
//            8, 0xa3711079, 0x0093fdd9, 0x4451faa2, 0x9502ceba, 0x1c92c9af, 0x53a8fd99, 0x99227ba2, 0x7e6b4b61
//    };

    LONG_NUM k;
    LONG_NUM temp;
    memset(k, 0x00, sizeof (k));
    memset(temp, 0x00, sizeof (temp));
    unsigned char random_str[STR_SIZE];
    memset(random_str, 0x00, sizeof (random_str));
    // TO DO
    // Random nunber k generate
    random_generate(random_str, STR_SIZE);
    temp[0] = 8;
    str_to_num(temp, random_str, STR_SIZE);
    mod_n(temp, n, k);
    printf("random number k:\n");
    display_number(k);

    LONG_NUM x_2, y_2;
    memset(x_2, 0x00, sizeof (x_2));
    memset(y_2, 0x00, sizeof (y_2));

//    affine coordinates:
//    point_mul_binary(k, Gx, Gy, x_1, y_1);
//    point_mul_binary(k, Qx, Qy, x_2, y_2);

//    jacobian coordinates:
//    jacobian_affine_point_mul(k, Gx, Gy, x_1, y_1);
//    jacobian_affine_point_mul(k, Qx, Qy, x_2, y_2);

//    naf:
    naf_point_mul(k, Gx, Gy, x_1, y_1);
    naf_point_mul(k, Qx, Qy, x_2, y_2);

    mod_mul(m, x_2, n, c);

}

void ELGamal_decryption(LONG_NUM x_1, LONG_NUM y_1, LONG_NUM private_key_d, LONG_NUM c, LONG_NUM m) {

    LONG_NUM x_2, y_2, x2_inverse;
    memset(x_2, 0x00, sizeof (x_2));
    memset(y_2, 0x00, sizeof (y_2));
    memset(x2_inverse, 0x00, sizeof (x2_inverse));

//    affine coordinates:
//    point_mul_binary(private_key_d, x_1, y_1, x_2, y_2);

//    jacobian coordinates:
//    jacobian_affine_point_mul(private_key_d, x_1, y_1, x_2, y_2);

//    naf:
    naf_point_mul(private_key_d, x_1, y_1, x_2, y_2);

    inverse(x_2, n, x2_inverse);
    mod_mul(c, x2_inverse, n, m);

}