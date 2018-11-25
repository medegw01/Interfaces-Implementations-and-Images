/**************************************************************
 * unblackedges.h                                             *
 * COMP 40  Homework 2                                        *
 * written by: Michael Edegware and Alejandro Vollbrechthausen*
 * Date:       2/13/2017                                      *
 * Purpose:    header folder for unblackedges.c. It holds     *
 *             functions, structure and constants declarations*
 **************************************************************/
 
#include <pnmrdr.h> 
#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "bit2.h" 
#include "uarray.h"
#include "stack.h"


/*Constants declaration*/
const int BLACK = 1;
const int WHITE = 0;
const int EMPTY = 1;

/*
* black_edges_location stores the row and column of
* black edges. This will be used by the satck
*/


typedef struct black_edges_location {
                int col;
                int row;
} black_edges_location;


/*functions declarations*/

/*
* get_file(int argc, char * argv[]) takes file name from main();
* if provided or request a file from standard input and return 
* an opened file(FILE).  It halts with too many input error or
* cannot  open file error
*/
FILE *get_file(int argc, char *argv[]);


/*
* get_image(FILE * fp) returns a pointer to a well formatted and 
* valid image file exit(1) at any instance the image is invalid
* with a  specific erro printout
*/
Pnmrdr_T get_image(FILE *fp);


/*
* pbmread(Pnmrdr_T image) creates a Bit2_T array, reads 
* in every pixel in an image and store it in a 2D array, 
* sequentually, by calling the map_row_major fucntion of
* Bit2_T. It then returns the 2D array of bits
*/ 
Bit2_T pbmread(Pnmrdr_T image);


/*
* helper_pbmread(int col, int row, Bit2_T bit2, int val,
* void *cl) is a helper used by apply() when called in 
*the map_row_major of Bit2_T
*/
void helper_pbmread(int col, int row, Bit2_T bit2, int val,\
    void *cl);

/*
 * analyze_image(Bit2_T bit2) loops through the 2D array
 * and save  any instance of black edges into a stack. 
 * Then unblacked the  edges.
*/
void analyze_image(Bit2_T bit2);


/*
* analyze_row_edges(Bit2_T bit2, Stack_T black_edges) is a
* helper function that loops through the row edges and 
* check if there are  black edges
*/
void analyze_row_edges(Bit2_T bit2, Stack_T black_edges);


/*
* analyze_col_edges(Bit2_T bit2, Stack_T black_edges) is
* a helper function that loops through the columns edges
* and check if there are  black edges
*/
void analyze_col_edges(Bit2_T bit2, Stack_T black_edges);

/*
* analyze_neighboring(Bit2_T bit2, Stack_T black_edges) is a
* helper function that loops through the inner indexes and
* check if there are  black edges
*/
void analyze_neighboring(Bit2_T bit2, Stack_T black_edges);

/*
* helper_analyze_neighboring(int c, int r, Bit2_T bit2, 
* Stack_T  black_edges) checks the left, right, top 
* and bottom neighbours  of a index to see if it 
* qualifies for a black edge. It's called by the 
* analyze_neighboring.
*/
void helper_analyze_neighboring(int c, int r, Bit2_T bit2,\
    Stack_T black_edges);

/*
* unblacken(Stack_T black_edges, Bit2_T bit2) loops 
* through an instance of stack, pop the struct and put
* white to all black edge coordinates saved in the stack
*/
void unblacken(Stack_T black_edges, Bit2_T bit2);

/*
* print_image(Bit2_T bit2) loops through the 2D array with help
* from Bit2_map_row_major and print the value stored in the 
* indexes
*/
void print_image(Bit2_T bit2);

/*
* add_to_stack(int c, int r, Stack_T black_edges) allocates
* memory for a pointer to the struct black_edges_location,
* populate the struct members and save the instance of struct
* to a stack
*/
void add_to_stack(int c, int r, Stack_T black_edges);

/*
* helper_print_image(int col, int row, Bit2_T bit2, int val,
* void *cl) is a helper function for print, it logically determine
* when to print a new line. It's called by print but used by
* apply() int the Bit2_map_row_major during iteration
*/
void helper_print_image(int col, int row,Bit2_T bit2, int val, void *cl);
