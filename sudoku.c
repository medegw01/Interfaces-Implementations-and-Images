/**************************************************************
 * Sudoku                                                     *
 * COMP 40  Homework 2                                        *
 * written by: Michael Edegware and Alejandro Vollbrechthausen*
 * Date:       2/13/2017                                      *
 * Purpose:    reads in a valid graymap file and return 0 if  *
 *             it's a solved sudoku or 1 is unsolved or any   *
 *             other form of error occur while anayzing file  *
 **************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <pnmrdr.h> 
#include <uarray2.h>

/*functions declaration*/
void analyze_Image(FILE * fp, UArray2_T * sudoku_2d);
void populate(UArray2_T * uarray2, int col, int row,\
               Pnmrdr_T * image, FILE * fp);
void analyze_rows(int col, int row, UArray2_T uarray2,\
               void *elem, void * cl);
void analyze_cols(int col, int row, UArray2_T uarray2,\
               void * elem, void * cl);
void analyze_3x3box(UArray2_T uarray2);
void helper_3x3box(int start_c_1, int start_r_2,\
               UArray2_T uarray2); 

int main(int argc, char *argv[]) {

	UArray2_T sudoku_2d;
	if (argc > 2) {
		exit(1);
	}
	FILE *fp;

	if (argc == 2) {
		fp = fopen(argv[1], "rb");
	}
	else {
		fp = stdin;
	}
	if (fp == NULL) {
		exit(1);
	}
	analyze_Image(fp, &sudoku_2d);
	if (fp != stdin) {
		fclose(fp);
	}
	void * cl = malloc(sizeof(int));
	/*if image is valid and properly
                 indexed in the 2D array, analyze the soduko*/
	*(int *) cl = 0;

	UArray2_map_row_major(sudoku_2d, analyze_cols, cl);
	/*to analyse a column, we use the row_major since it iterates
  down a column*/
	UArray2_map_col_major(sudoku_2d, analyze_rows, cl);
	/*to analyse a row, we use the row_major since it iterates
  accross a column*/

	analyze_3x3box(sudoku_2d);
	/* check the 3x3 section to see if it's 
     solved*/
	free(cl);
	/* success! :) */
	UArray2_free(&sudoku_2d);

	exit(0);
}


/*
* analyze_Image() reads in a file pointer passed from main
* and determine if it's a valid graymap file. If valid, it
* creates a 2D array, call populate() to store each pixel
* from picture to a the array and then free the file reader
*/
void analyze_Image(FILE *fp, UArray2_T *sudoku_2d) {
 
 /*creates a pnmrdr readear*/
	Pnmrdr_T r = Pnmrdr_new(fp);
	if (r == NULL) {/* check if file openned correctly*/
		Pnmrdr_free(&r);
		if (fp != stdin) {
			fclose(fp);
		}
		exit(1);
	}

	Pnmrdr_mapdata sudoku_map = Pnmrdr_data(r);
	if (sudoku_map.width != 9 && sudoku_map.height != 9 \
         && sudoku_map.denominator != 9) {
		Pnmrdr_free(&r);
		if (fp != stdin) {
			fclose(fp);
		}
	}

	if (sudoku_map.type != Pnmrdr_gray) {
		Pnmrdr_free(&r);
		/*free as r is not a correct file type*/
		if (fp != stdin) {
			fclose(fp);
		}

		exit(1);
	}

	*sudoku_2d = UArray2_new(sudoku_map.width, sudoku_map.height,\
     sizeof(int));
	if (sudoku_2d == NULL) {
		exit(1);
		Pnmrdr_free(&r);
	}
 /* if file is valid and dimention is 9X9, store pixel in a 2D array*/
	populate(sudoku_2d, sudoku_map.width, sudoku_map.height,\
          &r, fp);
	Pnmrdr_free(&r);
}


/*
*populate() stores each pixel on a valid file into a 2D array
*sequentially. 
*/
void populate(UArray2_T *uarray2, int col, int row, \
              Pnmrdr_T *image, FILE *fp) {
	
  int checker = 0;
	
  for (int r = 0; r < row; r++) {
		for (int c = 0; c < col; c++) {
			int pixel = Pnmrdr_get(*image);
			if (pixel < 1 || pixel > 9) {
				checker = -1;
			}
			int * num_ptr = UArray2_at(*uarray2, c, r);*num_ptr\
          = pixel;
		}
	}
	
  if (checker == -1) {
		UArray2_free(uarray2);
		Pnmrdr_free(image);

		if (fp != stdin) {
			fclose(fp);
		}
		exit(1);
	}
}


/*
* analyze_rows() loops through accross a row major and sum 
* the values. If the sum is not exaclty 45, then the row is
* not solved and thus exit a value of 1
*/
void analyze_rows(int col, int row, UArray2_T uarray2,\
     void *elem, void *cl) {
 
 (void) elem;
  int *tmp = UArray2_at(uarray2, col, row);
  *(int *) cl += *tmp;
	
  if (row == 8) {
		/* if it is at the end of row, the row much sum up to
   45 or it's an unsolved sudoku*/
		
    if (*(int*) cl != 45) {
			free(cl);
			UArray2_free(&uarray2);
			exit(1);
		} 
    *(int *) cl = 0;
		/*reset to one if done with a particular row or colume*/
	}
}


/*
* analyze_cols() loops down a colum and sum 
* the values. If the sum is not exaclty 45, then the column is
* not solved and thus exit a value of 1
*/
void analyze_cols(int col, int row, UArray2_T uarray2,\
                    void *elem, void *cl) { 
 (void) elem;
	int * tmp = UArray2_at(uarray2, col, row);
  *(int *) cl += *tmp;
	
  if (col == 8) {
		/* if it is at the end of col, the col much sum up to
   45 or it's an unsolved sudoku*/
		
    if (*(int *) cl != 45) {
			free(cl);
			UArray2_free(&uarray2);
			exit(1);
		} 
    *(int *) cl = 0;
		/*reset to one if done with a particular row or colume*/
	}
}


/*
 *analyze_3x3box () loop through the 9x9 2D array such that 
 *element will be accessed 3x3 and once, this will involve a 
 *helper function that'll be called nine times with two 
 *starting points. Using a mathematical relationship, this
 *can be achieved
 */
void analyze_3x3box(UArray2_T uarray2) {
	
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			helper_3x3box(c, r, uarray2);
		}
	}
}


/*
* helper_3x3box() is a helper function for analyze_3x3box().
* it gets two start values and loop through the 2D array,
starting from the given starter values
*/
void helper_3x3box(int start_c_1, int start_r_2, UArray2_T \
           uarray2) {
	int cl = 0;
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			int *tmp = (UArray2_at(uarray2, (c + 3*start_c_1),\
                   (r + 3*start_r_2)));
			cl += *tmp;
		}
	}
	if (cl != 45) {
		UArray2_free(&uarray2);
		exit(1);
	}
}