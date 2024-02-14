/**********************************************************
Homework 2: iii
CS40
sudoku.c
Author: Trin Changkasiri (pchang08) & Anh Hoang (ahoang05) 

Sudoku.c reads in a 9x9 pgm file containing a sudoku grid, and either exits 
success or failure based on the validity of the sudoku grid.

*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pnmrdr.h>
#include <except.h>
#include <stdbool.h>
#include "uarray2.h"

UArray2_T populateUArray2(FILE *fp);
void apply(int i, int j, UArray2_T uarray2, void *p1, void *p2);
extern void Grid_map_subsquare(UArray2_T uarray2, void apply(int i, int j, 
                               UArray2_T uarray2, void *p1, void *p2), void *cl,
                               int col, int row);
void checkSudoku(UArray2_T *uarray2);

Except_T Checked_Runtime = {"Checked Runtime Error"};

int main(int argc, char *argv[])
{
        FILE *fp = NULL;
        /* Open file, checks for valid arguments */
        if (argc == 1) {
                fp = stdin;
        }
        else if (argc == 2) {
                fp = fopen(argv[1], "rb");
                assert(fp);
        }
        else {
                RAISE(Checked_Runtime);
        }

        /* Initializes uarray2 */
        UArray2_T uarray2 = populateUArray2(fp);

        checkSudoku(&uarray2);

        UArray2_free(&uarray2);
        exit(EXIT_SUCCESS);
        return 0;
}

/********** checkSudoku  ********
 *
 *	Keeps track of digit frequencies via 9-integer array. Calls 
 *      UArray2_map_row_major to check rows for repeating digits, 
 *      UArray2_map_col_major to check columns, and 
 *      Grid_map_subsquare to check individual grids.
 * 	
 * 	Parameters:
 *      	UArray2_T *uarray2: populated uarray2 containing sudoku grid.
 *
 * 	Return:	
 *              N/A (void function)
 *
 * 	Expects:
 *      	uarray2 != NULL, apply != NULL.
 *
 * 	Notes:	
 *      	Will CRE if uarray2 is NULL.
 *      	Will CRE if apply is NULL.
 *      		
 *
 ************************/
void checkSudoku(UArray2_T *uarray2)
{
        /* create an array of 9 elements */
        int arr[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

        UArray2_map_row_major(*uarray2, apply, &arr);
        UArray2_map_col_major(*uarray2, apply, &arr);

        for (int i = 0; i < 9; i += 3) {
                for (int j = 0; j < 9; j += 3) {
                        Grid_map_subsquare(*uarray2, apply, &arr, i, j);
                }
        }
}

/********** populateUArray2 ********
 *
 *	 Utilizes Pnmrdr functions to read from provided file and populate 
 *       uarray2 with sudoku grid, closes file before returning.
 * 
 * 	Parameters:
 *      	FILE *fp: file pointer to be accessed by Pnmrdr_get.
 *
 * 	Return:     
 *              populated uarray2
 *
 * 	Expects:
 *              the pnm file has the correct data type
 *      	height == 9 && width ==  9, all values in the uarray2 after 
 *              populating must be > 0 and <= 9.
 *
 * 	Notes:	
 *      	Will CRE if height or width is < 9
 *      	Will CRE if any values in uarray are <= 0 or > 9
 *
 ************************/
UArray2_T populateUArray2(FILE *fp)
{
        Pnmrdr_T plain = Pnmrdr_new(fp);

        /* Checks for valid file type and map dimensions(9x9) */
        Pnmrdr_mapdata data = Pnmrdr_data(plain);
        assert(data.type == 2 || data.type == 5);
        assert(data.width == 9 && data.height == 9);

        UArray2_T newUArr2 = UArray2_new(data.width, data.height, sizeof(int));

        for (int j = 0; j < (int)data.height; j++) {
                for (int i = 0; i < (int)data.width; i++) {
                        *((int *)UArray2_at(newUArr2, i, j)) = 
                                                        Pnmrdr_get(plain);
                        assert(*((int *)UArray2_at(newUArr2, i, j)) > 0);
                        assert(*((int *)UArray2_at(newUArr2, i, j)) <= 9);
                }
        }

        Pnmrdr_free(&plain);
        fclose(fp);

        return newUArr2;
}

/********** apply ********
 *
 *	 Checks the frequency array initialized in checkSudoku for any 
 *       duplicates, resets array to all 0s once 9 elements have been checked.
 * 
 * 	Parameters:
 *      	int i:	current column index
 *      	int j: 	current row index
 *		UArray2_T uarray2: populated uarray2
 *		void *p1: Application-specific pointer (voided in function)
 *		void *p2: Pointer to frequency array
 *
 * 	Return:     
 *              N/A (void function)
 *
 * 	Expects:
 *      	uarray2 != NULL, p2 != NULL.
 *
 * 	Notes:	
 *      	Will CRE if uarray2 is NULL.
 *      	Will CRE if apply is NULL.
 *
 ************************/
void apply(int i, int j, UArray2_T uarray2, void *p1, void *p2)
{
        (void)p1;
        int *arr = (int *)p2;
        int curr = *((int *)(UArray2_at(uarray2, i, j))) - 1;
        arr[curr]++;

        /* If the element count is greater than 1, then the board is not a 
         * valid sudoku */
        if (arr[curr] > 1) {
                UArray2_free(&uarray2);
                exit(EXIT_FAILURE);
        }

        int count = 0;
        bool check1 = true;

        /* Checks for repeating integers by regulating frequency count */
        for (i = 0; i < 9; i++) {
                if (arr[i] == 1) {
                        count++;
                }
                if (arr[i] > 1) {
                        check1 = false;
                }
        }

        /* Resets all digit frequencies to 0 once full */
        if (check1 == false || count == 9) {
                for (int i = 0; i < 9; i++) {
                        arr[i] = 0;
                }
        }
}

/********** Grid_map_subsquare ********
 *
 *	 Loops through each sub-grid (3x3 square) to check for duplicate 
 *       numbers by manually calling the apply function.
 * 
 * 	Parameters:
 *      	UArray2_T uarray2: populated uarray2
 *      	void apply: apply function
 *		void *cl: Pointer to frequency array
 * 		int col: current column index
 *		int row: current row index
 *
 * 	Return:     
 *              N/A (void function)
 *
 * 	Expects:
 *      	uarray2 != NULL, apply != NULL.
 *
 * 	Notes:	
 *      	Will CRE if uarray2 is NULL.
 *      	Will CRE if apply is NULL.
 *
 ************************/
void Grid_map_subsquare(UArray2_T uarray2, void apply(int i, int j, UArray2_T 
                        uarray2, void *p1, void *p2), void *cl, int col, 
                        int row)
{
        assert(uarray2);
        assert(apply);

        int rowRange = row + 3;
        int colRange = col + 3;

        for (int j = row; j < rowRange; j++) {
                for (int i = col; i < colRange; i++) {
                        apply(i, j, uarray2, UArray2_at(uarray2, i, j), cl);
                }
        }
}