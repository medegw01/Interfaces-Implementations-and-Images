/**************************************************************
 * Implementation for UArray2_T                               *
 * COMP 40  Homework 2                                        *
 * written by: Michael Edegware and Alejandro Vollbrechthausen*
 * Date:       2/13/2017                                      *
 * Purpose:    An unboxed-array abstraction to support        *
 *             two-dimensional arrays. It is implemented with *
 *             a UArray from Hanson's interface               *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <uarray.h> 
#include <uarray2.h>



/*the struct T has a height which is the a UArray_T of
  length 'height' and each index of row_ptr points to 
  to another UArray_T array of col_ptr of length width
  and size, 'array_size'
  */
struct UArray2_T {
        int height;/*number of rows*/
        int width;/*number of columes*/
        int size;
        UArray_T rows_ptr; 
};


/* terminated the program by unchecked runtime error*/
static void terminate(){
UArray_T err = UArray_new(1, sizeof(UArray_T));
/*a checked runtime error since the index requested
is more than array size */
UArray_at(err, 3);
}


/* returns the address of a row in the 2D array when called.*/
static inline UArray_T *get_row(UArray2_T arr2, int row_index)
{   
        UArray_T *tmp = UArray_at(arr2->rows_ptr, row_index);
        return tmp; 
}

/*this checks for fundamental errors like array length,whether
 space is allocated or whether a simple array address can be reached*/
static void check_for_error(UArray2_T arr2)
{ 
	if (arr2 != NULL) {
		if ((UArray_length(arr2 ->rows_ptr) == arr2 ->height) &&\
               UArray_size(arr2 ->rows_ptr) == sizeof(UArray_T)) {
			if ((UArray_length(*get_row(arr2, 0)) == arr2 ->width && \
           UArray_size(*get_row(arr2, 0)) == arr2 ->size)\
                || arr2 ->height == 0) {
				return;
			}
		}
	}
  terminate();
}

/*allocated a space for a 2D array and populates this 2D array with
 the inputed values. It allow terminates with runtime error when the
 space is not properlly allocated.
 */
UArray2_T UArray2_new(int width, int height, int size)
{
        UArray2_T arr2  = malloc(sizeof(*arr2));/*manually allocates 
        space for the outer row array*/
        arr2->width    = width;
        arr2->height   = height;
        arr2->size     = size;
        /*create 'height' numbers of rows using the UArray_T interface*/
        arr2->rows_ptr = UArray_new(height, sizeof(UArray_T));
        for (int i = 0; i < height; i++) {
            UArray_T *row_index = UArray_at(arr2->rows_ptr, i);
            /*point each index of row to col array of length 'width'*/
            *row_index = UArray_new(width, size);
            }
        check_for_error(arr2);
        return arr2;
}

/*UArray2_free() deallocates spaces used for the UArray2_T*/
void UArray2_free(UArray2_T *arr2)
{
        check_for_error(*arr2);/*first check for error to ensure 
        array is valid*/

        for (int i = 0; i < (*arr2)->height; i++) {
                 UArray_free(get_row(*arr2, i));
                 /*first free col UArray then free the UArray2*/
        }
        UArray_free(&(*arr2)->rows_ptr);
        free(*arr2);
        /*finally free UArray2 from alloc*/
}

/*returns a pointer to the element in the [row, col] index*/
void *UArray2_at(UArray2_T arr2, int col, int row){
        check_for_error(arr2);
        return UArray_at(*get_row(arr2, row), col);
}

/*
*UArray2_height returns the height or number of rows of the UArray2_T.
*But first check for errors to ensure it's a valid UArray2_T
*/
int UArray2_height(UArray2_T arr2)
{
        check_for_error(arr2);
        return arr2->height;
}

/*
*UArray2_width() returns the width or number of columns of the UArray2_T.
*But first check for errors to ensure it's a valid UArray2_T
*/
int UArray2_width(UArray2_T arr2)
{
        check_for_error(arr2);
        return arr2->width;
}

/*
*UArray2_size() returns the sizeof element stored in the UArray2_T. But 
*first check for errors to ensure it's a valid UArray2_T
*/
int UArray2_size(UArray2_T arr2) 
{
        check_for_error(arr2);
        return arr2->size;
}

/*
* UArray2_map_row_major() calls the function pointed to by apply() 
* for every element in the 2-d array. 
* Column indices vary more rapidly than row indices.
*/
void  UArray2_map_row_major(UArray2_T arr2, void apply(int col, int row, \
                      UArray2_T arr2, void*elem, void *cl), void *cl)
{
       check_for_error(arr2);
       
       for (int r = 0; r < arr2->height; r++) {
       /*I put this outside col loop so it get's value
       once in every col iteration*/
                UArray_T *row_index = get_row(arr2, r);
                 for (int c = 0; c < arr2->width; c++)
                        apply(c, r, arr2, UArray_at(*row_index, c), cl);
        }
}

/*
* UArray2_map_col_major() calls the function pointed to by apply() 
* for every element in the 2-d array. 
* Row indices vary more rapidly than column indices.
*/
void    UArray2_map_col_major(UArray2_T arr2, void apply(int col, int row, \
                      UArray2_T arr2, void*elem, void *cl), void *cl)
{    
     check_for_error(arr2);
     
     for (int c = 0; c < arr2->width; c++) {
                    for (int r = 0; r < arr2->height; r++){
                        apply(c, r, arr2, UArray_at(*(get_row(arr2, r)),c), cl);
        }
        }
}