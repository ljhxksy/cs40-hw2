#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pnmrdr.h>
#include <except.h>
#include <stdbool.h>
#include <mem.h>
#include "bit2.h"
#include "seq.h"

typedef struct Pair
{
    int col;
    int row;
} *Pair;

Bit2_T populateBit2(FILE *fp);
void traverse(Bit2_T bit2, Seq_T visited, Seq_T queue, int col, int row);
bool validBlackEdge(int col, int row, Bit2_T bit2, Seq_T visited, Seq_T queue);
void addNeighbors(int currCol, int currRow, Bit2_T bit2, 
                  Seq_T visited, Seq_T queue);
void checkNeighbors(int col, int row, Bit2_T bit2, Seq_T visited, Seq_T queue);
Pair initPair(int col, int row);
void loop(Bit2_T bit2, Seq_T visited, Seq_T queue);

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

    loop(bit2, visited, queue);

    while (Seq_length(visited) > 0)
    {
        free((Pair)Seq_remhi(visited));
    }

    printf("\\\\\\ BIT2 AFTER ROW_CHECK \\\\\\\n");

        for (int i = 0; i < Bit2_width(bit2); i++) {
                for (int j = 0; j < Bit2_height(bit2); j++) {
            printf("%d ", Bit2_get(bit2, i, j));
        }
        printf("\n");
    }
    // col_check(&bit2);

    Bit2_free(&bit2);
    Seq_free(&visited);
    Seq_free(&queue);
    return 0;
}

void loop(Bit2_T bit2, Seq_T visited, Seq_T queue) {
    for (int i = 0; i < Bit2_height(bit2); i++) {
        traverse(bit2, visited, queue, 0, i);
        traverse(bit2, visited, queue, Bit2_width(bit2) - 1, i);
    }

    for (int i = 0; i < Bit2_width(bit2); i++)
    {
        traverse(bit2, visited, queue, i, 0);
        traverse(bit2, visited, queue, i, Bit2_height(bit2) - 1);
    }
    
    ///////TODO: If it's all correct from the beginning
}

void traverse(Bit2_T bit2, Seq_T visited, Seq_T queue, int col, int row) {
    // for (int i = 0; i < Bit2_width(bit2); i++) {
        if (Bit2_get(bit2, col, row) == 1) {
            Pair index = initPair(col, row);
            assert(index); //TODO: assert after every init

            fprintf(stderr, "edge at orig line: (%d, %d)\n", index->col, index->row);
            Seq_addhi(queue, (void*)index);
        }
    
        while (Seq_length(queue) > 0) {
            Pair frontQueue = (Pair)Seq_remlo(queue);
            fprintf(stderr, "(%d, %d)\n", frontQueue->col, frontQueue->row);
            Seq_addhi(visited, (void*)frontQueue);
            Bit2_put(bit2, frontQueue->col, frontQueue->row, 0);
            printf("frontQueue: elem at (%d,%d): %d\n", frontQueue->col, frontQueue->row, Bit2_get(bit2, frontQueue->col, frontQueue->row));
            addNeighbors(frontQueue->col, frontQueue->row, bit2, visited, queue);
        }
    // }
    
}

void addNeighbors(int currCol, int currRow, Bit2_T bit2, Seq_T visited, Seq_T queue) 
{
    printf("begin addNeighbors\n");
    checkNeighbors(currCol + 1, currRow, bit2, visited, queue);
    checkNeighbors(currCol - 1, currRow, bit2, visited, queue);
    checkNeighbors(currCol, currRow - 1, bit2, visited, queue);
    checkNeighbors(currCol, currRow + 1, bit2, visited, queue);
    printf("queue length: %d\n", Seq_length(queue));
    printf("////QUEUE/////\n");
    for (int i = 0; i < Seq_length(queue); i++) {
        Pair temp = Seq_get(queue, i);

        printf("(%d,%d), ", temp->col, temp->row);
    }
    printf("\n\n");
    printf("visited length: %d\n", Seq_length(visited));
    printf("////VISITED/////\n");
    for (int i = 0; i < Seq_length(visited); i++) {
        Pair temp = Seq_get(visited, i);

        printf("(%d,%d), ", temp->col, temp->row);
    }
    printf("\n");

}

void checkNeighbors(int col, int row, Bit2_T bit2, Seq_T visited, Seq_T queue)
{
    // bool validBlackEdge(int col, int row, Bit2_T *bit2, Seq_T visited)
    // printf("begin checkNeighbors\n");
    if (validBlackEdge(col, row, bit2, visited, queue)) {
        if (Bit2_get(bit2, col, row) == 1) {
            Pair edgePos = initPair(col, row);
            Seq_addhi(queue, edgePos);
        }
    }
}

Bit2_T populateBit2(FILE *fp)
{
    Pnmrdr_T p1 = Pnmrdr_new(fp);

    Pnmrdr_mapdata data = Pnmrdr_data(p1);
    assert(data.type == 1);

    Bit2_T newBit2 = Bit2_new(data.width, data.height);

    for (int i = 0; i < (int)data.width; i++)
    {
        for (int j = 0; j < (int)data.height; j++)
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
bool validBlackEdge(int col, int row, Bit2_T bit2, Seq_T visited, Seq_T queue)
{
    if (col < 0 || col >= Bit2_width(bit2) ||
        row < 0 || row >= Bit2_height(bit2)) {
        return false;
    }

    for (int i = 0; i < Seq_length(visited); i++) {
        Pair Temp = Seq_get(visited, i);
        assert(Temp);
        if (col == Temp->col && row == Temp->row) {
            return false;
        }
    }
    
    for (int i = 0; i < Seq_length(queue); i++) {
        Pair Temp = Seq_get(queue, i);
        assert(Temp != NULL);
        if (col == Temp->col && row == Temp->row)
        {
            return false;
        }
    }
    // printf("valid: (col: %d, row: %d)\n", col, row);
    return true;
}


Pair initPair(int col, int row) {
    Pair pair = ALLOC(sizeof(struct Pair));
    pair->col = col;
    pair->row = row;
    return pair;
}
