#include <stdint.h>

#include "misc.h"

int64_t
cdsa_math_gcd (int64_t a, int64_t b)
{
    if (b == 0)
        return a;

    return cdsa_math_gcd (b, a % b);
}

int64_t
cdsa_math_lcm (int64_t a, int64_t b)
{
    return a / cdsa_math_gcd (a, b) * b;
}
