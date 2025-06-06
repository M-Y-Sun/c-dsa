#include <stdio.h>
#include <stdlib.h>

#include "algs.h"
#include "heap.h"
#include "poly.h"
#include "sll.h"
#include "vec.h"

static int
lt (const void *lhs, const void *rhs)
{
    return *(int *)lhs - *(int *)rhs;
}

void
print_vec (cdsa_vec_t v)
{
    for (size_t i = 0; i < v->siz; ++i)
        printf ("%llu ", v->ptr[i]);

    putchar ('\n');
}

int
main (void)
{
    cdsa_heap_t heap;

    // int init[] = { 0, 1, 2 };
    // cdsa_heap_init_arr (heap, init, sizeof (init) / sizeof (init[0]),
    //                     sizeof (init[0]));

    if (cdsa_heap_init (heap, 3) != 3) {
        perror ("malloc failed");
        return EXIT_FAILURE;
    }

    cdsa_heap_insert (heap, 8);
    printf ("min after insert 8: %lld\n", cdsa_heap_top (heap));

    cdsa_heap_insert (heap, 6);
    printf ("min after insert 6: %lld\n", cdsa_heap_top (heap));

    cdsa_heap_insert (heap, 10);
    printf ("min after insert 10: %lld\n", cdsa_heap_top (heap));

    if (cdsa_heap_expand (heap, 7) != 7) {
        perror ("realloc failed");
        return EXIT_FAILURE;
    }

    cdsa_heap_insert (heap, 2);
    printf ("min after insert 2: %lld\n", cdsa_heap_top (heap));

    cdsa_heap_delete (heap);
    printf ("min after delete: %lld\n", cdsa_heap_top (heap));

    cdsa_heap_deinit (heap);

    puts ("--------");

    cdsa_sll_t sll;
    cdsa_sll_init (sll); // set up the sll
    printf ("[   \033[1;32mOK\033[0m   ] sll init\n");

    // vassign(sll, 3);  <- sets the sll to size 3 with values of 0. Only
    // works when sll is empty

    // append 3 values to the sll
    cdsa_sll_pushb (sll, 0);
    cdsa_sll_pushb (sll, 1);
    cdsa_sll_pushb (sll, 2);

    // print information
    // size() gets the size of the sll
    printf ("size: %lu\n", cdsa_sll_size (sll));
    // get gets the address of the value at a specific position
    printf ("0th index: %llu\n", *cdsa_sll_get (sll, 0));
    printf ("1st index: %llu\n", *cdsa_sll_get (sll, 1));
    printf ("2nd index: %llu\n", *cdsa_sll_get (sll, 2));

    // int bv = popb(sll);  <- deletes the last value and stores it in bv
    // int fv = popf(sll);  <- deletes the first value and stores it in fv

    cdsa_sll_erase (sll, 1); // erases the element at a certain index
    printf ("updated 1st index after erase: %llu\n", *cdsa_sll_get (sll, 1));

    // inserts an element at a certain index with a certain value
    cdsa_sll_insert (sll, 1, 4);
    printf ("updated 1st index after insertion: %llu\n",
            *cdsa_sll_get (sll, 1));

    // vclear(sll);  // deletes all elements

    *cdsa_sll_get (sll, 1) = 8;
    printf ("updated 1st index after change: %llu\n", *cdsa_sll_get (sll, 1));

    cdsa_sll_swap (sll, 1, 2); // swaps two elements
    printf ("updated 1st index after swap: %llu\n", *cdsa_sll_get (sll, 1));

    cdsa_sll_print_all (sll);

    cdsa_sll_resize (sll, 5, -1); // resizes the sll to a certain size with
                                  // undefined values set to a certain value
    printf ("size after first resize: %lu\n", cdsa_sll_size (sll));

    cdsa_sll_resize (sll, 3, -1);
    printf ("size after second resize: %lu\n", cdsa_sll_size (sll));

    cdsa_sll_print_all (sll);

    cdsa_sll_swap (sll, 1, 2);
    cdsa_sll_pushb (sll, 6);
    cdsa_sll_pushb (sll, 1);
    cdsa_sll_pushb (sll, 16);
    cdsa_sll_pushb (sll, 5);
    cdsa_sll_pushb (sll, 8);
    cdsa_sll_print_all (sll);
    // cdsa_sll_sort_inplace_lt (sll);
    cdsa_sll_sort (sll, lt);
    cdsa_sll_print_all (sll);

    // gets the first and last value respectively
    printf ("(first value, last value): (%llu, %llu)\n", cdsa_sll_front (sll),
            cdsa_sll_back (sll));

    printf ("elements 0 to 1: ");
    cdsa_sll_print (sll, 0, 2);
    printf ("final sll: ");
    cdsa_sll_print_all (sll);
    // computes the sum of the elements in a certain range, returns int64_t
    printf ("sum of all elements: %lld\n",
            cdsa_sll_sum (sll, 0, cdsa_sll_size (sll)));

    // computes the product of all the elements in a certain range, returns
    // int64_t
    printf ("product of elements 1 to end: %lld\n",
            cdsa_sll_prod (sll, 1, cdsa_sll_size (sll)));

    cdsa_sll_deinit (sll); // clean up sll and free memory
    printf ("[   \033[1;32mOK\033[0m   ] sll deinit\n");

    puts ("--------");

    cdsa_vec_t vec;
    cdsa_vec_init (vec);
    printf ("[   \033[1;32mOK\033[0m   ] vec init\n");

    cdsa_vec_pushb (vec, 0);
    cdsa_vec_pushb (vec, 1);
    cdsa_vec_pushb (vec, 2);

    printf ("size: %zu\n", vec->siz);
    printf ("0th index: %llu\n", vec->ptr[0]);
    printf ("1st index: %llu\n", vec->ptr[1]);
    printf ("2nd index: %llu\n", vec->ptr[2]);

    cdsa_vec_erase (vec, 1); // erases the element at a certain index
    printf ("updated 1st index after erase: %llu\n", vec->ptr[1]);

    cdsa_vec_insert (vec, 1, 4);
    printf ("updated 1st index after insertion: %llu\n", vec->ptr[1]);

    vec->ptr[1] = 8;
    printf ("updated 1st index after change: %llu\n", vec->ptr[1]);

    cdsa_vec_swap (vec, 1, 2); // swaps two elements
    printf ("updated 1st index after swap: %llu\n", vec->ptr[1]);

    print_vec (vec);

    cdsa_vec_resiz (vec, 5);
    printf ("size after first resize: %lu\n", vec->siz);

    print_vec (vec);

    cdsa_vec_resiz (vec, 3);
    printf ("size after second resize: %lu\n", vec->siz);

    print_vec (vec);

    cdsa_vec_swap (vec, 1, 2);
    cdsa_vec_pushb (vec, 6);
    cdsa_vec_pushb (vec, 1);
    cdsa_vec_pushb (vec, 16);
    cdsa_vec_pushb (vec, 5);
    cdsa_vec_pushb (vec, 8);
    print_vec (vec);
    cdsa_vec_sort (vec);
    print_vec (vec);

    // gets the first and last value respectively
    printf ("(first value, last value): (%llu, %llu)\n", vec->ptr[0],
            vec->ptr[vec->siz - 1]);

    printf ("final vec: ");

    cdsa_vec_deinit (vec); // clean up vec and free memory
    printf ("[   \033[1;32mOK\033[0m   ] vec deinit\n");

    puts ("--------");

    // 1 + 3x + 2x^2 + 6x^3 + 2x^4
    long double coefs[] = { 1, 3, 2, 6, 2 };

    cdsa_poly_t p;
    cdsa_poly_init (p, coefs, 4);
    cdsa_poly_print (p);

    printf ("eval at 1: %Lf\n", cdsa_poly_eval (p, 1));
    printf ("eval at 2: %Lf\n", cdsa_poly_eval (p, 2));

    cdsa_poly_ddx_n (p, 1);
    cdsa_poly_print (p);
    cdsa_poly_ddx_n (p, 1);
    cdsa_poly_print (p);

    cdsa_poly_deinit (p);

    // x^5 - 23
    long double coefs2[] = { -23, 0, 0, 0, 0, 1 };

    cdsa_poly_init (p, coefs2, 5);

    double r = cdsa_algs_newton_estim_n (p, 2, 3);
    printf ("fifth root of 23 extimate: %lf\n", r);

    cdsa_poly_deinit (p);

    return 0;
}
