#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pnmrdr.h>
#include <except.h>

#include "uarray2.h"

UArray2_T populateUArray2(FILE *fp);

// TODO: is assert() equivalent to raising CREs
Except_T Checked_Runtime = { "Checked Runtime Error" };

int main(int argc, char *argv[]) {
    FILE *fp = NULL;

    if (argc == 1) {
        fp = stdin;
    } else if (argc == 2) {
        // open file
        fp = fopen(argv[1], "rb");
        assert(fp);
    } else {
        RAISE(Checked_Runtime);
    }

    //TODO: File(s) ['stderr'] is (are) empty (1)

    UArray2_T uarray2 = populateUArray2(fp);


    printf("uarray2's width: %d, height: %d\n", UArray2_width(uarray2), UArray2_height(uarray2));



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


