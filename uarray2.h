/**************************************************************
 * Interface for UArray2_T                                    *
 * COMP 40  Homework 2                                        *
 * written by: Michael Edegware and Alejandro Vollbrechthausen*
 * Date:       2/7/2017                                       *
 * Purpose:    functions, constants and variable declarations *
 *             for uarray2.c                                  *
 * Joke:       A programmer was going out for his usual       *
 *             evening when his spouce told him,"while you're *
 *             out, buy me some eggs!" He never returned home *
 **************************************************************/ 
#include <uarray.h>  

#ifndef ARRAY2_INCLUDED
#define ARRAY2_INCLUDED

#define T UArray2_T
typedef struct T *T; 
typedef void UArray2_helpApply(int col, int row, T arr2, void\
                                             *elem, void *cl);

extern T       UArray2_new(int height, int width, int size);
extern void    UArray2_free(T *arr2);
extern int     UArray2_height(T arr2);
extern int     UArray2_width(T arr2);
extern int     UArray2_size(T arr2); 
extern void   *UArray2_at(T arr2, int col, int row);
extern void    UArray2_map_row_major(T arr2, UArray2_helpApply\
                                                 apply, void *cl);
extern void    UArray2_map_col_major(T arr2, UArray2_helpApply\
                                               apply, void *cl);
#undef T
#endif