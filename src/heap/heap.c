#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

#define root_(h) h->c_[CDSA_HEAP_ROOT_POS]
#define lc_(i)   ((((i)-CDSA_HEAP_ROOT_POS) << 1) + 1 + CDSA_HEAP_ROOT_POS)
#define rc_(i)   ((((i)-CDSA_HEAP_ROOT_POS) << 1) + 2 + CDSA_HEAP_ROOT_POS)
#define pa_(i)   (((i)-1 - (((i) & 1) ^ 1)) >> 1)

static void
__cdsa_heap_swap (const cdsa_heap_t heap, size_t i, size_t j)
{
    __cdsa_heap_val_t tmp = heap->c_[i];
    heap->c_[i] = heap->c_[j];
    heap->c_[j] = tmp;
}

static void
__bubble_up (const cdsa_heap_t heap, size_t pos)
{
    if (pos == 0)
        return;

    size_t pa_pos;
    int c;

    while (
        pos != CDSA_HEAP_ROOT_POS
        && (c = heap->compar_ (&heap->c_[pos], &heap->c_[pa_pos = pa_ (pos)]))
               < 0) {
        __cdsa_heap_swap (heap, pos, pa_pos);
        pos = pa_pos;
    }
}

static void
__bubble_down (const cdsa_heap_t heap, size_t pos)
{
    size_t chd_pos = lc_ (pos);

    if (chd_pos >= heap->size)
        return;

    // use rc = lc + 1 if rc is smaller
    chd_pos += (chd_pos + 1 < heap->size
                && heap->compar_ (&heap->c_[chd_pos], &heap->c_[chd_pos + 1]))
               > 0;

    if (heap->compar_ (&heap->c_[pos], &heap->c_[chd_pos]) <= 0)
        return;

    __cdsa_heap_swap (heap, pos, chd_pos);
    __bubble_down (heap, chd_pos);
}

static int
__lt (const void *p_lhs, const void *p_rhs)
{
    __cdsa_heap_val_t lhs = *(const __cdsa_heap_val_t *)p_lhs;
    __cdsa_heap_val_t rhs = *(const __cdsa_heap_val_t *)p_rhs;
    return (lhs > rhs) - (lhs < rhs);
}

/**
 * @return The number of bytes allocated, 0 on error (`errno` is set)
 */
size_t
cdsa_heap_init (cdsa_heap_t this, size_t max_sz)
{
    return cdsa_heap_init_compar (this, max_sz, __lt);
}

/**
 * @return The number of bytes allocated, 0 on error (`errno` is set)
 */
size_t
cdsa_heap_init_compar (cdsa_heap_t this, size_t maxsiz,
                       int (*compar) (const void *, const void *))
{
    if ((this->c_ = malloc (maxsiz * sizeof (__cdsa_heap_val_t))) == NULL)
        return 0;

    this->size = 0;
    this->compar_ = compar;

    return maxsiz;
}

void
cdsa_heap_init_arr (cdsa_heap_t this, const void *const arr, size_t siz,
                    size_t width)
{
    cdsa_heap_init_arr_compar (this, arr, siz, width, __lt);
}

void
cdsa_heap_init_arr_compar (cdsa_heap_t this, const void *const arr, size_t siz,
                           size_t width,
                           int (*compar) (const void *, const void *))
{
    // this->c_ = (__cdsa_heap_val_t *)arr;
    const size_t nbytes = siz * width;
    this->c_ = malloc (nbytes);
    memcpy (this->c_, arr, nbytes);
    this->size = siz;
    this->compar_ = compar;

    for (size_t i = (siz >> 1) - 1;; --i) {
        __bubble_down (this, i);

        if (i == 0)
            break;
    }
}

/**
 * @return The number of bytes allocated, 0 on error (`errno` is set)
 */
size_t
cdsa_heap_expand (cdsa_heap_t this, size_t siz)
{
    if ((this->c_ = realloc (this->c_, siz * sizeof (__cdsa_heap_val_t))) == NULL)
        return 0;

    return siz;
}

void
cdsa_heap_deinit (cdsa_heap_t this)
{
    free (this->c_);
    this = NULL;
}

__cdsa_heap_val_t
cdsa_heap_top (const cdsa_heap_t this)
{
    return this->c_[0];
}

void
cdsa_heap_insert (cdsa_heap_t this, __cdsa_heap_val_t v)
{
    size_t pos = this->size++;
    this->c_[pos] = v;
    __bubble_up (this, pos);
}

void
cdsa_heap_delete (cdsa_heap_t this)
{
    __cdsa_heap_swap (this, CDSA_HEAP_ROOT_POS, --this->size);
    __bubble_down (this, CDSA_HEAP_ROOT_POS);
}

void
cdsa_heap_replace_always (const cdsa_heap_t this, __cdsa_heap_val_t v)
{
    root_ (this) = v;
    __bubble_down (this, CDSA_HEAP_ROOT_POS);
}

void
cdsa_heap_replace_first (const cdsa_heap_t this, __cdsa_heap_val_t v)
{
    if (this->compar_ (&v, &root_ (this)) <= 0)
        return;

    root_ (this) = v;
    __bubble_down (this, CDSA_HEAP_ROOT_POS);
}

#undef pa_
#undef rc_
#undef lc_
#undef root_
