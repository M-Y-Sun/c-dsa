#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_pthread/_pthread_types.h>

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

    struct __cdsa_sll_elem_t *ptr1 = vec->front;
    struct __cdsa_sll_elem_t *ptr2 = vec->front;

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
cdsa_sll_assign (cdsa_sll_t vec, size_t size, __cdsa_sll_data_t data)
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
    struct __cdsa_sll_elem_t *new
        = (struct __cdsa_sll_elem_t *)malloc (sizeof (struct __cdsa_sll_elem_t));
    if (new == NULL) {
        perror ("[ \033[1;31mFAILED\033[0m ] malloc: memory request service "
                "failed\n");
        return;
    }
    vec->front = new;
    struct __cdsa_sll_elem_t *iter = vec->front;
    for (size_t i = 1; i < size; ++i) {
        struct __cdsa_sll_elem_t *new_next
            = (struct __cdsa_sll_elem_t *)malloc (sizeof (struct __cdsa_sll_elem_t));
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
cdsa_sll_resize (cdsa_sll_t vec, size_t size, __cdsa_sll_data_t data)
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
cdsa_sll_pushb (cdsa_sll_t vec, __cdsa_sll_data_t data)
{
    struct __cdsa_sll_elem_t *new
        = (struct __cdsa_sll_elem_t *)malloc (sizeof (struct __cdsa_sll_elem_t));
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
        struct __cdsa_sll_elem_t *cur = vec->front;
        struct __cdsa_sll_elem_t *prev = vec->front;
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
__cdsa_sll_data_t *
cdsa_sll_get (cdsa_sll_t vec, size_t pos)
{
    if (pos < 0 || pos >= vec->size) {
        perror ("[ \033[1;31mFAILED\033[0m ] get: requested size of out "
                "bounds\n");
        return 0;
    }

    struct __cdsa_sll_elem_t *iter = vec->front;
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
struct __cdsa_sll_elem_t *
cdsa_sll_insert (cdsa_sll_t vec, size_t pos, __cdsa_sll_data_t data)
{
    if (pos < 0
        || pos >= vec->size) { // if position is negative or larger than the
        // vector size handle the out of bounds error
        perror ("[ \033[1;31mFAILED\033[0m ] insert: requested size out of "
                "bounds\n");
        return NULL;
    }

    // get to insertion position
    struct __cdsa_sll_elem_t *iter = vec->front;
    for (size_t i = 0; i < pos - 1; ++i) {
        iter = iter->next;
    }

    // set the values and link
    struct __cdsa_sll_elem_t *new
        = (struct __cdsa_sll_elem_t *)malloc (sizeof (struct __cdsa_sll_elem_t));
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
        __cdsa_sll_data_t tmp = i1;
        i1 = i2;
        i2 = tmp;
    }

    // read the data

    // get to the first index and read value
    struct __cdsa_sll_elem_t *iter = __cdsa_sll_iter_begin (vec, i1);

    __cdsa_sll_data_t data1 = iter->data;

    for (size_t i = i1; i < i2; ++i)
        iter = iter->next;

    __cdsa_sll_data_t data2 = iter->data;

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
    struct __cdsa_sll_elem_t **indirect = &vec->front;

    for (size_t i = 0; i < pos; ++i)
        indirect = &(*indirect)->next;

    // simply remove the element and free memory
    struct __cdsa_sll_elem_t *free_ptr = *indirect;
    *indirect = (*indirect)->next;
    free (free_ptr);
}

/**
 * Removes the last element.
 * @return The data stored by the popped value.
 * */
__cdsa_sll_data_t
cdsa_sll_popb (cdsa_sll_t vec)
{
    if (vec->size == 0) {
        perror ("[ \033[1;31mFAILED\033[0m ] popb: cannot remove elements "
                "in an empty vector");
        return 0;
    }

    // get iterator to the last element
    struct __cdsa_sll_elem_t *p_rm = __cdsa_sll_iter_begin (vec, vec->size - 1);
    __cdsa_sll_data_t val = p_rm->data;

    free (p_rm);
    --(vec->size);

    return val;
}

/**
 * Removes the first element.
 * @return The data stored by the popped value.
 * */
__cdsa_sll_data_t
cdsa_sll_popf (cdsa_sll_t vec)
{
    if (vec->size == 0) {
        perror ("[ \033[1;31mFAILED\033[0m ] popf: cannot remove elements "
                "in an empty vector");
        return 0;
    }

    // set the front equal to the next element
    struct __cdsa_sll_elem_t *p_rm = vec->front;
    vec->front = vec->front->next;
    __cdsa_sll_data_t val = p_rm->data;

    free (p_rm);
    --(vec->size);

    return val;
}

void
cdsa_sll_sort (cdsa_sll_t this, int (*compar) (const void *, const void *))
{
    __cdsa_sll_data_t *arr = malloc (this->size * sizeof (__cdsa_sll_data_t));

    struct __cdsa_sll_elem_t *node = this->front;

    for (size_t i = 0; i < this->size; ++i) {
        arr[i] = node->data;
        node = node->next;
    }

    qsort (arr, this->size, sizeof (__cdsa_sll_data_t), compar);

    node = this->front;

    for (size_t i = 0; i < this->size; ++i) {
        node->data = arr[i];
        node = node->next;
    }

    free (arr);
}

static void
__swap (__cdsa_sll_data_t *a, __cdsa_sll_data_t *b)
{
    __cdsa_sll_data_t tmp = *a;
    *a = *b;
    *b = tmp;
}

#define pred(x) ((x) < pivot || (le && (x) == pivot))

static struct __cdsa_sll_elem_t *
__partition (struct __cdsa_sll_elem_t *head, struct __cdsa_sll_elem_t *tail,
             __cdsa_sll_data_t pivot, bool le)
{
    struct __cdsa_sll_elem_t *mid = head;

    for (; mid != tail && pred (mid->data); mid = mid->next)
        ;

    if (mid == tail)
        return mid;

    for (struct __cdsa_sll_elem_t *r = mid->next; r != tail; r = r->next) {
        if (pred (r->data)) {
            __swap (&r->data, &mid->data);
            mid = mid->next;
        }
    }

    return mid;
}

#undef pred

static void
__cdsa_sll_quicksort (struct __cdsa_sll_elem_t *head, struct __cdsa_sll_elem_t *tail)
{
    if (head == tail)
        return;

    __cdsa_sll_data_t pivot = head->data;
    struct __cdsa_sll_elem_t *mid1 = __partition (head, tail, pivot, false);
    struct __cdsa_sll_elem_t *mid2 = __partition (mid1, tail, pivot, true);

    __cdsa_sll_quicksort (head, mid1);
    __cdsa_sll_quicksort (mid2, tail);
}

void
cdsa_sll_sort_inplace_lt (cdsa_sll_t this)
{
    __cdsa_sll_quicksort (this->front, NULL);
}
