#include <stdio.h>

#include "sll.h"
#include "structs.h"

/** @return The length of the vector */
size_t
cdsa_sll_size (cdsa_sll_t vec)
{
    return vec->size;
}

/** @return A pointer to the first value */
__sll_data_t
cdsa_sll_front (cdsa_sll_t vec)
{
    return vec->front->data;
}

/** @return A pointer to the last value */
__sll_data_t
cdsa_sll_back (cdsa_sll_t vec)
{
    struct __sll_elem_t *iter = vec->front;

    for (size_t i = 0; i < vec->size - 1; ++i)
        iter = iter->next;

    return iter->data;
}

/** @return If the vector is empty */
__sll_data_t
cdsa_sll_empty (cdsa_sll_t vec)
{
    return vec->front == NULL;
}
