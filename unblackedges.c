#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pnmrdr.h>
#include <except.h>
#include <stdbool.h>

#include "bit2.h"
#include "seq.h"
typedef struct
{
    int col;
    int row;
} pair;

Bit2_T populateBit2(FILE *fp);
void traverse(Bit2_T *bit2, Seq_T visited, Seq_T queue);
bool is_Valid(int col, int row, Bit2_T *bit2, Seq_T visited);

int main(int argc, char *argv[])
{
    FILE *fp = NULL;

    if (argc == 1)
    {
        fp = stdin;
    }
    else if (argc == 2)
    {
        fp = fopen(argv[1], "rb");
        assert(fp);
    }
    // else {
    //     RAISE(Checked_Runtime);
    // }

    Bit2_T bit2 = populateBit2(fp);

    printf("\\\\\\ BIT2 BEFORE \\\\\\\n");
    for (int i = 0; i < Bit2_width(bit2); i++)
    {
        for (int j = 0; j < Bit2_height(bit2); j++)
        {
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
    // col_check(&bit2);

    Bit2_free(&bit2);
    Seq_free(&visited);
    Seq_free(&queue);
    return 0;
}

void traverse(Bit2_T *bit2, Seq_T visited, Seq_T queue, int row) {
    for (int i = 0; i < Bit2_width(*bit2); i++) {
        if (Bit2_get(*bit2, i, row) == 1) {

            pair Index;
            Index.col = i;
            Index.row = row;
            Seq_addhi(queue, &Index);
        }
    
        while (queue != NULL) {
            
        }
    }
}

Bit2_T populateBit2(FILE *fp)
{
    Pnmrdr_T p1 = Pnmrdr_new(fp);

    Pnmrdr_mapdata data = Pnmrdr_data(p1);
    assert(data.type == 1);

    Bit2_T newBit2 = Bit2_new(data.width, data.height);

    for (int j = 0; j < (int)data.height; j++)
    {
        for (int i = 0; i < (int)data.width; i++)
        {
            Bit2_put(newBit2, i, j, Pnmrdr_get(p1));
            printf("%d ", Bit2_get(newBit2, i, j));
        }
        printf("\n");
    }

    Pnmrdr_free(&p1);
    fclose(fp);

    return newBit2;
}

// checks if bit is not out of bounds and not visited
bool is_Valid(int col, int row, Bit2_T *bit2, Seq_T visited)
{

    if (col < 0 || col > Bit2_width(*bit2) ||
        row < 0 || row > Bit2_height(*bit2))
    {
        return false;
    }

    for (int i = 0; i < Seq_length(visited); i++)
    {
        pair Temp = *((pair *)Seq_get(visited, i));

        if (col == Temp.col && row == Temp.row)
        {
            return false;
        }
    }
    return true;
}





// void traverse(Bit2_T *bit2, Seq_T visited, Seq_T queue)
// {
//     //
//     for (int i = 0; i < Bit2_width(*bit2); i++)
//     {
//         int j = 0;

//         pair Index;
//         Index.col = i;
//         Index.row = j;

//         printf("queue length: %d\n", Seq_length(queue));
//         // the while wont work if we dont push sth into the queue first

//         if (Bit2_get(*bit2, i, j) == 1)
//         {
//             Seq_addhi(queue, &Index);
//             printf("queue length: %d\n", Seq_length(queue));

//             while (queue != NULL)
//             {
//                 // Index is a pair and its value is still (i, 0) throughout
//                 if (is_Valid(i + 1, j, bit2, visited) && Bit2_get(*bit2, i + 1, j) == 1)
//                 {
//                     pair right;
//                     Index.col = i + 1;
//                     Index.row = j;
//                     Seq_addhi(queue, &right);
//                     printf("queue length: %d\n", Seq_length(queue));
//                 }

//                 // if (is_Valid(i - 1, j, bit2, visited) && Bit2_get(*bit2, i - 1, j) == 1)
//                 // {
//                 //     pair left;
//                 //     Index.col = i - 1;
//                 //     Index.row = j;
//                 //     Seq_addhi(queue, &left);
//                 // }

//             //     if (is_Valid(i, j - 1, bit2, visited) && Bit2_get(*bit2, i, j - 1) == 1)
//             //     {
//             //         pair top;
//             //         Index.col = i;
//             //         Index.row = j - 1;
//             //         Seq_addhi(queue, &top);
//             //     }

//                 if (is_Valid(i, j + 1, bit2, visited) && Bit2_get(*bit2, i, j + 1) == 1)
//                 {
//                     pair down;
//                     Index.col = i;
//                     Index.row = j + 1;
//                     Seq_addhi(queue, &down);
//                 }



//             Seq_addlo(visited, Seq_remlo(queue));
//             printf("queue length: %d\n", Seq_length(queue));
//             printf("visited length: %d\n", Seq_length(visited));
//             }

//         }

//         // if (Bit2_get(*bit2, i, 0) == 1 && is_Valid(Index, bit2, visited)) {
//         //     // i + 1, 0
//         //     // i - 1, 0
//         //     // i, 0 - 1
//         //     // i, 0 + 1
//         //     printf("%d\n", Index.col);

//         //     Seq_addlo(queue, &Index);
//         //     printf("seq length: %d\n", Seq_length(queue));
//         //     //check neighbors

//         //     //check visited. if not in visited, add to queue
//         //     //

//         // }
//     }
// }