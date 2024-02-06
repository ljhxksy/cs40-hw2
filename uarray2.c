#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "uarray.h"
#include "uarray2.h"

#define T UArray2_T

/********** UArray2_new ********
 *
 *	 Allocates, initializes, and returns a new 2-dimensional  array of 
 *   (height*width) elements with bounds zero through (height − 1)(width - 1).
 *
 * 	Parameters:
 *      		int width:	the width of the new uarray2
 *      		int height: the height of the new uarray 2
 *      		int size:   the size of each element in the uarray2
 *
 * 	Return:     the new uarray2
 *
 * 	Expects:
 *      		height > 0 && width > 0 && size > 0 
 *
 * 	Notes:	
 *      		Will CRE if height, width or size is <= 0
 *      		Will CRE if malloc fails.
 *
 ************************/

extern T UArray2_new (int width, int height, int size) {
    //TODO: Add a bunch of asserts
    assert(height > 0 && width > 0 && size > 0);

    UArray_T uarray = UArray_new(width*height, size);
    T newUArr2 = malloc(sizeof(struct T));
    assert(newUArr2);

    newUArr2->height = height;
    newUArr2->width = width;
    newUArr2->size = size;
    newUArr2->array = uarray;

    return newUArr2;
}

/********** UArray2_free ********
* 
*   	Deallocates and clears all elements in the uarray2.
*
* 	Parameters: 	
*		UArray2_T *uarray2: The pointer to uarray2.
*
* 	Return:   none	
*
* 	Expects:
*		Both the uarray2 and the pointer to uarray2 cannot be NULL
*
*	Notes:
*		CRE if uarray2 == NULL or *uarray2 == NULL
*		Sets the pointer to uarray2 to NULL
*
 ************************/

extern void UArray2_free(T *uarray2) {
    assert(*uarray2 && uarray2);

    UArray_free(&(*uarray2)->array);
    free(*uarray2);
}

/********** UArray2_width ********
 *
 *	    Checks for and returns the width of the uarray2.
 *
 * 	Parameters:
 *      		UArray2_T uarray2: the uarray2 
 *
 * 	Return: Int holding the width of the uarray2.
 *
 * 	Expects:
 *      		The uarray2 cannot be NULL
 *
 * 	Notes:
 *      		CRE if uarray2 == NULL
 *
 ************************/

extern int UArray2_width(T uarray2) {
    assert(uarray2);
    return uarray2->width;
}

/********** UArray2_height ********
 *
 *	    Checks for and returns the height of the uarray2.
 *
 * 	Parameters:
 *      		UArray2_T uarray2: the uarray2 
 *
 * 	Return: Int holding the height of the uarray2
 *
 * 	Expects:
 *      		The uarray2 cannot be NULL
 *
 * 	Notes:
 *      		CRE if uarray2 == NULL
 *
 ************************/

extern int UArray2_height(T uarray2) {
    assert(uarray2);
    return uarray2->height;
}

/********** UArray2_size ********
 *
 *	    Provides the size of each element in the uarray2 in bytes
 *
 * 	Parameters:
 *      		UArray2_T uarray2: the uarray2 
 *
 * 	Return:     Int containing the size of an element in the uarray2 in bytes
 *
 * 	Expects:    The uarray2 cannot be NULL
 *
 * 	Notes:      CRE if uarray2 == NULL
 *
 ************************/

extern int UArray2_size (T uarray2) {
    assert(uarray2);
    return uarray2->size;
}

/********** UArray2_at ********
 *
 *	    Finds the element at the (col, row) position of the uarray2
 *
 * 	Parameters:
 *      		UArray2_T uarray2: the uarray2 
 *		        int col: the column number of the element in the uarray2
 *		        int row: the row number of the element in the uarray2
 *
 * 	Return:     A pointer to the desired element 
 *
 * 	Expects:
 *      		The uarray2 cannot be NULL
 *		        Valid col, row coordinate that exists in uarray2
 * 	Notes:
 *      		CRE if uarray2 == NULL
 *		        CRE if the position (col, row) is out of bounds
 *
 ************************/

void *UArray2_at(T uarray2, int col, int row) {
    assert(uarray2);

    printf("//////// col, row: %d, %d /////", col, row);
    assert(0 <= col && col < uarray2->width);
    assert(0 <= row && row < uarray2->height);

    //assert out of bounds
    void *kiki = UArray_at(uarray2->array, uarray2->width * row + col);
    // uarray2->width * col + row;
    return kiki;
}

/**********UArray2_map_row_major ********
 *
 *	    Applies an apply() function of choice to every value in the uarray2 in 
 *      row-major order.
 *
 * 	Parameters:
 *      		UArray2_T uarray2: the uarray2 
 *		        void apply: the apply function, which applies to 
 *                          every element in the uarray2
 *		        void *cl: application-specific pointer
 *
 * 	Return: none
 *
 * 	Expects:
 *      		Valid apply function as well as a pre-initialized uarray2
 *		NULL can be passed in instead of an application-specific pointer
 * 	Notes:
 *		Apply function takes arguments: int width, int height,  UArray2_T uarray2, 
 *						    void *p1, void *p2
 *      		Arguments can be set as void if necessary
 *		CRE if uarray2 == NULL
 *
 ************************/

extern void UArray2_map_row_major (T uarray2, void apply(int i, 
                                   int j, UArray2_T uarray2, void *p1, 
                                   void *p2), void *cl) {
        assert(uarray2);

        void *p1 = NULL;
        for (int i = 0; i < uarray2->width; i++) {
            for (int j = 0; j < uarray2->height; j++) {
                apply(j, i, uarray2, p1, cl);
            }
        }
        // apply();
}
/**********UArray2_map_col_major ********
 *
 *	Applies an apply() function of choice to every value in the uarray2 in column-major order.
 *
 * 	Parameters:
 *      		UArray2_T uarray2: the uarray2 
 *		void apply: the apply function, which applies to every element in the uarray2
 *		void *cl: application-specific pointer
 *
 * 	Return: none
 *
 * 	Expects: 
 *      		Valid apply function as well as a pre-initialized uarray2
 *		NULL can be passed in instead of an application-specific pointer
 * 	Notes: 
 *		Apply function takes arguments: int width, int height,  UArray2_T uarray2, 
 *						    void *p1, void *p2
 *      		Arguments can be set as void if necessary
 *		CRE if uarray2 == NULL
 *
 ************************/

extern void UArray2_map_col_major (T uarray2, void apply(int i, 
                                    int j, UArray2_T uarray2, void *p1, 
                                    void *p2), void *cl) {

        void *p1 = NULL;
        for (int i = 0; i < uarray2->height; i++) {
            for (int j = 0; j < uarray2->width; j++) {
                apply(i, j, uarray2, p1, cl);
            }
        }
}