#include "uarray.h"

#ifndef UARRAY2
#define UARRAY2

#define T UArray2_T
typedef struct T *T;
struct T {
    int width;
    int height;
    int size;
    UArray_T array;
};

extern T UArray2_new (int width, int height, int size);
extern void UArray2_free(T *uarray2);
extern int UArray2_width(T uarray2);
extern int UArray2_height(T uarray2);
extern int UArray2_size (T uarray2);
void *UArray2_at(T uarray2, int col, int row);
extern void UArray2_map_row_major (T uarray2, void apply(int i, 
                                    int j, UArray2_T uarray2, void *p1, void *p2), void *cl);
extern void UArray2_map_col_major (T uarray2, void apply(int i, 
                                    int j, UArray2_T uarray2, void *p1, void *p2), void *cl);

#undef T
#endif