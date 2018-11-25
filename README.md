# Interfaces-Implementations-and-Images
### Purpose
This assignment has three goals:
1. To give you practice designing your own interfaces, not just using interfaces designed by other people
2. To give you practice thinking about what familiar techniques you can use to solve new problems
3. To lay a foundation for future assignments. In these future assignments,
   - You will learn about locality, its effects on performance, and how to change the locality of a program.
   - You will understand how data structures in a high-level language map to machine structures, and how to improve space performance by programming directly with machine structures.
   - You will learn to improve the performance of programs by code tuning

The abstractions you build in this assignment will help you represent and manipulate digital images.
### Preliminaries
In Hanson's C interfaces and implementations, refresh your memory about exceptions (Section 4.1) and memory management (Section 5.1), and study the Array and Bit abstractions defined in Chapters 10 and 13.
### Part A: Two-Dimensional Arrays of Anything
In Chapter 10, Dave Hanson provides an abstraction that implements one-dimensional arrays. For this part of the assignment, you'll adapt Hanson's abstraction to support two-dimensional arrays. Your adaptation will be called Array2 and should define the type Array2_T. Your adaptation should include the following changes:
- Instead of a length, an Array2_T will have a width and a height.
- Instead of being identified by a single index, an element of an Array2_T will be identified by two indices: the column or x index measures the distance between the element and the left edge of the array, while the row or y index measures the distance between the element and the top row of the array. Thus the top left element is always indexed by the pair (0,0).
- Omit the resize and copy operations.
- You may elect, if you wish, to define an Array2_loc operation, to be discussed in class, instead of the pair Array2_get and Array2_put.
- You must define two analogs of the Bit_map function described on page 201:
  -  *Array2_map_row_major* calls an apply function for each element in the array. Column indices vary more rapidly than row indices.
  - *Array2_map_col_major* calls an apply function for each element in the array. Row indices vary more rapidly than column indices.
The terms "Row major" and "column major" may be found in Bryant and O'Halloran as well as on Wikipedia.
As in Hanson's code, a reference out of bounds should result in a checked run-time error.
For part A, the problem you are to solve is define an interface and build an implementation for Array2. 
My solution to this problem takes about 100 lines of C code.

### Part B: Two-Dimensional Arrays of Bits
In some cases, particularly for documents scanned at high resolution, it can be useful to represent an image as an array of bits. Each bit is either black (1) or white (0). To save space, it is useful to have a packed representation of such images. For this part of the lab, you'll design Bits2: an interface to support two-dimensional arrays of bits.

### Part C: Using the Array2 abstraction to identify Sudoku solutions
Write the test program sudoku. It takes as input a single portable graymap file, which may be named on the command line or may be given on standard input. Your program should not print anything, but if the graymap file represents a solved sudoku puzzle, your program should call exit(0); otherwise it should call exit(1). A solved sudoku puzzle is a nine-by-nine graymap with these properties:
The maximum pixel intensity is nine.
No pixel has zero intensity.
In each row, no two pixels have the same intensity.
In each column, no two pixels have the same intensity.
If the nine-by-nine graymap is divided into nine three-by-three submaps (like a tic-tac-toe board), in each three-by-three submap, no two pixels have the same intensity.
Here's an example (which you can also view as an image):
P2
9 9
portable graymap representing a sudoku solution
9 
1 2 3   4 5 6   7 8 9
4 5 6   7 8 9   1 2 3
7 8 9   1 2 3   4 5 6

2 3 4   5 6 7   8 9 1 
5 6 7   8 9 1   2 3 4 
8 9 1   2 3 4   5 6 7 

3 4 5   6 7 8   9 1 2
6 7 8   9 1 2   3 4 5
9 1 2   3 4 5   6 7 8
My solution to this problem takes about 120 lines of C code. There is a significant opportunity for abstraction; a Very Good solution will identify such opportunities and use them to avoid repeating code.
If sudoku is used in a way that violates its specification, it should terminate with a checked run-time error (any one will do). Read the specification carefully!

Part D: Using the Bit2 abstraction to remove black edges
Write the test program unblackedges, which removes black edges from a scanned image. Example:
![] (bf.PNG )
![] (af.PNG )

The program unblackedges takes at most one argument:

If an argument is given, it should be the name of a portable bitmap file (in pbm format).
If no argument is given, unblackedges reads from standard input, which should contain a portable bitmap.
If more than one argument is given, unblackedges halts with an error message.
If a portable bitmap is promised but not delivered, unblackedges should halt with some sort of error message on stderr.
Program unblackedges should print, on standard output, a portable bitmap file which is identical to the original file except that all black edge pixels should changed to white. You can find some sample images in /comp/40/images/bitonal; try, for example,
  pngtopnm /comp/40/images/bitonal/hyphen.png | ./unblackedges | display -
For a bitmap of size w by h, a black edge pixel is defined inductively as follows:

A pixel is a black edge pixel if it is black and if it appears in column 0, in row 0, in column w-1, or in row h-1.
A pixel that appears in column i, row j is a black edge pixel if it is black, if i>0 and i<w-1, if j>0 and j<h-1, and any neighboring pixel is a black edge pixel.
The neighboring pixels of the pixel in column i, row j are
The pixel in column i-1, row j
The pixel in column i+1, row j
The pixel in column i, row j-1
The pixel in column i, row j+1

## Implementation:

Alejandro Vollbrechthausen & Michael
Proj0 Comp 40 
02/2/2017

#### Files                                     

uarray2.c bit2.c sudoku.c unblackedges.c


#### Purposes                                   

* Part A -> uarray2
the purpose of this function is to create a usable two dimentional array. this 
will be implimented in other files to allow for creating grids to hold info on
specific tasks or to simply hold double arrays.

* Part B -> bit2
the purpose of this is to create a double array of bits that can be used to 
represent greyscale images by creating a grid using ones and zeros. this can 
then be used to manipulate several images and create them. 

* Part C -> sudoku
 the purpose of this file is to create a program that checks to see if a sudoku
puzzle is solved correctly. that is it is a 9 by 9 grid, max intesnty of 9, min
of 1 in each row and column no to pixels have the same intensity, and there are
3 by 3 submaps with no pixels of the same intensity. 

* Part D -> unblackedges
the purpose of this file is to create a program that takes in an image then it
goes through the pixelage of the image finding the black edges and turning them
into white edges. then returning the same image with the same dimesntions but
with no black edges.

#### Compiling                                  
We where provided a make file that compiles all the programs 

#### Data Structures and Algorithms 
* Part A -> uarray2 
for this program we used a uarray and filled it with pointers to other arrays. 
this created the ability to create a doubel array of multiple dimentions by 
changging the width and height and or colomn size or rows. 

* Part B -> bit2
for this program we ussed a uarray and filled it with pointers to bit arrays
similar to part a this allows for a double array of bits to form that can be 
manipulated by height and width or row and column size.

* Part C -> sudoku
for this program we created a double array that takes int the sudoku and checks
to see if it is solved. it does this by creating a double array and placing in 
the then it loops through to see if any row or colunm has the same number then
sees if any sub grid of three by three doesn not have any of the same numbers
then it declairs it as solved and exits with 0 and if it doesnt it exits with 1

* Part D -> unblackedges
for this program we created a double array of bits that is made of an image 
imported. them we move thought the outside crid of the image and locating black
edges and turning them to white but marking how much of the exposed edges are
black bits as well. after it is done it then returns the image from the double
bit array to a an image with out black edges.
