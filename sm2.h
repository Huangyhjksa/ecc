//
// Created by 24208 on 2023/11/10.
//

#include "longprime_generate.h"
#include "ellipticcurve_calculation.h"
#include "sm3.h"
#include <windows.h>

#ifndef ECC_SM2_H
#define ECC_SM2_H


void sm2_cryption();

void KDF(const uint8_t* Z, int klen, uint8_t* k);
void sm2_encryption(unsigned char* m, LONG_NUM Px, LONG_NUM Py, LONG_NUM C1x, LONG_NUM C1y, LONG_NUM C2, LONG_NUM C3);
void sm2_decryption(LONG_NUM C1x, LONG_NUM C1y, const LONG_NUM C2, LONG_NUM C3, LONG_NUM d);

void sm2_digital_signature();
void sm2_signature(unsigned char* m, LONG_NUM ZA, LONG_NUM d, LONG_NUM r, LONG_NUM s);
void sm2_verification(unsigned char* m, LONG_NUM Px, LONG_NUM Py, LONG_NUM ZA, LONG_NUM r, LONG_NUM s);

#endif //ECC_SM2_H
