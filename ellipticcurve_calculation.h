//
// Created by 24208 on 2023/10/26.
//

#include "basic_calculation.h"


#ifndef ECC_ELLIPTIC_CURVE_CALCULATION_H
#define ECC_ELLIPTIC_CURVE_CALCULATION_H


void affine_coordinates_addition(LONG_NUM Px, LONG_NUM Py, LONG_NUM Qx, LONG_NUM Qy, LONG_NUM Rx, LONG_NUM Ry);
void affine_coordinates_doubling(LONG_NUM Px, LONG_NUM Py, LONG_NUM Rx, LONG_NUM Ry);
void point_mul_binary(LONG_NUM k, LONG_NUM Px, LONG_NUM Py, LONG_NUM Rx, LONG_NUM Ry);

void jacobian_coordinates_doubling(LONG_NUM Px, LONG_NUM Py, LONG_NUM Pz, LONG_NUM Rx, LONG_NUM Ry, LONG_NUM Rz);
void mixed_jacobian_affine(LONG_NUM Px, LONG_NUM Py, LONG_NUM Pz, LONG_NUM Qx, LONG_NUM Qy, LONG_NUM Rx, LONG_NUM Ry, LONG_NUM Rz);
void jacobian_affine_point_mul(LONG_NUM k, LONG_NUM Px, LONG_NUM Py, LONG_NUM Rx, LONG_NUM Ry);

void naf_trans(LONG_NUM k, uint32_t w, int *k_w_width, int *length);
void naf_point_mul(LONG_NUM k, LONG_NUM Px, LONG_NUM Py, LONG_NUM Rx, LONG_NUM Ry);

#endif //ECC_ELLIPTIC_CURVE_CALCULATION_H
