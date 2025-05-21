/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * FILE NAME:                                                                *
 * mand.c                                                                    *
 *                                                                           *
 * PURPOSE:                                                                  *
 * Includes mandatory functions that must be run                             *
 *                                                                           *
 * EXTERNAL REFERENCES:                                                      *
 * 'size_t' type              (from <stdlib.h>)                              *
 * 'free' function            (from <stdlib.h>)                              *
 * 'cdsa_sll_t ' struct             (from 'structs.h') * '__cdsa_sll_elem_t'
 * struct (from 'structs.h')                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "sll.h"
#include "structs.h"

// mandatory functions

/** Initializes a `cdsa_sll_t ` instance. */
void
cdsa_sll_init (cdsa_sll_t vec)
{
    vec->size = 0;
    vec->front = NULL;
}

/** Cleans up a `cdsa_sll_t ` instance to free allocated memory. */
void
cdsa_sll_deinit (cdsa_sll_t vec)
{
    // if it is empty (contains only a NULL element) or has only 1 element
    if (vec->size < 2) {
        free (vec->front);
        return;
    }

    struct __cdsa_sll_elem_t *ptr1 = vec->front;
    struct __cdsa_sll_elem_t *ptr2 = vec->front;

    for (size_t i = 0; i < vec->size; ++i) {
        ptr1 = ptr2;
        ptr2 = ptr1->next;
        free (ptr1);
    }

    if (ptr2 == NULL)
        free (ptr2);
}
