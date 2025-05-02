#pragma once

#ifndef CDSA_SLL_SLL_H
#define CDSA_SLL_SLL_H

#include "structs.h"

#include <stdint.h>

/****** MUST INCLUDE ******/

/** Initializes a `cdsa_sll_t ` instance. */
void cdsa_sll_init (cdsa_sll_t vec);

/** Cleans up a `cdsa_sll_t ` instance to free allocated memory. */
void cdsa_sll_deinit (cdsa_sll_t vec);

/****** READ ONLY FUNCTIONS ******/

/** @return The length of the vector */
size_t cdsa_sll_size (cdsa_sll_t vec);

/** @return A pointer to the first value */
__cdsa_sll_data_t cdsa_sll_front (cdsa_sll_t vec);

/** @return A pointer to the last value */
__cdsa_sll_data_t cdsa_sll_back (cdsa_sll_t vec);

/** @return If the vector is empty */
__cdsa_sll_data_t cdsa_sll_empty (cdsa_sll_t vec);

/****** MODIFIER FUNCTIONS ******/

/** Deletes all elements in the vector. */
void cdsa_sll_clear (cdsa_sll_t vec);

/**
 * Fills an empty vector to a specified size and initializes all values to a
 * specified value
 * @param size The size to assign to
 * @param data The data to assign each element to
 * */
void cdsa_sll_assign (cdsa_sll_t vec, size_t size, __cdsa_sll_data_t data);

/**
 * Resizes the vector and initiates all values to a specific value
 * @param size The size to resize to
 * @param data The data to assign each new value to
 * */
void cdsa_sll_resize (cdsa_sll_t vec, size_t size, __cdsa_sll_data_t data);

/**
 * Adds an element to the end
 * @param data The data to append
 * */
void cdsa_sll_pushb (cdsa_sll_t vec, __cdsa_sll_data_t data);

/**
 * @param pos The position to get
 * @return A pointer to the value at a certain location.
 * */
__cdsa_sll_data_t *cdsa_sll_get (cdsa_sll_t vec, size_t pos);

/**
 * Inserts an element in a specified position
 * @param pos The position to insert at
 * @param data The data to insert
 * */
struct __cdsa_sll_elem_t *cdsa_sll_insert (cdsa_sll_t vec, size_t pos,
                                           __cdsa_sll_data_t data);

/**
 * Swaps the value of two elements in a specified position
 * @param i1 The index of the first element
 * @param i2 The index of the second element
 * */
void cdsa_sll_swap (cdsa_sll_t vec, size_t i1, size_t i2);

/**
 * Deletes an element in a specified position
 * @param pos The position to erase
 * */
void cdsa_sll_erase (cdsa_sll_t vec, size_t pos);

/**
 * Removes the last element.
 * @return The data stored by the popped value.
 * */
__cdsa_sll_data_t cdsa_sll_popb (cdsa_sll_t vec);

/**
 * Removes the first element.
 * @return The data stored by the popped value.
 * */
__cdsa_sll_data_t cdsa_sll_popf (cdsa_sll_t vec);

void cdsa_sll_sort (cdsa_sll_t this, int (*cmp) (const void *, const void *));

void cdsa_sll_sort_inplace_lt (cdsa_sll_t this);

/****** QUALITY OF LIFE FUNCTIONS (READ ONLY) ******/

/**
 * Prints the vector in a certain range specified by [beg, end)
 *
 * Print format: '[ 0 , 1 , 2 , 3 ]'
 *
 * @param beg The beginning index
 * @param end The ending index
 * */
void cdsa_sll_print (cdsa_sll_t vec, size_t beg, size_t end);

void cdsa_sll_print_all (cdsa_sll_t vec);

/**
 * @return The sum of the elements of a vector in a certain range [beg, end).
 * @param beg The beginning index
 * @param end The ending index
 * */
int64_t cdsa_sll_sum (cdsa_sll_t vec, size_t beg, size_t end);

/**
 * @return The product of the elements of a vector in a certain range
 * @param beg The beginning index
 * @param end The ending index
 * */
int64_t cdsa_sll_prod (cdsa_sll_t vec, size_t beg, size_t end);

#endif
