#pragma once

#ifndef CDSA_MATH_MATH_H
#define CDSA_MATH_MATH_H

#include <stdint.h>

typedef struct __cdsa_poly_struct {
    uint_fast8_t deg;
    long double *coef;
} cdsa_poly_t[1];

extern void cdsa_poly_init (cdsa_poly_t this, const long double *restrict coef,
                            uint_fast8_t deg);

extern void cdsa_poly_deinit (cdsa_poly_t this);

extern long double cdsa_poly_eval (const cdsa_poly_t this, long double x);

extern void cdsa_poly_ddx_n (cdsa_poly_t this, uint_fast8_t n);

extern void cdsa_poly_print (const cdsa_poly_t this);

#endif
