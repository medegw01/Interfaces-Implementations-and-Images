/**************************************************************
 * unblackedges.c                                             *
 * COMP 40  Homework 2                                        *
 * written by: Michael Edegware and Alejandro Vollbrechthausen*
 * Date:       2/13/2017                                      *
 * Purpose:    reads in a portable bitmap and convert the bla-*
 *             ck edge pixels to white                        *
 **************************************************************/

#include "unblackedges.h"

int main(int argc, char * argv[]) {
	FILE * fp = get_file(argc, argv);
	Pnmrdr_T image = get_image(fp);
	Bit2_T bit2 = pbmread(image);
	analyze_image(bit2);
	print_image(bit2);
	Pnmrdr_free( & image);
	Bit2_free( & bit2);
	fclose(fp);
	return 0;
}

/*
* get_file(int argc, char * argv[]) takes file name from main();
* if provided or request a file from standard input and return 
* an opened file(FILE).  It halts with too many input error or
* cannot  open file error
*/
FILE * get_file(int argc, char * argv[]) {
	FILE * fp;
	if (argc > 2) {
		/*prints error if argument is more than one*/
		fprintf(stderr, "%s\n", "Too many arguments.");
		exit(1);
	}

	if (argc == 2) {
		fp = fopen(argv[1], "rb");
		if (fp == NULL) {
			/* Prints error and exits if file cannot be opened */
			fprintf(stderr, "%s: %s %s\n", argv[0], "Could not open", \
			argv[1]);
			exit(1);
		}
	}
	else {
		fp = stdin;
	}
	return fp;
}


/*
* get_image(FILE * fp) returns a pointer to a well formatted and 
* valid image file exit(1) at any instance the image is invalid
* with a  specific erro printout
*/
Pnmrdr_T get_image(FILE * fp) {
	/*creates a pnmrdr readear*/
	Pnmrdr_T image;
	/* check if file is correctly formatted. Since there's 
  Pnmrdr_Badformat exception in Pnmrdr_T, we'll be using 
  TRY AND EXCEPT*/
	TRY
	image = Pnmrdr_new(fp);
	EXCEPT(Pnmrdr_Badformat)
	fprintf(stderr, "Bad format\n");
	fclose(fp);
	exit(1);
	ELSE
	exit(1);
	END_TRY;
  
  /*create a map data for the image*/
	Pnmrdr_mapdata image_data = Pnmrdr_data(image);
	if (image_data.type != Pnmrdr_bit) {
		fprintf(stderr, "Image is not a valid bitmap\n");
		Pnmrdr_free( & image);
		fclose(fp);
		exit(1);
	}
	return image;
}


/*
* pbmread(Pnmrdr_T image) creates a Bit2_T array, reads 
* in every pixel in an image and store it in a 2D array, 
* sequentually, by calling the map_row_major fucntion of
* Bit2_T. It then returns the 2D array of bits
*/ 
Bit2_T pbmread(Pnmrdr_T image){
        Pnmrdr_mapdata imageData = Pnmrdr_data(image);
        int width = imageData.width;
        int height = imageData.height; 
/*create a Bit2_T array and populate it by calling 
map_row_major fucntion*/
        Bit2_T bit2 = Bit2_new(width, height);
        Bit2_map_row_major(bit2, helper_pbmread, &image);
        return bit2;  
}


/*
* helper_pbmread(int col, int row, Bit2_T bit2, int val,
* void *cl) is a helper used by apply() when called in 
*the map_row_major of Bit2_T
*/
void helper_pbmread(int col, int row, Bit2_T bit2, int \
           val, void *cl){
        (void) val;
        Pnmrdr_T image = *(Pnmrdr_T *) cl;
        int bit = Pnmrdr_get(image);
        Bit2_put(bit2, col, row, bit);
}

/*
 * analyze_image(Bit2_T bit2) loops through the 2D array
 * and save  any instance of black edges into a stack. 
 * Then unblacked the  edges.
*/
void analyze_image(Bit2_T bit2){
        /*create a new stack of struct pointer(this struct 
      houses the row and col values) to save the black edged*/
        Stack_T black_edges = Stack_new();
        analyze_row_edges(bit2, black_edges);
        analyze_col_edges(bit2, black_edges);
        analyze_neighboring(bit2,black_edges);
        unblacken(black_edges, bit2);
        Stack_free(&black_edges);
}


/*
* analyze_row_edges(Bit2_T bit2, Stack_T black_edges) is a
* helper function that loops through the row edges and 
* check if there are  black edges
*/
void analyze_row_edges(Bit2_T bit2, Stack_T black_edges) {
	int w = Bit2_width(bit2);
	int h = Bit2_height(bit2);
	for (int c = 0; c < w; c++) {
  /* top row*/
		if (Bit2_get(bit2, c, 0) == BLACK) {
			add_to_stack(c, 0, black_edges);
					}
     /* bottom row*/
		if (Bit2_get(bit2, c, h - 1) == BLACK) {
			 add_to_stack(c, h - 1, black_edges);
				}
	}
}


/*
* analyze_col_edges(Bit2_T bit2, Stack_T black_edges) is
* a helper function that loops through the columns edges
* and check if there are  black edges
*/
void analyze_col_edges(Bit2_T bit2, Stack_T black_edges) {
	int w = Bit2_width(bit2);
	int h = Bit2_height(bit2);
	for (int r = 0; r < h; r++) {
		/*left column*/
		if (Bit2_get(bit2, 0, r) == BLACK) {
			add_to_stack(0, r, black_edges);
		}
		/*right column*/
		if (Bit2_get(bit2, w - 1, r) == BLACK) {
			add_to_stack(w - 1, r, black_edges);
		}
	}
}


/*
* analyze_neighboring(Bit2_T bit2, Stack_T black_edges) is a
* helper function that loops through the inner indexes and
* check if there are  black edges
*/
void analyze_neighboring(Bit2_T bit2, Stack_T black_edges) {
	int w = Bit2_width(bit2);
	int h = Bit2_height(bit2);
	for (int c = 1; c < w - 1; c++) {
		for (int r = 1; r < h - 1; r++) {
			if (Bit2_get(bit2, c, r) == BLACK) {
			helper_analyze_neighboring(c, r, bit2, black_edges);	
			}
		}
	}
}


/*
* helper_analyze_neighboring(int c, int r, Bit2_T bit2, 
* Stack_T  black_edges) checks the left, right, top 
* and bottom neighbours  of a index to see if it 
* qualifies for a black edge. It's called by the 
* analyze_neighboring.
*/
void helper_analyze_neighboring(int c, int r, Bit2_T bit2,\
           Stack_T black_edges){
if ((Bit2_get(bit2, c - 1, r) == BLACK) || (Bit2_get(bit2,\
       c + 1, r) == BLACK) || (Bit2_get(bit2, c, r - 1) ==\
        BLACK)|| (Bit2_get(bit2, c, r + 1) == BLACK)) {
					 add_to_stack(c, r, black_edges);
				}
}

/*
* add_to_stack(int c, int r, Stack_T black_edges) allocates
* memory for a pointer to the struct black_edges_location,
* populate the struct members and save the instance of struct
* to a stack
*/ 
void add_to_stack(int c, int r, Stack_T black_edges){
/*allocate memory for the struct pointer*/
  black_edges_location *bel_ptr = (black_edges_location*) \
  malloc(sizeof(black_edges_location));
  bel_ptr->col = c;
  bel_ptr->row = r;
  /*push it into a struct*/
  Stack_push(black_edges, bel_ptr);
}


/*
* unblacken(Stack_T black_edges, Bit2_T bit2) loops 
* through an instance of stack, pop the struct and put
* white to all black edge coordinates saved in the stack
*/
void unblacken(Stack_T black_edges, Bit2_T bit2) {
	black_edges_location * bel_ptr;
	int col = 0;
	int row = 0;
	/*loop till stack is empty*/
	while (Stack_empty(black_edges) != EMPTY) {
		bel_ptr = (black_edges_location *) Stack_pop(black_edges);
		col = (int) bel_ptr->col;
		row = (int) bel_ptr->row;
		free(bel_ptr);
		Bit2_put(bit2, col, row, WHITE);
	}
}


/*
* print_image(Bit2_T bit2) loops through the 2D array with help
* from Bit2_map_row_major and print the value stored in the 
* indexes
*/
void print_image(Bit2_T bit2) {
	int width = Bit2_width(bit2);
	int height = Bit2_height(bit2);
	printf("P1\n# unblacked.pbm\n%d %d  \n", width, height);
	Bit2_map_row_major(bit2, helper_print_image, &height);
}
/*
* helper_print_image(int col, int row, Bit2_T bit2, int val,
* void *cl) is a helper function for print, it logically determine
* when to print a new line. It's called by print but used by
* apply() int the Bit2_map_row_major during iteration
*/

void helper_print_image(int col, int row, Bit2_T bit2, \
   int val, void *cl) {
 
  (void) col;
  (void) bit2;
  printf("%d", val);
	int *height = (int *) cl;
	if (row == *height - 1)
  printf("\n");
}
