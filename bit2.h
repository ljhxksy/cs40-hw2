#ifndef BIT2
#define BIT2

#define T BIT2_T
typedef struct T *T;
struct T {
    int width;
    int height;
    Bit_T array;
};

/********** Bit2_new ********
 *
 *	 Allocates, initializes, and returns a new 2-dimensional bit vector of
 *   width * height bits
 * 	Parameters:
 *      		int width:	the width of the new bit2
 *      		int height: the height of the new bit2
 *
 * 	Return:     the new bit2 vector
 *
 * 	Expects:
 *      		height > 0 && width > 0
 *
 * 	Notes:	
 *      		Will CRE if height or width is <= 0
 *      		Will CRE if malloc fails.
 *
 ************************/
extern T Bit2_new (int width, int height);

/********** Bit2_free ********
* 
*   	Deallocates and clears all elements in the bit2.
*
* 	Parameters: 	
*		UArray2_T *bit2: The pointer to bit2.
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


extern int Bit2_width(T bit2);
extern int Bit2_height(T bit2);
extern int Bit2_get(T bit2, int col, int row);
extern int Bit2_put(T bit2, int col, int row, int bit);
extern void Bit2_map_row_major (T bit2, void apply(int i, int j,  
                                   Bit2_T bit2, int bit, void *p2), 
                                   void *cl);
extern void Bit2_map_col_major (T bit2, void apply(int width, int height,  
                                   Bit2_T bit2, int bit, void *p2), 
                                   void *cl);

#undef T
#endif