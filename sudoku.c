#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pnmrdr.h>
#include <except.h>
#include <stdbool.h>
#include "uarray2.h"

UArray2_T populateUArray2(FILE *fp);
void apply(int i, int j, UArray2_T uarray2, void *p1, void *p2);

// TODO: is assert() equivalent to raising CREs
Except_T Checked_Runtime = { "Checked Runtime Error" };

int main(int argc, char *argv[]) {
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

    printf("uarray2's width: %d, height: %d\n", UArray2_width(uarray2), UArray2_height(uarray2));

    int arr[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    UArray2_map_row_major(uarray2, apply, &arr);
    UArray2_map_col_major(uarray2, apply, &arr);

    //break when false --> not sudoku

    UArray2_free(&uarray2);
    return 0;
}

UArray2_T populateUArray2(FILE *fp) {
    Pnmrdr_T plain = Pnmrdr_new(fp);

    Pnmrdr_mapdata data = Pnmrdr_data(plain);
    assert(data.type == 2 && data.width == 9 && data.height == 9);

    UArray2_T newUArr2 = UArray2_new(data.width, data.height, sizeof(int));

    for (int j = 0; j < (int)data.height; j++) {
        for (int i = 0; i < (int)data.width; i++){
            *((int *)UArray2_at(newUArr2, i, j)) = Pnmrdr_get(plain);
            printf ("%d", *(int *)UArray2_at(newUArr2, i, j)); //DELETE LATER
        }
        printf("\n"); //DELETE LATER
    }
    printf("\n"); //DELETE LATER

    Pnmrdr_free (&plain);
    fclose(fp);

    return newUArr2;
}

void apply(int i, int j, UArray2_T uarray2, void *p1, void *p2) { 
    (void)p1;  
    int* arr = (int*)p2;
    int curr = *((int*)(UArray2_at(uarray2, i, j))) - 1;
    arr[curr]++;

    for (int i = 0; i < 9; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // ++ arr[UArray2_at(uarray2, i, j) - 1];

    if (arr[curr] > 1)
    {
        printf("fuck cs40\n");
    }

    int count = 0;
    bool check1 = true;
    for (i = 0; i < 9; i++)
    {
        if (arr[i] == 1) count++;
        if (arr[i] > 1) check1 = false;
    }
    
    if (check1 == false || count == 9)
    {
        printf("check\n");
        for (int i = 0; i < 9; i++) {
            arr[i] = 0;
        }
    }  
}



