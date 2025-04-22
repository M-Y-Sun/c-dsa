#pragma once

#ifndef SLL_STRUCTS_H
#define SLL_STRUCTS_H

#include <stdint.h>
#include <stdlib.h>

typedef int64_t __sll_data_t;

/** A single element in a vector */
struct __sll_elem_t {
    __sll_data_t data;
    struct __sll_elem_t *next;
};

/** The vector type representing the beginning of the data structure. */
typedef struct __sll_struct {
    size_t size;
    struct __sll_elem_t *front;
} cdsa_sll_t[1];

typedef struct __sll_struct *restrict cdsa_sll_restrict_ptr_t;

#endif
