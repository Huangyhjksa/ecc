#pragma once
//
// Created by 24208 on 2023/10/23.
//

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef RSA2_LONGNUM_CALCULATION_H
#define RSA2_LONGNUM_CALCULATION_H

#define VALID 0
#define ADD_OVERFLOW 1
#define SUB_OVERFLOW 2
#define MUL_OVERFLOW 3
#define DIV_ZERO 4
#define NO_INVERSE 5
#define ERROR 6

#define NUMBER_LENGTH 33
#define WORD_BITS 32
#define STR_SIZE 32
#define SMALL_PRIMES 2560
#define INTERVAL_L 1024
#define QUATERNARY 4


typedef uint32_t LONG_NUM[NUMBER_LENGTH + 1];           // length of long_num is 1024+32 bits
typedef uint32_t DOUBLE_LONG_NUM[NUMBER_LENGTH * 2 + 1];// length of double_long_num is 2048+32 bits
#define UNIT_SIZE sizeof(LONG_NUM)                      // bits the long_num occupied

int mod_add(LONG_NUM a, LONG_NUM b, LONG_NUM n, LONG_NUM result);
void mod_sub(LONG_NUM x, LONG_NUM y, LONG_NUM n,LONG_NUM diff);
int mod_n(LONG_NUM a, LONG_NUM n, LONG_NUM remainder);
void mod_mul(LONG_NUM a, LONG_NUM b, LONG_NUM n, LONG_NUM remainder);


int gcd(LONG_NUM a, LONG_NUM b, LONG_NUM factor);
int inverse(LONG_NUM a, LONG_NUM n, LONG_NUM inverse_element);
void mod_exp(LONG_NUM M, LONG_NUM e, LONG_NUM n, LONG_NUM rem);
void mod_exp_binary(LONG_NUM M, LONG_NUM e, LONG_NUM n, LONG_NUM rem);
void mod_exp_quaternary(LONG_NUM M, LONG_NUM e, LONG_NUM n, LONG_NUM rem);


int add(LONG_NUM a, LONG_NUM b, LONG_NUM sum);
int sub(LONG_NUM a, LONG_NUM b, LONG_NUM diff);
int mul(LONG_NUM a, LONG_NUM b, DOUBLE_LONG_NUM prod);
int division(LONG_NUM a, LONG_NUM b, DOUBLE_LONG_NUM q, LONG_NUM remainder);
int division_v2(LONG_NUM a, LONG_NUM b, DOUBLE_LONG_NUM q, LONG_NUM remainder);


void set_max(LONG_NUM a);
int shift_left(DOUBLE_LONG_NUM a);
int shift_right(LONG_NUM a);


void number_copy(LONG_NUM destination, LONG_NUM source);
int number_compare(LONG_NUM a, LONG_NUM b);
int get_bits(const LONG_NUM a);
void display_number(LONG_NUM a);


#endif //RSA2_LONGNUM_CALCULATION_H