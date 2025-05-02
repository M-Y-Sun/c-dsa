#pragma once

#ifndef CDSA_MATH_ALGS_H
#define CDSA_MATH_ALGS_H

#include <stdint.h>

#include "poly.h"

extern long double cdsa_algs_newton_estim_n (cdsa_poly_t p, long double r,
                                        uint_fast8_t n);

#endif
