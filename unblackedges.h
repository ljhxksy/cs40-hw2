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

/********** populateBit2 ********
 *
 * Return true if all scores are under a given limit and return number of
 * scores under the limit via reference parameter
 *
 * Parameters:
 *      int *scoresUnderLimit: address of variable to store # scores < limit
 *      int limit:             limit to compare to
 *      int scores[]:          array of scores
 *      int len:               length of scores array
 *
 * Return: true if all scores are under limit, false if not
 *
 * Expects
 *      scores and scoresUnderLimit must not be NULL
 * Notes:
 *      value of scoresUnderLimit is set to the number of scores under the limit
 *      Will CRE if scores or scoresUnderLimit is NULL
 *      May CRE if malloc fails.
 ************************/
Bit2_T populateBit2(FILE *fp);

void loop(Bit2_T bit2, Seq_T queue);
void unblackEdge(Bit2_T bit2, Seq_T queue, int col, int row);

bool validBlackEdge(int col, int row, Bit2_T bit2, Seq_T queue);
void addNeighbors(int currCol, int currRow, Bit2_T bit2, Seq_T queue);
void checkNeighbors(int col, int row, Bit2_T bit2, Seq_T queue);
Pair initPair(int col, int row);
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