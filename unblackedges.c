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
void checkEdges(Bit2_T bit2, Seq_T queue);
void unblackEdge(Bit2_T bit2, Seq_T queue, int col, int row);
bool validBlackEdge(int col, int row, Bit2_T bit2, Seq_T queue);
void addNeighbors(int currCol, int currRow, Bit2_T bit2, Seq_T queue);
void checkNeighbors(int col, int row, Bit2_T bit2, Seq_T queue);
Pair initPair(int col, int row);
void pbmwrite(FILE *outputfp, Bit2_T bitmap);
void writeBit(int col, int row, Bit2_T bit2, int bit, void *p1);
void insertBit(int col, int row, Bit2_T bit2, int bit, void *p1);

Except_T Checked_Runtime = { "Checked Runtime Error" };

int main(int argc, char *argv[])
{
        FILE *fp = NULL;
        /* Open file, checks for valid arguments */
        if (argc == 1) {
                fp = stdin;
        } else if (argc == 2) {
                fp = fopen(argv[1], "rb");
                assert(fp);
        } else {
                RAISE(Checked_Runtime);
        }

        /* Initializes bit2 and "queue" sequence */
        Bit2_T bit2 = populateBit2(fp);
        Seq_T queue = Seq_seq(NULL);

        checkEdges(bit2, queue);
        pbmwrite(stdout, bit2);

        /* Frees memory for bit2 and queue */
        Bit2_free(&bit2);
        Seq_free(&queue);
        return 0;
}

/********** initPair ********
 *
 *      Allocates, initializes, and returns a new pair of integers column
 *      and width, which is the index of the bit in the bitmap
 *
 * Parameters:
 *      int col: the column index
 *      in row: the row index
 *
 * Return: the new pair
 *
 * Expects:
 *      Memory allocation for pair should not fail
 *      
 * Notes:
 *      CRE if allocation fails
 * 
 ************************/
Pair initPair(int col, int row)
{
        Pair pair = ALLOC(sizeof(struct Pair));
        assert(pair);
        pair->col = col;
        pair->row = row;
        return pair;
}

/********** pbmwrite ********
 *
 *      Write to standard output a plain (P1) format portable bitmap file
 *      which has width, height and pixel values identical to the original file 
 *      except that all black edge pixels are changed to white. 
 *
 * Parameters:
 *      FILE *outputfp: A pointer to the output file
 *      Bit2_T bitmap: the bit2 bitmap
 *
 * Return: none
 *
 * Expects:
 *      The output file should not be NULL
 *      
 * Notes:
 *      CRE if outputfp is NULL
 *      uses the Bit2_map_row_major function, whose apply function is writeBit()
 *      Close the output file
 * 
 ************************/
void pbmwrite(FILE *outputfp, Bit2_T bitmap)
{
        assert(outputfp);
        fprintf(outputfp, "P1\n%d %d\n", Bit2_width(bitmap), 
                                         Bit2_height(bitmap));
        Bit2_map_row_major(bitmap, writeBit, NULL);
        fclose(outputfp);
}

/********** writeBit ********
 *
 *      Print out each bit from the bit2 bitmap
 *
 * Parameters:
 *      int col: the column index of the current bit
 *      int row: the row index of the current bit. is voided
 *      Bit2_T bit2: the bit2 bitmap
 *      int bit: the value of the current bit (either 0 or 1)
 *      void *p1: closure pointer. is voided
 *
 * Return: none
 *
 * Expects:
 *      Prints out the current bit
 *      Prints out endline (\n) if the column length has reached the max column 
 *      length for a P1 file (70 characters)
 *      
 * Notes:
 *      The row index and p1 pointer is voided
 * 
 ************************/
void writeBit(int col, int row, Bit2_T bit2, int bit, void *p1)
{
        (void)row;
        (void)p1;

        printf("%d ", bit);
        if (col == Bit2_width(bit2) - 1 || col == 35) {
                printf("\n");
        }
}

/********** checkEdges ********
 *
 *      Traverse each edge (right, left, top, bottom) to unblack edges
 *
 * Parameters:
 *      Bit2_T bit2: the bit2 bitmap
 *      Seq_T queue: the queue, which is a sequence of pair indices 
 *
 * Return: none
 *
 * Expects:
 *      Call the unblackEdge function on each edge
 *      
 * Notes:
 *      none
 * 
 ************************/
void checkEdges(Bit2_T bit2, Seq_T queue)
{
        /* Check the left and right edges */
        for (int i = 0; i < Bit2_height(bit2); i++) {
                unblackEdge(bit2, queue, 0, i);
                unblackEdge(bit2, queue, Bit2_width(bit2) - 1, i);
        }

        /* Check the top and bottom edges */
        for (int i = 0; i < Bit2_width(bit2); i++) {
                unblackEdge(bit2, queue, i, 0);
                unblackEdge(bit2, queue, i, Bit2_height(bit2) - 1);
        }
}

/********** unblackEdge ********
 *
 *      Search for black edges breadth first, then push onto the queue and
 *      unblack the edge.
 *
 * Parameters:
 *      Bit2_T bit2: the bit2 bitmap
 *      Seq_T queue: the queue, which is a sequence of pair indices 
 *      int col: the column index of the bit
 *      int row: the row index of the bit
 *
 * Return: none
 *
 * Expects:
 *      The index pair, which is to be pushed into the queue, should not be 
 *      NULL after initializing
 *      
 * Notes:
 *      the index pair != NULL
 *      calls addNeighbors, which adds neighboring black edges onto the queue
 *      Removes the front index off the queue, then free its memory.
 *      The memory for the queue will be deallocated in main().
 *      Memory for bit2 will be freed in main()
 * 
 ************************/
void unblackEdge(Bit2_T bit2, Seq_T queue, int col, int row)
{
        /* If the bit at (col, row) is a black edge, add into queue */
        if (Bit2_get(bit2, col, row) == 1) {
                Pair index = initPair(col, row);
                assert(index);
                Seq_addhi(queue, (void *)index);
        }

        while (Seq_length(queue) > 0) {
                Pair frontQueue = (Pair)Seq_remlo(queue);
                /* Put the unblacked edge back into bit2 */
                Bit2_put(bit2, frontQueue->col, frontQueue->row, 0);
                
                addNeighbors(frontQueue->col, frontQueue->row, bit2, queue);
                free(frontQueue);
        }
}

/********** addNeighbors ********
 *
 *      Search for neighboring black edges from the current black edge
 *
 * Parameters:
 *      int currCol: the column index of the bit
 *      int currRow: the row index of the bit
 *      Bit2_T bit2: the bit2 bitmap
 *      Seq_T queue: the queue, which is a sequence of pair indices 

 *
 * Return: none
 *
 * Expects:
 *      none
 *      
 * Notes:
 *      calls checkNeighbors on all valid neighbors of provided index.
 * 
 ************************/
void addNeighbors(int currCol, int currRow, Bit2_T bit2, Seq_T queue)
{
        /* Check the right, left, top, bottom of the current bit */
        checkNeighbors(currCol + 1, currRow, bit2, queue);
        checkNeighbors(currCol - 1, currRow, bit2, queue);
        checkNeighbors(currCol, currRow - 1, bit2, queue);
        checkNeighbors(currCol, currRow + 1, bit2, queue);
}

/********** checkNeighbors ********
 *
 *      Checks edge pixels for a valid black edge, adds to "queue" sequence if 
 *      so. 
 *
 * Parameters:
 *      int col: the column index
 *      int row: the row index
 *      Bit2_T bit2: bit2 containing all pixels within file
 *      Seq_T queue: sequence containing all indexes to be visited
 *
 * Return: N/A(void)
 *
 * Expects:
 *      The index of a valid black edge.
 *      
 * Notes:
 *      CRE if struct containing valid edge pixel isn't correctly initiated.
 *      Used to call subsidiary functions validBlackEdge and initPair to add an 
 *      index to the queue.
 *      The memory for the queue will be deallocated in main().
 * 
 ************************/
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

/********** populateBit2 ********
 *
 *      Read in the pbm file using the Pnmrdr interface and populate 
 *      its data into a bit2.
 *
 * Parameters:
 *      FILE *fp: a pointer to file
 *
 * Return: the new, populated bit2.
 *
 * Expects:
 *      The input file should not be NULL
 *      The input file be a valid pbm file (P1 or P4)
 *      The height and width of the image should be > 0
 *      
 * Notes:
 *      CRE if fp == NULL
 *      CRE if data.type != 1 || data.type != 4
 *      CRE if data.height <= 0 || data.width <= 0
 * 
 ************************/
Bit2_T populateBit2(FILE *fp)
{
        assert(fp);
        Pnmrdr_T p1 = Pnmrdr_new(fp);
        
        /* Checks for valid map dimensions by utilizing Pnmrdr Function */
        Pnmrdr_mapdata data = Pnmrdr_data(p1);
        assert(data.type == 1 || data.type == 4);
        assert(data.height > 0 && data.width > 0);

        /* Inserts file contents into Bit2 */
        Bit2_T newBit2 = Bit2_new(data.width, data.height);
        Bit2_map_row_major(newBit2, insertBit, &p1);

        Pnmrdr_free(&p1);
        fclose(fp);

        return newBit2;
}

/********** insertBit ********
 *
 *      Inserts the provided bit into the index of choice via map function.
 *
 * Parameters:
 *      int col: the column index
 *      int row: the row index
 *      Bit2_T bit2: bit2 containing all pixels within file
 *      int bit: voided integer value.
 *      void *p1: file pointer used to access file.
 *
 * Return: N/A(void)
 *
 * Expects:
 *      The input file should not be NULL
 *      
 * Notes:
 *      CRE if fp == NULL
 *      Used in the place of apply function via Bit2_map
 *      Memory for bit2 will be freed in main()
 * 
 ************************/
void insertBit(int col, int row, Bit2_T bit2, int bit, void *p1)
{
        assert(p1);
        (void)bit;
        Bit2_put(bit2, col, row, Pnmrdr_get(*(Pnmrdr_T *)p1));
}

/********** validBlackEdge ********
 *
 *      Allocates, initializes, and returns a new pair of integers column
 *      and width, which is the index of the bit in the bitmap
 *
 * Parameters:
 *      int col: the column index
 *      int row: the row index
 *      Bit2_T bit2: bit2 containing all pixels within file
 *      Seq_T queue: sequence containing all indexes to be visited
 *
 * Return: boolean indicating whether the index provided is a valid black edge 
 *         pixel
 *
 * Expects:
 *      col, row should not be out of bounds from the bitmap
 *      pair retrieved from queue sequence to be valid pair
 *      
 * Notes:
 *      CRE if pair retrieved from queue sequence is NULL
 * 
 ************************/
bool validBlackEdge(int col, int row, Bit2_T bit2, Seq_T queue)
{
        /* Checks if the current index is out of bounds */
        if (col < 0 || col >= Bit2_width(bit2) ||
            row < 0 || row >= Bit2_height(bit2)) {
                return false;
        }

        /* Checks if the current index is already in the queue */
        for (int i = 0; i < Seq_length(queue); i++) {
                Pair Temp = Seq_get(queue, i);
                assert(Temp != NULL);
                if (col == Temp->col && row == Temp->row) {
                        return false;
                }
        }
        return true;
}
