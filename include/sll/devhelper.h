#pragma once

#ifndef SLL_DEVHELPER_H
#define SLL_DEVHELPER_H

#include <stdlib.h>
#include "./structs.h"

/**
 * Helper function; DEVONLY
 *
 * @return An element iterator to the requested beginning of the operation.
 * @param vec The vector to operate on.
 * @param beg The index to begin at
 */
struct __cdsa_sll_elem_t *__cdsa_sll_iter_begin (cdsa_sll_t vec, size_t beg);

#endif
