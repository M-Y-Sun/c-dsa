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
    int (*compar_) (const void *, const void *);
} cdsa_heap_t[1];

/**
 * @return The number of bytes allocated, 0 on error (`errno` is set)
 */
extern size_t cdsa_heap_init (cdsa_heap_t this, size_t max_sz);

/**
 * @return The number of bytes allocated, 0 on error (`errno` is set)
 */
extern size_t cdsa_heap_init_compar (cdsa_heap_t this, size_t max_sz,
                                     int (*compar) (const void *,
                                                    const void *));

extern void cdsa_heap_init_arr (cdsa_heap_t this, __heap_val_t *arr,
                                size_t len);

/**
 * @return The number of bytes allocated, 0 on error (`errno` is set)
 */
extern size_t cdsa_heap_expand (cdsa_heap_t this, size_t new_sz);

extern void cdsa_heap_deinit (cdsa_heap_t this);

extern __heap_val_t cdsa_heap_top (const cdsa_heap_t this);

extern void cdsa_heap_insert (cdsa_heap_t this, __heap_val_t v);

extern void cdsa_heap_delete (cdsa_heap_t this);

extern void cdsa_heap_replace_always (const cdsa_heap_t this, __heap_val_t v);

extern void cdsa_heap_replace_first (const cdsa_heap_t this, __heap_val_t v);

#endif
