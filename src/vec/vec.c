#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

static uint64_t
np2 (uint64_t v)
{
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    ++v;
    return v;
}

void
cdsa_vec_init (cdsa_vec_t this)
{
    this->cap = 1;
    this->siz = 0;
    this->ptr = malloc (sizeof (__cdsa_vec_val_t));
}

void
cdsa_vec_deinit (cdsa_vec_t this)
{
    free (this->ptr);
}

#define memset_qword(p, v, siz)                                               \
    do {                                                                      \
        uint64_t *p_ = p;                                                     \
                                                                              \
        while (siz--)                                                         \
            *p_++ = v;                                                        \
    } while (0);

#define bytecap(this) (this->cap * sizeof (__cdsa_vec_val_t))
#define bitsiz(this)  (this->siz * sizeof (__cdsa_vec_val_t))

#define resiz(this, siz)                                                      \
    do {                                                                      \
        this->siz = (siz);                                                    \
                                                                              \
        if (this->cap < siz) {                                                \
            this->cap = np2 (siz);                                            \
            this->ptr = realloc (this->ptr, bytecap (this));                  \
        }                                                                     \
    } while (0);

void
cdsa_vec_resiz (cdsa_vec_t this, size_t siz)
{
    int ncap = np2 (siz);
    resiz (this, siz);
    int bncap = bytecap (this);
}

void
cdsa_vec_assign (cdsa_vec_t this, size_t siz, __cdsa_vec_val_t v)
{
    resiz (this, siz);
    memset_qword (this->ptr, v, siz);
}

void
cdsa_vec_reserv (cdsa_vec_t this, size_t cap)
{
    if (this->cap >= cap)
        return;

    this->cap = cap;
    this->ptr = realloc (this->ptr, this->cap * sizeof (__cdsa_vec_val_t));
}

__cdsa_vec_val_t
cdsa_vec_at (cdsa_vec_t this, size_t pos)
{
    return this->ptr[pos];
}

void
cdsa_vec_set (cdsa_vec_t this, size_t pos, __cdsa_vec_val_t v)
{
    this->ptr[pos] = v;
}

void
cdsa_vec_clear (cdsa_vec_t this)
{
    this->siz = 0;
}

#define expand(this)                                                          \
    do {                                                                      \
        this->cap <<= 1;                                                      \
        this->ptr = realloc (this->ptr, bytecap (this));                      \
    } while (0);

void
cdsa_vec_insert (cdsa_vec_t this, size_t pos, __cdsa_vec_val_t v)
{
    if (this->siz == this->cap)
        expand (this);

    memmove (this->ptr + pos + 1, this->ptr + pos,
             bitsiz (this) - pos * sizeof (__cdsa_vec_val_t));

    this->ptr[pos] = v;
}

void
cdsa_vec_erase (cdsa_vec_t this, size_t pos)
{
    memmove (this->ptr + pos, this->ptr + pos + 1,
             bitsiz (this) - (pos + 1) * sizeof (__cdsa_vec_val_t));
}

void
cdsa_vec_pushb (cdsa_vec_t this, __cdsa_vec_val_t v)
{
    if (this->siz == this->cap)
        expand (this);

    this->ptr[this->siz++] = v;
}

void
cdsa_vec_popb (cdsa_vec_t this)
{
    --this->siz;
}

void
cdsa_vec_swap (cdsa_vec_t this, size_t i, size_t j)
{
    int tmp = this->ptr[i];
    this->ptr[i] = this->ptr[j];
    this->ptr[j] = tmp;
}

static int
lt (const void *lhs, const void *rhs)
{
    const __cdsa_vec_val_t l = *(__cdsa_vec_val_t *)lhs;
    const __cdsa_vec_val_t r = *(__cdsa_vec_val_t *)rhs;
    return (l > r) - (l < r);
}

void
cdsa_vec_sort (cdsa_vec_t this)
{
    qsort (this->ptr, this->siz, sizeof (__cdsa_vec_val_t), lt);
}

void
cdsa_vec_sort_compar (cdsa_vec_t this,
                      int (*compar) (const void *, const void *))
{
    qsort (this->ptr, this->siz, sizeof (__cdsa_vec_val_t), compar);
}

#undef expand
#undef resiz
#undef memset_qword
