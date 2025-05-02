#include <stdint.h>

#include "misc.h"
#include "rational.h"

cdsa_rat_t
cdsa_rat_new (int64_t n, int64_t d)
{
    return (cdsa_rat_t){ n, d };
}

cdsa_rat_t
cdsa_rat_new_z (int64_t x)
{
    return (cdsa_rat_t){ x, 1 };
}

static void
__simplify (cdsa_rat_t *x)
{
    int64_t gcd = cdsa_math_gcd (x->n, x->d);
    x->n /= gcd;
    x->d /= gcd;
}

cdsa_rat_t
cdsa_rat_add (cdsa_rat_t a, cdsa_rat_t b)
{
    cdsa_rat_t new = cdsa_rat_new (a.n * b.d + b.n * a.d, a.d * b.d);
    __simplify (&new);
    return new;
}

cdsa_rat_t
cdsa_rat_add_z (cdsa_rat_t a, int64_t c)
{
    a.n += c * a.d;
    __simplify (&a);
    return a;
}

cdsa_rat_t
cdsa_rat_sub (cdsa_rat_t a, cdsa_rat_t b)
{
    a.n *= -1;
    return cdsa_rat_add (a, b);
}

cdsa_rat_t
cdsa_rat_sub_z (cdsa_rat_t a, int64_t c)
{
    a.n -= c * a.d;
    __simplify (&a);
    return a;
}

cdsa_rat_t
cdsa_rat_mul (cdsa_rat_t a, cdsa_rat_t b)
{
    int64_t gcd = cdsa_math_gcd (a.n, b.d);
    a.n /= gcd;
    b.d /= gcd;
    gcd = cdsa_math_gcd (b.n, a.d);
    b.n /= gcd;
    a.d /= gcd;
    return cdsa_rat_new (a.n * b.n, a.d * b.d);
}

cdsa_rat_t
cdsa_rat_mul_z (cdsa_rat_t a, int64_t c)
{
    a.n *= c;
    __simplify (&a);
    return a;
}

cdsa_rat_t
cdsa_rat_div (cdsa_rat_t a, cdsa_rat_t b)
{
    int64_t tmp = a.n;
    a.n = a.d;
    a.d = tmp;
    return cdsa_rat_mul (a, b);
}

cdsa_rat_t
cdsa_rat_div_z (cdsa_rat_t a, int64_t c)
{
    a.d *= c;
    __simplify (&a);
    return a;
}

int
cdsa_rat_compar (const cdsa_rat_t lhs, const cdsa_rat_t rhs)
{
    int64_t l = lhs.n * rhs.d;
    int64_t r = rhs.n * lhs.d;
    return (l > r) - (l < r);
}

int
cdsa_rat_pcompar (const void *lhs, const void *rhs)
{
    return cdsa_rat_compar (*(cdsa_rat_t *)lhs, *(cdsa_rat_t *)rhs);
}

long double
cdas_rat_val (cdsa_rat_t x)
{
    return x.n / (long double)x.d;
}
