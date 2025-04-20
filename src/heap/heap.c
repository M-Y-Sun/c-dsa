#include <stddef.h>
#include <stdlib.h>

#include "heap.h"

#define root_(h) h->c_[CDSA_HEAP_ROOT_POS]
#define lc_(i)   ((((i)-CDSA_HEAP_ROOT_POS) << 1) + 1 + CDSA_HEAP_ROOT_POS)
#define rc_(i)   ((((i)-CDSA_HEAP_ROOT_POS) << 1) + 2 + CDSA_HEAP_ROOT_POS)
#define pa_(i)   (((i)-1 - (((i) & 1) ^ 1)) >> 1)

static void
heap_swap_ (volatile const cdsa_heap_t heap, size_t i, size_t j)
{
    __heap_val_t tmp = heap->c_[i];
    heap->c_[i] = heap->c_[j];
    heap->c_[j] = tmp;
}

static void
bubble_up_ (volatile const cdsa_heap_t heap, size_t pos)
{
    if (pos == 0)
        return;

    size_t pa_pos;
    int c;

    while (
        pos != CDSA_HEAP_ROOT_POS
        && (c = heap->compar_ (&heap->c_[pos], &heap->c_[pa_pos = pa_ (pos)]))
               < 0) {
        heap_swap_ (heap, pos, pa_pos);
        pos = pa_pos;
    }
}

static void
bubble_down_ (volatile const cdsa_heap_t heap, size_t pos)
{
    size_t chd_pos = lc_ (pos);

    if (chd_pos >= heap->next_)
        return;

    chd_pos += (chd_pos + 1 < heap->next_
                && heap->c_[chd_pos] > heap->c_[chd_pos + 1]);

    if (heap->compar_ (&heap->c_[pos], &heap->c_[chd_pos]) <= 0)
        return;

    heap_swap_ (heap, pos, chd_pos);
    bubble_down_ (heap, chd_pos);
}

static int
__lt (const void *p_lhs, const void *p_rhs)
{
    return *(__heap_val_t *)p_lhs - *(__heap_val_t *)p_rhs;
}

/**
 * @return The number of bytes allocated, 0 on error (`errno` is set)
 */
size_t
cdsa_heap_init (volatile cdsa_heap_t this, size_t max_sz)
{
    return cdsa_heap_init_compar (this, max_sz, __lt);
}

/**
 * @return The number of bytes allocated, 0 on error (`errno` is set)
 */
size_t
cdsa_heap_init_compar (volatile cdsa_heap_t this, size_t maxsiz,
                       int (*compar) (const void *, const void *))
{
    if ((this->c_ = malloc (maxsiz * sizeof (__heap_val_t))) == NULL)
        return 0;

    this->next_ = 0;
    this->compar_ = compar;

    return maxsiz;
}

void
cdsa_heap_init_arr (volatile cdsa_heap_t this, __heap_val_t *arr, size_t len)
{
    this->c_ = arr;

    for (size_t i = (len >> 1) - 1; i >= 0; --i) {
        bubble_down_ (this, i);
    }
}

/**
 * @return The number of bytes allocated, 0 on error (`errno` is set)
 */
size_t
cdsa_heap_expand (volatile cdsa_heap_t this, size_t siz)
{
    if ((this->c_ = realloc (this->c_, siz * sizeof (__heap_val_t))) == NULL)
        return 0;

    return siz;
}

void cdsa_heap_deinit (volatile cdsa_heap_t this)
{
    free (this->c_);
    this = NULL;
}

__heap_val_t
cdsa_heap_top (volatile const cdsa_heap_t this)
{
    return this->c_[0];
}

void
cdsa_heap_insert (volatile cdsa_heap_t this, __heap_val_t v)
{
    size_t pos = this->next_++;
    this->c_[pos] = v;
    bubble_up_ (this, pos);
}

void cdsa_heap_delete (volatile cdsa_heap_t this)
{
    heap_swap_ (this, CDSA_HEAP_ROOT_POS, --this->next_);
    bubble_down_ (this, CDSA_HEAP_ROOT_POS);
}

void
cdsa_heap_replace_always (volatile const cdsa_heap_t this, __heap_val_t v)
{
    root_ (this) = v;
    bubble_down_ (this, CDSA_HEAP_ROOT_POS);
}

void
cdsa_heap_replace_first (volatile const cdsa_heap_t this, __heap_val_t v)
{
    if (this->compar_ (&v, &root_ (this)) <= 0)
        return;

    root_ (this) = v;
    bubble_down_ (this, CDSA_HEAP_ROOT_POS);
}

#undef pa_
#undef rc_
#undef lc_
#undef root_
