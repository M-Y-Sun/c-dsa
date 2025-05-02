#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "poly.h"

void
cdsa_poly_init (cdsa_poly_t this, const long double *restrict coef,
                uint_fast8_t deg)
{
    this->deg = deg;
    this->coef = malloc (deg * sizeof (long double));

    for (uint_fast8_t i = 0; i <= deg; ++i)
        this->coef[i] = coef[i];
}

void
cdsa_poly_deinit (cdsa_poly_t this)
{
    free (this->coef);
}

long double
cdsa_poly_eval (const cdsa_poly_t this, const long double x)
{
    long double ans = 0;

    for (uint_fast8_t i = 0; i <= this->deg; ++i)
        ans += this->coef[i] * pow (x, i);

    return ans;
}

void
cdsa_poly_ddx_n (cdsa_poly_t this, uint_fast8_t n)
{
    if (n == 0)
        return;

    for (uint_fast8_t i = 1; i <= this->deg; ++i) {
        this->coef[i - 1] = i * this->coef[i];
    }

    --this->deg;

    cdsa_poly_ddx_n (this, n - 1);
}

void
cdsa_poly_print (const cdsa_poly_t this)
{
    printf ("%Lf", this->coef[0]);

    for (uint_fast8_t i = 1; i <= this->deg; ++i)
        printf (" + %Lfx^%hhu", this->coef[i], i);

    putchar ('\n');
}
