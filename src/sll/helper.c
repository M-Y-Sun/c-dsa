#include <stdio.h>

#include "structs.h"

/**
 * Helper function; DEVONLY
 *
 * @return An element iterator to the requested beginning of the operation.
 * @param vec The vector to operate on.
 * @param beg The index to begin at
 * */
struct __cdsa_sll_elem_t *
__cdsa_sll_iter_begin (cdsa_sll_t vec, size_t beg)
{
    if (beg >= vec->size) {
        perror ("[ \033[1;31mFAILED\033[0m ] iter_begin: requested position "
                "out of bounds\n");
        return NULL;
    }

    struct __cdsa_sll_elem_t *iter = vec->front;

    for (size_t i = 0; i < beg; ++i)
        iter = iter->next;

    return iter;
}
