#pragma once

#ifndef CDSA_MATH_RATIONAL_H
#define CDSA_MATH_RATIONAL_H

#include <stdint.h>

typedef struct __cdsa_rat_struct {
    int64_t n;
    int64_t d;
} cdsa_rat_t;

extern cdsa_rat_t cdsa_rat_new (int64_t n, int64_t d);

extern cdsa_rat_t cdsa_rat_new_z (int64_t x);

extern cdsa_rat_t cdsa_rat_add (cdsa_rat_t a, cdsa_rat_t b);

extern cdsa_rat_t cdsa_rat_add_z (cdsa_rat_t a, int64_t c);

extern cdsa_rat_t cdsa_rat_sub (cdsa_rat_t a, cdsa_rat_t b);

extern cdsa_rat_t cdsa_rat_sub_z (cdsa_rat_t a, int64_t c);

extern cdsa_rat_t cdsa_rat_mul (cdsa_rat_t a, cdsa_rat_t b);

extern cdsa_rat_t cdsa_rat_mul_z (cdsa_rat_t a, int64_t b);

extern cdsa_rat_t cdsa_rat_div (cdsa_rat_t a, cdsa_rat_t b);

extern cdsa_rat_t cdsa_rat_div_z (cdsa_rat_t a, int64_t b);

extern int cdsa_rat_compar (const cdsa_rat_t lhs, const cdsa_rat_t rhs);

extern int cdsa_rat_pcompar (const void *lhs, const void *rhs);

extern long double cdas_rat_val (cdsa_rat_t x);

#endif
