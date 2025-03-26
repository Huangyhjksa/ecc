cmake_minimum_required(VERSION 3.26)
project(ECC C)

set(CMAKE_C_STANDARD 99)

add_executable(ECC main.c
        basic_calculation.c
        basic_calculation.h
        ELGamal.c
        ELGamal.h
        ellipticcurve_calculation.c
        ellipticcurve_calculation.h
        longprime_generate.c
        longprime_generate.h
        optimizer.c
        optimizer.h
        sm2.c
        sm2.h
        sm3.c
        sm3.h
)
