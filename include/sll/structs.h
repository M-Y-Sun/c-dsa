#pragma once

#ifndef SLL_STRUCTS_H
#define SLL_STRUCTS_H

#include <stdlib.h>

/** A single element in a vector */
struct __sll_elem_t {
    int data;
    struct __sll_elem_t *next;
};

/** The vector type representing the beginning of the data structure. */
typedef struct {
    size_t size;
    struct __sll_elem_t *front;
} cdsa_sll_t[1];

#endif
