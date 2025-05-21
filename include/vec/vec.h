#pragma once

#ifndef CDSA_VEC_VEC_H
#define CDSA_VEC_VEC_H

#include <stddef.h>
#include <stdint.h>

typedef uint64_t __cdsa_vec_val_t;

typedef struct __cdsa_vec_struct {
    size_t cap;
    size_t siz;
    __cdsa_vec_val_t *ptr;
} cdsa_vec_t[1];

extern void cdsa_vec_init (cdsa_vec_t this);

extern void cdsa_vec_deinit (cdsa_vec_t this);

extern void cdsa_vec_assign (cdsa_vec_t this, size_t siz, __cdsa_vec_val_t v);

extern void cdsa_vec_resiz (cdsa_vec_t this, size_t siz);

extern void cdsa_vec_reserv (cdsa_vec_t this, size_t siz);

extern __cdsa_vec_val_t cdsa_vec_at (cdsa_vec_t this, size_t pos);

extern void cdsa_vec_set (cdsa_vec_t this, size_t pos, __cdsa_vec_val_t v);

extern void cdsa_vec_clear (cdsa_vec_t this);

extern void cdsa_vec_insert (cdsa_vec_t this, size_t pos, __cdsa_vec_val_t v);

extern void cdsa_vec_erase (cdsa_vec_t this, size_t pos);

extern void cdsa_vec_pushb (cdsa_vec_t this, __cdsa_vec_val_t v);

extern void cdsa_vec_popb (cdsa_vec_t this);

extern void cdsa_vec_swap (cdsa_vec_t this, size_t i, size_t j);

extern void cdsa_vec_sort (cdsa_vec_t this);

extern void cdsa_vec_sort_compar (cdsa_vec_t this,
                                  int (*compar) (const void *, const void *));

#endif // !CDSA_VEC_VEC_H
