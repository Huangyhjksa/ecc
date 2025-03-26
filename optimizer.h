//
// Created by 24208 on 2023/11/2.
//

#include "basic_calculation.h"

#ifndef ECC_OPTIMIZER_H
#define ECC_OPTIMIZER_H

void crt(LONG_NUM a, LONG_NUM b, LONG_NUM p, LONG_NUM q, LONG_NUM rem);
void montgomery_exp(LONG_NUM M, LONG_NUM e, LONG_NUM n, LONG_NUM x);

void mon_pro(LONG_NUM hat_a, LONG_NUM hat_b, LONG_NUM n, LONG_NUM prime_n, LONG_NUM r, LONG_NUM result);
void mon_pro_sos(LONG_NUM hat_a, LONG_NUM hat_b, LONG_NUM n, const LONG_NUM prime_n, LONG_NUM r, LONG_NUM result);
//void mon_pro_cios(LONG_NUM hat_a, LONG_NUM hat_b, LONG_NUM n, LONG_NUM prime_n, LONG_NUM r, LONG_NUM result);


#endif //ECC_OPTIMIZER_H
