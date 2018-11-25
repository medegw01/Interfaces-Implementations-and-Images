/**************************************************************
 * Interface for Bit2_T                                       *
 * COMP 40  Homework 2                                        *
 * written by: Michael Edegware and Alejandro Vollbrechthausen*
 * Date:       2/7/2017                                       *
 * Purpose:    functions, constants and variable declarations *
 *             for uarray2.c                                  *
 * Joke:       Q. Who's a programmer?                         *
 *             A. A  persomn that fix a problem that you don't*
 *                know you have in a way you don't understand *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <uarray.h> 
#include <bit.h> 

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#define T Bit2_T
typedef struct T {
        int height;
        int width;
	UArray_T row_ptr;
} *T; 

extern T         Bit2_new(int width, int height);
extern void      Bit2_free(T *bit2);
extern int       Bit2_height(T bit2);
extern int       Bit2_width(T bit2);

extern int      Bit2_put(T bit2, int column, int row, int v);
extern int Bit2_get(T bit2, int column, int row); 
extern void      Bit2_map_row_major(T bit2, void apply(int c , int r, T bit2,
						 int val, void *cl), void *cl);
extern void      Bit2_map_col_major(T bit2, void apply(int r , int c, T bit2,
						 int val, void *cl), void *cl);
#undef T
#endif