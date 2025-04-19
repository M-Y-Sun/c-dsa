#include <stdio.h>

#include "devhelper.h"
#include "sll.h"
#include "structs.h"

/** Deletes all elements in the vector. */
void
cdsa_sll_clear (cdsa_sll_t vec)
{
    // cleanup function
    if (vec->size < 2) {
        free (vec->front);
        return;
    }
    struct __sll_elem_t *ptr1 = vec->front;
    struct __sll_elem_t *ptr2 = vec->front;
    for (size_t i = 0; i < vec->size; ++i) {
        ptr1 = ptr2;
        ptr2 = ptr1->next;
        free (ptr1);
    }
    if (ptr2 == NULL)
        free (ptr2);

    vec->front = NULL;
}

/**
 * Fills an empty vector to a specified size and initializes all values to a
 * specified value
 * @param size The size to assign to
 * @param data The data to assign each element to
 * */
void
cdsa_sll_assign (cdsa_sll_t vec, size_t size, int data)
{
    // if size is negative or too large that it overflows the size limit
    if (size < 0 || size > SIZE_MAX) {
        perror ("[ \033[1;31mFAILED\033[0m ] assign: requested size out of "
                "bounds\n");
        return;
    }

    // if the vector already contains elements, dont do anything
    if (vec->front != NULL || vec->size != 0)
        return;

    vec->size = size;
    struct __sll_elem_t *new
        = (struct __sll_elem_t *)malloc (sizeof (struct __sll_elem_t));
    if (new == NULL) {
        perror ("[ \033[1;31mFAILED\033[0m ] malloc: memory request service "
                "failed\n");
        return;
    }
    vec->front = new;
    struct __sll_elem_t *iter = vec->front;
    for (size_t i = 1; i < size; ++i) {
        struct __sll_elem_t *new_next
            = (struct __sll_elem_t *)malloc (sizeof (struct __sll_elem_t));
        if (new_next == NULL) {
            perror ("[ \033[1;31mFAILED\033[0m ] malloc: memory request "
                    "service failed\n");
            return;
        }

        new_next->data = data;
        iter->next = new_next;
        iter = new_next;
    }
}

/**
 * Resizes the vector and initiates all values to a specific value
 * @param size The size to resize to
 * @param data The data to assign each new value to
 * */
void
cdsa_sll_resize (cdsa_sll_t vec, size_t size, int data)
{
    // if size is negative or too large that it overflows the size_t limit
    if (size < 0 || size > SIZE_MAX) {
        perror ("[ \033[1;31mFAILED\033[0m ] resize: requested size out of "
                "bounds\n");
        return;
    }

    // if the requested size = current size, do nothing
    if (vec->size == size)
        return;

    // if the vector wants to be shrunk,
    // pop the elements overflowing the size
    if (size < vec->size) {
        while (vec->size > size)
            cdsa_sll_popb (vec);

    } else { // if the vector wants to be enlarged, add initialized elements to
             // end
        while (vec->size < size)
            cdsa_sll_pushb (vec, data);
    }
}

/**
 * Adds an element to the end
 * @param data The data to append
 * */
void
cdsa_sll_pushb (cdsa_sll_t vec, int data)
{
    struct __sll_elem_t *new
        = (struct __sll_elem_t *)malloc (sizeof (struct __sll_elem_t));
    if (new == NULL) {
        perror ("[ \033[1;31mFAILED\033[0m ] malloc: memory request service "
                "failed\n");
        return;
    }

    if (vec->front == NULL) {
        new->data = data;
        vec->front = new;
        new->next = NULL;
    } else {
        // find the position
        struct __sll_elem_t *cur = vec->front;
        struct __sll_elem_t *prev = vec->front;
        for (size_t i = 0; i < vec->size; ++i) {
            prev = cur;
            cur = cur->next;
        }

        // set the values
        cur = new;
        cur->data = data;
        prev->next = cur;
    }

    ++(vec->size);
}

/**
 * @param pos The position to get
 * @return A pointer to the value at a certain location.
 * */
int *
cdsa_sll_get (cdsa_sll_t vec, size_t pos)
{
    if (pos < 0 || pos >= vec->size) {
        perror ("[ \033[1;31mFAILED\033[0m ] get: requested size of out "
                "bounds\n");
        return 0;
    }

    struct __sll_elem_t *iter = vec->front;
    for (size_t i = 0; i < pos; ++i) {
        iter = iter->next;
    }

    return &(iter->data);
}

/**
 * Inserts an element in a specified position
 * @param pos The position to insert at
 * @param data The data to insert
 * */
struct __sll_elem_t *
cdsa_sll_insert (cdsa_sll_t vec, size_t pos, int data)
{
    if (pos < 0
        || pos >= vec->size) { // if position is negative or larger than the
        // vector size handle the out of bounds error
        perror ("[ \033[1;31mFAILED\033[0m ] insert: requested size out of "
                "bounds\n");
        return NULL;
    }

    // get to insertion position
    struct __sll_elem_t *iter = vec->front;
    for (size_t i = 0; i < pos - 1; ++i) {
        iter = iter->next;
    }

    // set the values and link
    struct __sll_elem_t *new
        = (struct __sll_elem_t *)malloc (sizeof (struct __sll_elem_t));
    if (new == NULL) {
        perror ("[ \033[1;31mFAILED\033[0m ] malloc: memory request service "
                "failed\n");
        return NULL;
    }

    new->data = data;
    new->next = iter->next;
    iter->next = new;

    return new;
}

/**
 * Swaps the value of two elements in a specified position
 * @param i1 The index of the first element
 * @param i2 The index of the second element
 * */
void
cdsa_sll_swap (cdsa_sll_t vec, size_t i1, size_t i2)
{
    if (i1 < 0 || i2 < 0 || i1 >= vec->size
        || i2 >= vec->size) { // if any requested index is negative or greater
        // than the size, handle the out of bounds error
        perror ("[ \033[1;31mFAILED\033[0m ] swap: requested size out of "
                "bounds\n");
        return;
    }

    // make i1 always smaller than i2
    if (i1 > i2) {
        int tmp = i1;
        i1 = i2;
        i2 = tmp;
    }

    // read the data

    // get to the first index and read value
    struct __sll_elem_t *iter = __cdsa_sll_iter_begin (vec, i1);

    int data1 = iter->data;

    for (size_t i = i1; i < i2; ++i)
        iter = iter->next;

    int data2 = iter->data;

    // set the data

    // get to the first index and change to the swapped value
    iter = __cdsa_sll_iter_begin (vec, i1);

    iter->data = data2;

    for (size_t i = i1; i < i2; ++i)
        iter = iter->next;

    iter->data = data1;
}

/**
 * Deletes an element in a specified position
 * @param pos The position to erase
 * */
void
cdsa_sll_erase (cdsa_sll_t vec, size_t pos)
{
    // if position is negative or larger than the vector size,
    // handle the out of bounds
    if (pos < 0 || pos >= vec->size) {
        perror ("[ \033[1;31mFAILED\033[0m ] erase: requested size out of "
                "bounds\n");
        return;
    }

    // points to the address of the element to be removed
    struct __sll_elem_t **indirect = &vec->front;

    for (size_t i = 0; i < pos; ++i)
        indirect = &(*indirect)->next;

    // simply remove the element and free memory
    struct __sll_elem_t *free_ptr = *indirect;
    *indirect = (*indirect)->next;
    free (free_ptr);
}

/**
 * Removes the last element.
 * @return The data stored by the popped value.
 * */
int
cdsa_sll_popb (cdsa_sll_t vec)
{
    if (vec->size == 0) {
        perror ("[ \033[1;31mFAILED\033[0m ] popb: cannot remove elements "
                "in an empty vector");
        return 0;
    }

    // get iterator to the last element
    struct __sll_elem_t *p_rm = __cdsa_sll_iter_begin (vec, vec->size - 1);
    int val = p_rm->data;

    free (p_rm);
    --(vec->size);

    return val;
}

/**
 * Removes the first element.
 * @return The data stored by the popped value.
 * */
int
cdsa_sll_popf (cdsa_sll_t vec)
{
    if (vec->size == 0) {
        perror ("[ \033[1;31mFAILED\033[0m ] popf: cannot remove elements "
                "in an empty vector");
        return 0;
    }

    // set the front equal to the next element
    struct __sll_elem_t *p_rm = vec->front;
    vec->front = vec->front->next;
    int val = p_rm->data;

    free (p_rm);
    --(vec->size);

    return val;
}
