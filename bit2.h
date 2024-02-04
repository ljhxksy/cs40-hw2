#include <stdio.h>
#include <stdlib.h>
#include "uarray.h"

#ifndef BIT2
#define BIT2

#define T BIT2_T
typedef struct T *T;
struct T {
    int width;
    int height;
    char *elems; // TODO: wtf
};

extern T Bit2_new (int width, int height);
extern void Bit2_free(T *bit2);
extern int Bit2_width(T bit2);
extern int Bit2_height(T bit2);
extern int Bit2_get(T bit2, int width, int height);
extern int Bit2_put(T bit2, int width, int height, int bit);
extern void Bit2_map_row_major (T bit2, void apply(int width, int height,  
                                   Bit2_T bit2, int bit, void *p2), 
                                   void *cl);
extern void Bit2_map_col_major (T bit2, void apply(int width, int height,  
                                   Bit2_T bit2, int bit, void *p2), 
                                   void *cl);

#undef T
#endif