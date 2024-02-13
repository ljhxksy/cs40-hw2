/**************************************************************
 *
 *                     unblackedges.c
 *
 *     Assignment: iii
 *     Authors:  Trin Changkasiri (pchang08) & Anh Hoang (ahoang05)
 *     Date:     02/12/2024
 *
 *     This file contains the implementation of unblackedges, which removes 
 *     black edges from a scanned image (P1/P5 pbm file).
 *
 **************************************************************/

#include <unblackedges.h>

Except_T Checked_Runtime = {"Checked Runtime Error"};

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

        Bit2_T bit2 = populateBit2(fp);
        Seq_T queue = Seq_seq(NULL);

        loop(bit2, queue);
        pbmwrite(stdout, bit2);

        Bit2_free(&bit2);
        Seq_free(&queue);
        return 0;
}

void pbmwrite(FILE *outputfp, Bit2_T bitmap)
{
        assert(outputfp);
        fprintf(outputfp, "P1\n%d %d\n", Bit2_width(bitmap), 
                                         Bit2_height(bitmap));
        Bit2_map_row_major(bitmap, writeBit, NULL);
        fclose(outputfp);
}

void writeBit(int col, int row, Bit2_T bit2, int bit, void *p1)
{
        (void)row;
        (void)p1;

        printf("%d ", bit);
        if (col == Bit2_width(bit2) - 1 || col == 35) printf("\n");
}

void loop(Bit2_T bit2, Seq_T queue)
{
        for (int i = 0; i < Bit2_height(bit2); i++) {
                unblackEdge(bit2, queue, 0, i);
                unblackEdge(bit2, queue, Bit2_width(bit2) - 1, i);
        }

        for (int i = 0; i < Bit2_width(bit2); i++) {
                unblackEdge(bit2, queue, i, 0);
                unblackEdge(bit2, queue, i, Bit2_height(bit2) - 1);
        }
}

void unblackEdge(Bit2_T bit2, Seq_T queue, int col, int row)
{
        if (Bit2_get(bit2, col, row) == 1) {
                Pair index = initPair(col, row);
                assert(index);
                Seq_addhi(queue, (void *)index);
        }

        while (Seq_length(queue) > 0) {
                Pair frontQueue = (Pair)Seq_remlo(queue);
                Bit2_put(bit2, frontQueue->col, frontQueue->row, 0);
                addNeighbors(frontQueue->col, frontQueue->row, bit2, queue);
                free(frontQueue);
        }
}

void addNeighbors(int currCol, int currRow, Bit2_T bit2, Seq_T queue)
{
        checkNeighbors(currCol + 1, currRow, bit2, queue);
        checkNeighbors(currCol - 1, currRow, bit2, queue);
        checkNeighbors(currCol, currRow - 1, bit2, queue);
        checkNeighbors(currCol, currRow + 1, bit2, queue);
}

void checkNeighbors(int col, int row, Bit2_T bit2, Seq_T queue)
{
        if (validBlackEdge(col, row, bit2, queue)) {
                if (Bit2_get(bit2, col, row) == 1) {
                        Pair edgePos = initPair(col, row);
                        assert(edgePos);
                        Seq_addhi(queue, edgePos);
                }
        }
}

Bit2_T populateBit2(FILE *fp)
{
        Pnmrdr_T p1 = Pnmrdr_new(fp);

        Pnmrdr_mapdata data = Pnmrdr_data(p1);
        assert(data.type == 1 || data.type == 4);
        assert(data.height != 0 && data.width != 0);

        Bit2_T newBit2 = Bit2_new(data.width, data.height);
        Bit2_map_row_major(newBit2, insertBit, &p1);

        Pnmrdr_free(&p1);
        fclose(fp);

        return newBit2;
}

void insertBit(int col, int row, Bit2_T bit2, int bit, void *p1)
{
        (void)bit;
        Bit2_put(bit2, col, row, Pnmrdr_get(*(Pnmrdr_T *)p1));
}

// checks if bit is not out of bounds and not visited
bool validBlackEdge(int col, int row, Bit2_T bit2, Seq_T queue)
{
        if (col < 0 || col >= Bit2_width(bit2) ||
            row < 0 || row >= Bit2_height(bit2)) {
                return false;
        }

        for (int i = 0; i < Seq_length(queue); i++) {
                Pair Temp = Seq_get(queue, i);
                assert(Temp != NULL);
                if (col == Temp->col && row == Temp->row) {
                        return false;
                }
        }
        return true;
}
