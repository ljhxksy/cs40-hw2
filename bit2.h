/**************************************************************
 *
 *                     bit2.h
 *
 *     Assignment: iii
 *     Authors:  Trin Changkasiri (pchang08) & Anh Hoang (ahoang05)
 *     Date:     02/12/2024
 *
 *     This file contains the function contracts for the implementation
 *     of the Bit2_T data structure.
 *
 **************************************************************/

#include <bit.h>

#ifndef BIT2
#define BIT2

#define T Bit2_T
typedef struct T *T;
struct T
{
        int width;
        int height;
        Bit_T array;
};

/********** Bit2_new ********
 *
 *	 Allocates, initializes, and returns a new 2-dimensional bit vector of
 *   width * height bits
 *
 * 	Parameters:
 *      		int width:	the width of the new bit2
 *      		int height: the height of the new bit2
 *
 * 	Return:     the new bit2 vector
 *
 * 	Expects:
 *      		height >= 0 && width >= 0
 *
 * 	Notes:
 *      		Will CRE if height or width is <= 0
 *      		Will CRE if malloc fails.
 *
 ************************/
extern T Bit2_new(int width, int height);

/********** Bit2_free ********
 *
 *   	Deallocates and clears all elements in the bit2.
 *
 * 	Parameters:
 *		Bit2_T *bit2: The pointer to bit2.
 *
 * 	Return:   none
 *
 * 	Expects:
 *		Both the bit2 and the pointer to bit2 cannot be NULL
 *
 *	Notes:
 *		CRE if bit2 == NULL or *bit2 == NULL
 *		Sets the pointer to bit2 to NULL
 *
 ************************/
extern void Bit2_free(T *bit2);

/********** Bit2_width ********
 *
 *	    Checks for and returns the width of the bit2.
 *
 * 	Parameters:
 *      		Bit2_T bit2: the bit2
 *
 * 	Return: Int holding the width of the bit2.
 *
 * 	Expects:
 *      		The bit2 cannot be NULL
 *
 * 	Notes:
 *      		CRE if bit2 == NULL
 *
 ************************/
extern int Bit2_width(T bit2);

/********** Bit2_height ********
 *
 *	    Checks for and returns the height of the bit2.
 *
 * 	Parameters:
 *      		Bit2_T bit2: the bit2
 *
 * 	Return: Int holding the height of the bit2.
 *
 * 	Expects:
 *      		The bit2 cannot be NULL
 *
 * 	Notes:
 *      		CRE if bit2 == NULL
 *
 ************************/
extern int Bit2_height(T bit2);

/********** Bit2_get ********
 *
 *	    returns the bit at (col,row) and thus tests whether that bit
 *          is in bit2
 *
 * 	Parameters:
 *      		Bit2_T bit2: the bit2
 *              int col:     the column number of the element in the bit2
 *              int row:     the row number of the element in the uarray2
 *
 * 	Return: returns 1 if bit (col, row) in bit2 is 1 and 0 otherwise
 *
 * 	Expects:
 *      		The bit2 cannot be NULL
 *
 * 	Notes:
 *      		CRE if bit2 == NULL
 *
 ************************/
extern int Bit2_get(T bit2, int col, int row);

/********** Bit2_put ********
 *
 *	    Sets bit (col, row) to bit and returns the previous value of that 
 *          bit
 *
 * 	Parameters:
 *      	Bit2_T bit2: the bit2
 *              int col:     the column number of the element in the bit2
 *              int row:     the row number of the element in the bit2
 *
 * 	Return:     the previous value of the bit
 *
 * 	Expects:
 *      		The bit2 cannot be NULL
 *              Valid col, row coordinate that exists in bit2
 *              The bit has to be either 0 or 1
 *
 * 	Notes:
 *      		CRE if bit2 == NULL
 *              CRE if the position (col, row) is out of bounds
 *              CRE if the bit != 0 || bit != 1
 *
 ************************/
extern int Bit2_put(T bit2, int col, int row, int bit);

/**********Bit2_map_row_major ********
 *
 *	    Applies the apply() function to every value in the bit2 in
 *      row-major order.
 *
 * 	Parameters:
 *      		Bit2_T bit2: the bit2
 *		        void apply: the apply function, which applies to
 *                          every element in the bit2
 *		        void *cl: application-specific pointer
 *
 * 	Return: none
 *
 * 	Expects:
 *      		Valid apply function as well as a pre-initialized bit2
 *		        NULL can be passed in instead of an application-specific
 *                      pointer
 * 	Notes:
 *		        Apply function takes arguments: int width, int height,
 *                                      Bit2_T bit2, int bit, void *p1
 *      		Arguments can be set as void if necessary
 *		        CRE if bit2 == NULL
 *
 ************************/
extern void Bit2_map_row_major(T bit2, void apply(int i, int j, Bit2_T bit2, 
                               int bit, void *p1), void *cl);

/**********Bit2_map_col_major ********
 *
 *	    Applies the apply() function to every value in the bit2 in
 *      col-major order.
 *
 * 	Parameters:
 *      		Bit2_T bit2: the bit2
 *		        void apply: the apply function, which applies to
 *                          every element in the bit2
 *		        void *cl: application-specific pointer
 *
 * 	Return: none
 *
 * 	Expects:
 *      		Valid apply function as well as a pre-initialized bit2
 *		        NULL can be passed in instead of an application-specific
 *                      pointer
 * 	Notes:
 *		        Apply function takes arguments: int width, int height,
 *                                      Bit2_T bit2, int bit, void *p1
 *      		Arguments can be set as void if necessary
 *		        CRE if bit2 == NULL
 *
 ************************/
extern void Bit2_map_col_major(T bit2, void apply(int width, int height, 
                                Bit2_T bit2, int bit, void *p1), void *cl);
                                
#undef T
#endif