#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pnmrdr.h>
#include "uarray2.h"

int main(int argc, char *argv[]) {
    FILE *fp = NULL;

    if (argc == 1) {
        fp = stdin;
    } else if (argc == 2) {
        // open file
        fp = fopen(argv[1], "rb");
    } 
    // else {
    //     RAISE(Checked_Runtime);
    // }

    //TODO: File(s) ['stderr'] is (are) empty (1)


    Pnmrdr_T test = Pnmrdr_new(fp);

    Pnmrdr_mapdata data = Pnmrdr_data(test);
    int width = data.width;
    int height = data.height;

    UArray2_T newUArr2 = UArray2_new(width, height, 8);

    for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++){
                    UArray2_at(newUArr2, i, j) = &((int)Pnmrdr_get(test));
                    printf ("",UArray2_at(newUArr2, i, j));
            }
    }
    

   
    fclose(fp);

    Pnmrdr_free (&test);

    UArray2_new(
    )

}
