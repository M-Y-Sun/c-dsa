#include <stdint.h>
#include <stdio.h>

#include "algs.h"
#include "poly.h"

static long double
__newton_estim_n (const cdsa_poly_t p, const cdsa_poly_t dpdx, long double r0,
                  uint_fast8_t n)
{
    if (n == 0)
        return r0;

    long double r = r0 - cdsa_poly_eval (p, r0) / cdsa_poly_eval (dpdx, r0);

    return __newton_estim_n (p, dpdx, r, n - 1);
}

/**
 * linear approximation of $f(x)$ at $r_0$ is given by
 *
 * \[ y = f'(r0)(x - r0) + f(r0) \]
 *
 * so the x-intercept will become
 *
 * \[ x = r0 - f(r0) / f'(r0) \]
 *
 * so we iterate with x = r1
 */
long double
cdsa_algs_newton_estim_n (cdsa_poly_t p, long double r0, uint_fast8_t n)
{
    cdsa_poly_t dpdx;
    cdsa_poly_init (dpdx, p->coef, p->deg);
    cdsa_poly_ddx_n (dpdx, 1);

    long double ans = __newton_estim_n (p, dpdx, r0, n);

    cdsa_poly_deinit (dpdx);

    return ans;
}
