/**************************************************************
 *
 *                     uarray2.c
 *
 *     Assignment: iii
 *     Authors:  Trin Changkasiri (pchang08) & Anh Hoang (ahoang05)
 *     Date:     02/12/2024
 *
 *     This file contains the implementation of the UArray2 data structure.
 *     This data structure is a 2D unboxed array, which relies 
 *     on the Hanson uarray.h interface.
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "uarray.h"
#include "uarray2.h"

#define T UArray2_T

extern T UArray2_new(int width, int height, int size)
{
        assert(height >= 0 && width >= 0 && size > 0);

        UArray_T uarray = UArray_new(width * height, size);
        T newUArr2 = malloc(sizeof(struct T));
        assert(newUArr2);

        newUArr2->height = height;
        newUArr2->width = width;
        newUArr2->size = size;
        newUArr2->array = uarray;

        return newUArr2;
}

extern void UArray2_free(T *uarray2)
{
        assert(*uarray2 && uarray2);

        UArray_free(&(*uarray2)->array);
        free(*uarray2);
}

extern int UArray2_width(T uarray2)
{
        assert(uarray2);
        return uarray2->width;
}

extern int UArray2_height(T uarray2)
{
        assert(uarray2);
        return uarray2->height;
}

extern int UArray2_size(T uarray2)
{
        assert(uarray2);
        return uarray2->size;
}

void *UArray2_at(T uarray2, int col, int row)
{
        assert(uarray2);
        assert(col >= 0 && col < UArray2_width(uarray2));
        assert(row >= 0 && row < UArray2_height(uarray2));

        void *elem = UArray_at(uarray2->array, 
                               UArray2_width(uarray2) * row + col);
        assert(elem);

        return elem;
}

extern void UArray2_map_row_major(T uarray2, void apply(int i, int j, 
                        UArray2_T uarray2, void *p1, void *p2), void *cl)
{
        assert(uarray2);
        assert(apply);

        for (int j = 0; j < UArray2_height(uarray2); j++)
        {
                for (int i = 0; i < UArray2_width(uarray2); i++)
                {
                        apply(i, j, uarray2, UArray2_at(uarray2, i, j), cl);
                }
        }
}

extern void UArray2_map_col_major(T uarray2, void apply(int i, int j, 
                        UArray2_T uarray2, void *p1, void *p2), void *cl)
{
        assert(uarray2 && apply);

        for (int i = 0; i < UArray2_width(uarray2); i++) {
                for (int j = 0; j < UArray2_height(uarray2); j++) {
                        apply(i, j, uarray2, UArray2_at(uarray2, i, j), cl);
                }
        }
}