/**************************************************************
 *
 *                     bit2.c
 *
 *     Assignment: iii
 *     Authors:  Trin Changkasiri (pchang08) & Anh Hoang (ahoang05)
 *     Date:     02/12/2024
 *
 *     This file contains the implementation of the Bit2 data structure.
 *     This data structure is a 2D (a.k.a matrix) of bits, which relies 
 *     on the Hanson bit.h interface.
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bit.h"
#include "bit2.h"

#define T Bit2_T

extern T Bit2_new(int width, int height)
{
        assert(width > 0 && height > 0);

        Bit_T bit = Bit_new(width * height);
        T newBit2 = malloc(sizeof(struct T));
        assert(newBit2);

        newBit2->height = height;
        newBit2->width = width;
        newBit2->array = bit;

        return newBit2;
}

extern void Bit2_free(T *bit2)
{
        assert(*bit2 && bit2);

        Bit_free(&(*bit2)->array);
        free(*bit2);
}

extern int Bit2_width(T bit2)
{
        assert(bit2);
        return bit2->width;
}

extern int Bit2_height(T bit2)
{
        assert(bit2);
        return bit2->height;
}

extern int Bit2_get(T bit2, int col, int row)
{
        assert(bit2);

        int elem = Bit_get(bit2->array, Bit2_width(bit2) * row + col);

        return elem;
}

extern int Bit2_put(T bit2, int col, int row, int bit)
{
        assert(bit2);

        int prev = Bit2_get(bit2, col, row);

        Bit_put(bit2->array, Bit2_width(bit2) * row + col, bit);

        return prev;
}

extern void Bit2_map_row_major(T bit2, void apply(int i, int j, 
                                Bit2_T bit2, int bit, void *p1), void *cl)
{
        assert(bit2);
        assert(apply);

        for (int j = 0; j < Bit2_height(bit2); j++)
        {
                for (int i = 0; i < Bit2_width(bit2); i++)
                {
                        apply(i, j, bit2, Bit2_get(bit2, i, j), cl);
                }
        }
}

extern void Bit2_map_col_major(T bit2, void apply(int width, int height, 
                                Bit2_T bit2, int bit, void *p1),void *cl)
{
        assert(bit2);
        assert(apply);

        for (int i = 0; i < Bit2_width(bit2); i++) {
                for (int j = 0; j < Bit2_height(bit2); j++) {
                        apply(i, j, bit2, Bit2_get(bit2, i, j), cl);
                }
        }
}