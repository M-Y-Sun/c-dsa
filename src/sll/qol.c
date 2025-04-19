#include <limits.h>
#include <stdio.h>

#include "devhelper.h"
#include "sll.h"
#include "structs.h"

/**
 * Checks if the specified beginning and ending index is within the bounds of
 * the vector.
 * @param beg The beginning index
 * @param end The ending index
 */
static inline int
__in_bound (cdsa_sll_t vec, size_t beg, size_t end)
{
    return beg < 0 || end < 0 || beg > vec->size || end > vec->size
           || beg > end;
}

/**
 * Prints the vector in a certain range specified by [beg, end)
 *
 * Print format: '[ 0 , 1 , 2 , 3 ]'
 *
 * @param beg The beginning index
 * @param end The ending index
 * */
void
cdsa_sll_print (cdsa_sll_t vec, size_t beg, size_t end)
{
    if (__in_bound (vec, beg, end)) {
        perror ("[ \033[1;31mFAILED\033[0m ] print: requested position out "
                "of bounds");
        return;
    }

    struct __sll_elem_t *iter = __cdsa_sll_iter_begin (vec, beg);

    printf ("[ ");
    for (size_t i = beg; i < end; ++i) {
        if (i != end - 1)
            printf ("%d , ", iter->data);
        else
            printf ("%d", iter->data);

        iter = iter->next;
    }

    printf (" ]\n");
}

/**
 * @return The sum of the elements of a vector in a certain range [beg, end).
 * @param beg The beginning index
 * @param end The ending index
 * */
int64_t
cdsa_sll_sum (cdsa_sll_t vec, size_t beg, size_t end)
{
    if (__in_bound (vec, beg, end)) {
        perror ("[ \033[1;31mFAILED\033[0m ] sum: requested position out of "
                "bounds");
        return 0;
    }

    struct __sll_elem_t *iter = __cdsa_sll_iter_begin (vec, beg);
    int64_t sum = 0;

    for (size_t i = beg; i < end; ++i) {
        sum += iter->data;
        iter = iter->next;
    }

    return sum;
}

/**
 * @return The product of the elements of a vector in a certain range
 * @param beg The beginning index
 * @param end The ending index
 * */
int64_t
cdsa_sll_prod (cdsa_sll_t vec, size_t beg, size_t end)
{
    if (__in_bound (vec, beg, end)) {
        perror ("[ \033[1;31mFAILED\033[0m ] prod: requested position out "
                "of bounds");
        return 0;
    }

    struct __sll_elem_t *iter = __cdsa_sll_iter_begin (vec, beg);
    int64_t prod = 1;

    for (size_t i = beg; i < end; ++i) {
        if (iter->data == 0)
            return 0;

        prod = prod * iter->data;
        iter = iter->next;
    }

    return prod;
}
