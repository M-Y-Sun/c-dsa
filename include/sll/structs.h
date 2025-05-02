#pragma once

#ifndef CDSA_SLL_STRUCTS_H
#define CDSA_SLL_STRUCTS_H

#include <stdint.h>
#include <stdlib.h>

typedef int64_t __cdsa_sll_data_t;

/** A single element in a vector */
struct __cdsa_sll_elem_t {
    __cdsa_sll_data_t data;
    struct __cdsa_sll_elem_t *next;
};

/** The vector type representing the beginning of the data structure. */
typedef struct __cdsa_sll_struct {
    size_t size;
    struct __cdsa_sll_elem_t *front;
} cdsa_sll_t[1];

typedef struct __cdsa_sll_struct *restrict cdsa_sll_restrict_ptr_t;

#endif
