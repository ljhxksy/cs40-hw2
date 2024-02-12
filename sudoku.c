#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pnmrdr.h>
#include <except.h>
#include <stdbool.h>
#include "uarray2.h"

UArray2_T populateUArray2(FILE *fp);
void apply(int i, int j, UArray2_T uarray2, void *p1, void *p2);
extern void Grid_map_subsquare(UArray2_T uarray2, void apply(int i, 
                                   int j, UArray2_T uarray2, void *p1, 
                                   void *p2), void *cl, int col, int row);
void checkSudoku(UArray2_T *uarray2);

Except_T Checked_Runtime = { "Checked Runtime Error" };

int main(int argc, char *argv[]) 
{
    FILE *fp = NULL;

    if (argc == 1) {
        fp = stdin;
    } else if (argc == 2) {
        fp = fopen(argv[1], "rb");
        assert(fp);
    } else {
        RAISE(Checked_Runtime);
    }

    //TODO: File(s) ['stderr'] is (are) empty (1)

    UArray2_T uarray2 = populateUArray2(fp);
    
    checkSudoku(&uarray2);

    UArray2_free(&uarray2);
    printf("EXIT SUCCESS\n");
    exit(EXIT_SUCCESS);
    return 0;
}

void checkSudoku(UArray2_T *uarray2) {
    int arr[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    UArray2_map_row_major(*uarray2, apply, &arr);
    UArray2_map_col_major(*uarray2, apply, &arr);
    
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            Grid_map_subsquare(*uarray2, apply, &arr, i, j);
        }
    }
}

UArray2_T populateUArray2(FILE *fp) 
{
    Pnmrdr_T plain = Pnmrdr_new(fp);

    Pnmrdr_mapdata data = Pnmrdr_data(plain);
    assert(data.type == 2 || data.type == 5);
    assert(data.width == 9 && data.height == 9);

    UArray2_T newUArr2 = UArray2_new(data.width, data.height, sizeof(int));

    for (int j = 0; j < (int)data.height; j++) {
        for (int i = 0; i < (int)data.width; i++) {
            *((int *)UArray2_at(newUArr2, i, j)) = Pnmrdr_get(plain);
            assert(*((int *)UArray2_at(newUArr2, i, j)) > 0);
            assert(*((int *)UArray2_at(newUArr2, i, j)) <= 9);
        }
    }

    Pnmrdr_free(&plain);
    fclose(fp);

    return newUArr2;
}

void apply(int i, int j, UArray2_T uarray2, void *p1, void *p2) 
{ 
    (void)p1;  
    int* arr = (int*)p2;
    int curr = *((int*)(UArray2_at(uarray2, i, j))) - 1;
    arr[curr]++;

    if (arr[curr] > 1) {
        UArray2_free(&uarray2);
        printf("EXIT FAILURE\n");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    bool check1 = true;

    for (i = 0; i < 9; i++) {
        if (arr[i] == 1) count++;
        if (arr[i] > 1) check1 = false;
    }
    
    if (check1 == false || count == 9) {
        for (int i = 0; i < 9; i++) {
            arr[i] = 0;
        }   
    }  
}

void Grid_map_subsquare(UArray2_T uarray2, void apply(int i, 
                                   int j, UArray2_T uarray2, void *p1, 
                                   void *p2), void *cl, int col, int row) 
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