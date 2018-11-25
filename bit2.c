/**************************************************************
 * Implemintation for Bit2_T                                  *
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
#include <bit2.h> 

#define T Bit2_T

/*allocated a space for a 2D array and populates this 2D bit array 
 with the inputed values. It allow terminates with runtime error 
 when the space is not properlly allocated.
*/
extern T Bit2_new(int width, int height) {
	T bit2 = malloc(sizeof( * bit2));
	bit2->width = width;
	bit2->height = height;
	UArray_T tmp = UArray_new(height, sizeof(Bit_T));
	bit2->row_ptr = tmp;
	for (int i = 0; i < height; i++){
   *((Bit_T *) UArray_at(bit2->row_ptr, i)) = Bit_new(width);
	}
	return bit2;
}


/*Bit2_free() deallocates spaces used for the UArray2_T*/
void Bit2_free(T * bit2) {
	
  for(int i = 0; i < (*bit2)->height; i++){
		Bit_free((Bit_T*) UArray_at((* bit2)->row_ptr, i));
	}
	UArray_free(&((*bit2)->row_ptr));
	free(*bit2);

}


/*
*UArray2_height returns the height or number of rows of the Bit2_T.
*/
int Bit2_height(T bit2){
	return bit2->height;
}


/*
*UArray2_width() returns the width or number of columns of the Bit2_T.
*/
int Bit2_width(T bit2){
	return bit2->width;
}

/*
* Bit2_put() puts a bit(v) into Bit2_T array and return the inputed
* value
*/
int Bit2_put(T bit2, int column, int row, int v) {
	v = Bit_put(*((Bit_T *) UArray_at(bit2->row_ptr, row)),\
    column, v);
  return v;
}

/*
* Bit2_get() gets a bit stored in the requested  into row and column.
* It terminates with runtime error is the row or column requested is
* out of scope 
*/
int Bit2_get(T bit2, int column, int row) {
	int v = Bit_get(*((Bit_T *) UArray_at(bit2->row_ptr, row)),\
                column);
  return v;
  }


/*
* UArray2_map_col_major() calls the function pointed to by apply() 
* for every element in the 2-d array. 
* Row indices vary more rapidly than column indices.
*/
void Bit2_map_col_major(T bit2, void apply(int c, int r, T bit2,\
                   int val, void * cl), void * cl) {
	
  for (int r = 0; r < bit2->height; r++){
		  Bit_T row_index = *((Bit_T *) UArray_at(bit2->row_ptr, r));
		  for (int c = 0; c < bit2->width; c++){
		  apply(c, r, bit2, Bit_get(row_index, c), cl);
      }
	}
}

/*
* Bit2_map_row_major() calls the function pointed to by apply() 
* for every element in the 2-d bit array. 
* Column indices vary more rapidly than row indices.
*/
void Bit2_map_row_major(T bit2, void apply(int r, int c, T bit2,\
                     int val, void *cl), void *cl){
	
  for (int c = 0; c < bit2 ->width; c++) {
		for (int r = 0; r < bit2 ->height; r++) {
			Bit_T row_index = *((Bit_T *) UArray_at(bit2->row_ptr, r));
			apply(c, r, bit2, Bit_get(row_index, c), cl);
		}
	}
}