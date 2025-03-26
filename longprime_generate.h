//
// Created by 24208 on 2023/10/23.
//

#include "basic_calculation.h"

#ifndef RSA2_LONGPRIME_GENERATE_H
#define RSA2_LONGPRIME_GENERATE_H


void prime_generate(LONG_NUM prime);

void init_random_seed();
void random_generate(uint8_t* str, int len);

void num_to_str(const LONG_NUM long_num, uint8_t* str);
void str_to_num(LONG_NUM long_num, const uint8_t* str, int length);

int fermat_test(LONG_NUM a);
int miller_rabin_test(LONG_NUM n);




#endif //RSA2_LONGPRIME_GENERATE_H
