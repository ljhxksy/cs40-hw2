#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pnmrdr.h> 
#include <except.h>
#include <stdbool.h>

#include "bit2.h"
#include "seq.h"
typedef struct {
    int col;
    int row;
} pair;

Bit2_T populateBit2(FILE *fp);
void traverse(Bit2_T *bit2, Seq_T visited, Seq_T queue);
bool is_Valid(pair Index, Bit2_T *bit2, Seq_T visited);

int main(int argc, char *argv[]) {
    FILE *fp = NULL;

    if (argc == 1) {
        fp = stdin;
    } else if (argc == 2) {
        fp = fopen(argv[1], "rb");
        assert(fp);
    } 
    // else {
    //     RAISE(Checked_Runtime);
    // }

    Bit2_T bit2 = populateBit2(fp);

    printf("\\\\\\ BIT2 BEFORE \\\\\\\n");
    for (int i = 0; i < Bit2_width(bit2); i++) {
        for (int j = 0; j < Bit2_height(bit2); j++) {
            printf("%d ", Bit2_get(bit2, i, j));
        }
        printf("\n");
    }

    Seq_T visited = Seq_seq(NULL);
    Seq_T queue = Seq_seq(NULL);
    traverse(&bit2, visited, queue);
    
    // printf("\\\\\\ BIT2 AFTER ROW_CHECK \\\\\\");
    // for (int i = 0; i < Bit2_width(bit2); i++) {
    //     for (int j = 0; j < Bit2_height(bit2); j++) {
    //         printf("%d ", Bit2_get(bit2, i, j));
    //     }
    //     printf("\n");
    // }
    //col_check(&bit2);
    
    Bit2_free(&bit2);
    Seq_free(&visited);
    Seq_free(&queue);
    return 0;
}

Bit2_T populateBit2(FILE *fp) {
    Pnmrdr_T p1 = Pnmrdr_new(fp);

    Pnmrdr_mapdata data = Pnmrdr_data(p1);
    assert(data.type == 1);

    Bit2_T newBit2 = Bit2_new(data.width, data.height);

    for (int j = 0; j < (int)data.height; j++) {
        for (int i = 0; i < (int)data.width; i++) {
            Bit2_put(newBit2, i, j, Pnmrdr_get(p1));
            printf("%d ", Bit2_get(newBit2, i, j));
        }
        printf("\n");
    }

    Pnmrdr_free(&p1);
    fclose(fp);

    return newBit2;
}

void traverse(Bit2_T *bit2, Seq_T visited, Seq_T queue) {
    (void) visited;
    for (int i = 0; i < Bit2_width(*bit2); i++) {
        pair Index;
        Index.col = i;
        Index.row = 0;

        if (Bit2_get(*bit2, i, 0) == 1) {
            printf("%d\n", Index.col);
        
            Seq_addlo(queue, &Index);
            printf("seq length: %d\n", Seq_length(queue));
            //check neighbors

            //check visited. if not in visited, add to queue
            //

        }
        
    }
    
}

// checks if bit is not out of bounds and not visited
bool is_Valid(pair Index, Bit2_T *bit2, Seq_T visited) {
    
    if (Index.col < 0 || Index.col > Bit2_width(*bit2) || 
        Index.row < 0 || Index.row > Bit2_height(*bit2)) {
        return false;
    }

    for (int i = 0; i < Seq_length(visited); i++) {
        pair Temp;
        Temp.col = *((int *)Seq_get(visited, i));
        Temp.row = *((int *)Seq_get(visited, i));
        
        if (Index.col == Temp.col && Index.row == Temp.row) {
            return false;
        }
    }
    return true;
}