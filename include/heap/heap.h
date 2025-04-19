#pragma once

#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>

#define CDSA_HEAP_ROOT_POS 0

typedef int64_t __heap_val_t;

typedef struct {
    __heap_val_t *c_;
    size_t next_;
} cdsa_heap_t[1];

extern void cdsa_heap_init (volatile cdsa_heap_t this, size_t max_sz);

extern void cdsa_heap_init_arr (volatile cdsa_heap_t this, __heap_val_t *arr,
                                size_t len);

extern void cdsa_heap_expand (volatile cdsa_heap_t this, size_t new_sz);

extern void cdsa_heap_deinit (volatile cdsa_heap_t this);

extern __heap_val_t cdsa_heap_min (volatile const cdsa_heap_t this);

extern void cdsa_heap_insert (volatile cdsa_heap_t this, __heap_val_t v);

extern void cdsa_heap_delete (volatile cdsa_heap_t this);

extern void cdsa_heap_replace_always (volatile const cdsa_heap_t this,
                                      __heap_val_t v);

extern void cdsa_heap_replace_first (volatile const cdsa_heap_t this,
                                     __heap_val_t v);

#endif
