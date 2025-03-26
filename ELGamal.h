//
// Created by 24208 on 2023/10/26.
//

#include "longprime_generate.h"
#include "ellipticcurve_calculation.h"
#include <windows.h>

#ifndef ECC_ELGAMAL_H
#define ECC_ELGAMAL_H

void ELGamal();

void ELGamal_encryption(LONG_NUM Qx, LONG_NUM Qy, LONG_NUM x_1, LONG_NUM y_1, LONG_NUM f, LONG_NUM c);
void ELGamal_decryption(LONG_NUM x_1, LONG_NUM y_1, LONG_NUM private_key_d, LONG_NUM c, LONG_NUM m);


#endif //ECC_ELGAMAL_H
