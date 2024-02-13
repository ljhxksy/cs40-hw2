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
void traverse(Bit2_T bit2, Seq_T queue, int col, int row);
bool validBlackEdge(int col, int row, Bit2_T bit2, Seq_T queue);
void addNeighbors(int currCol, int currRow, Bit2_T bit2, Seq_T queue);
void checkNeighbors(int col, int row, Bit2_T bit2, Seq_T queue);
Pair initPair(int col, int row);
void loop(Bit2_T bit2, Seq_T queue);
void pbmwrite(FILE *outputfp, Bit2_T bitmap);
void writeBit(int col, int row, Bit2_T bit2, int bit, void *p1);
void insertBit(int col, int row, Bit2_T bit2, int bit, void *p1);

Pair initPair(int col, int row)
{
        Pair pair = ALLOC(sizeof(struct Pair));
        pair->col = col;
        pair->row = row;
        return pair;
}