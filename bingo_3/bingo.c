/********************************************************************** 

Program: bingo
Author: John Whelchel
Date: 7/24/13

Description: 

This program runs an imaginary bingo game N times with M cards in play.
It then outputs the average number of calls for anyone acheive bingo.

Usage:

% bingo N M

**********************************************************************/

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

// -1 means called.
typedef struct board {
    int b[5][5];
} board;


int bingo_rand(int column) {
    return ((rand() % 15) + 1) + 15 * column;
}

// Sets a new number n in the board at position i, j, making sure
// it hasn't been taken yet. Returns 0 when succesful
void set_num(board* b, int i, int j, int n) {
    for (int w = 0; (w<j) && (w<5); w++) {
        if (n == b->b[w][i]) {
            set_num(b, i, j, bingo_rand(i));
            return;
        }
    }
    b->b[j][i] = n;
    return;
}

//  You are responsible for freeing!
board* new_board() {

    board* b;
    int r_num;

    b = calloc(1, sizeof(board));
    if (b == NULL) {
        printf("Out of memory.\n");
        return  NULL;
    }

    // randomize
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            // Random number from 1-15, 16-30, etc. by column (switch j and i)
            r_num = bingo_rand(i);
            set_num(b, i, j, r_num);
        }
    }
    // Free space!
    b->b[2][2] = -1;

    return b;
}

// Assume n is valid 1-75
int has_num(board* b, int n) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            // printf("Does %d match %d?\n", b->b[i][j], n);
            if (b->b[i][j] == n) {
                // printf("Found\n");
                b->b[i][j] = -1;
                return 1;
            }
        }
    }
    return 0;
}

// Returns 1 on bingo
int has_bingo(board* b) {
    int row_check;
    int col_check;
    int diag_check;

    for (int i = 0; i < 5; i++) {
        row_check = 0;
        col_check = 0;
        for (int j = 0; j < 5; j++) {
            if (b->b[i][j] == -1) {
                row_check++;
            }
            if (b->b[j][i] == -1) {
                col_check++;
            }
        }
        // Some row or column is bingo
        if ((row_check == 5) || (col_check == 5)) return 1;
    }

    // Check diagonals
    diag_check = 0;
    for (int j = 0; j < 5; j++) {
        diag_check += b->b[j][j];
    }
    if (diag_check == 5) return 1;

    diag_check = 0;
    for (int k = 0; k < 5; k++) {
        diag_check += b->b[4 - k][4 - k];
    }
    if (diag_check == 5) return 1;

    return 0;
}

// // Pointer to array of boards of size M
// // Returns one if no one has won yet (i.g. 0 in bingo)
// int no_winners(board** boards, int M) {
//     for (int i = 0; i < M; i++) {
//         if (has_bingo(boards[i])) return 0;
//     }
//     return 1;
// }

int main(int argc, char const *argv[]) {

    if (argc != 3) {
        printf("Usage: bingo N M, where N is the number of bingo games and M is the number of cards in play..\n");
        return -1;
    }

    int N = (int) strtol(argv[1], NULL, 10);
    int M = (int) strtol(argv[2], NULL, 10);

    if ((N < 0) || (M < 0)) {
        printf("N and M must both be positive.\n");
        return -1;
    }


    // Initialize random seed
    srand ( (unsigned int) time(NULL) );


    // Create games
    double all_games_average = 0.0;
    int bingo;
    int new_num;
    int old_num;
    int count;
    board ** boards;

    int ** called_numbers;
    called_numbers = (int **) calloc(M, sizeof(void *));
    
    for (int i = 0; i < N; i++) {

        // Create zeroed ut array keeping track of called numbers in this game.
        called_numbers[i] = calloc(75 + 1, sizeof(int));

        // Allocate memory for boards
        boards = calloc(M, sizeof(void *));

        printf("Playing game %d with %d boards.\n", i, M);

        // Create boards w/ numbers
        for (int j = 0; j < M; j++) {
            boards[j] = new_board();
        }

        bingo = 0;
        count = 0;
        while(bingo == 0) {

            // Call a number.
            new_num = rand() % 75 + 1;
            // printf("first drawn number is %d\n\n", new_num);
            // Has it been called?
            while (called_numbers[i][new_num]) {
                // Check to see if all have been called.
                for (int poop = 0; poop < 75 + 1; poop++) {
                    if (called_numbers[i][poop] == 0) break;
                    if (poop == 75) {
                        // printf("All numbers called\n");
                        // printf("Count was %d\n", count);
                        abort();
                    }
                }
                // printf("\n");

                // Has been called, get a new one.
                old_num = new_num;
                new_num = rand() % 75 + 1;
                // printf("%d was already called. Trying to find new_rand -> %d\n\n", old_num, new_num);

                // printf("Printing all board's states.\n");
                for (int r = 0; r < M; r++) {
                    // printf("Board %d\n\n", r);
                    for (int y = 0; y < 5; y++) {
                        for (int yum = 0; yum < 5; yum++) {
                            // printf("%d   ", boards[r]->b[y][yum]);
                        }
                        // printf("\n");
                    }
                }
            }

            // Mark number as called.
            called_numbers[i][new_num] = 1;
            // printf("calling %d\n", new_num);
            count++;



            // update boards with new_num, check if necessary
            for (int q = 0; q < M; q++) {
                if(has_num(boards[q], new_num)) {
                    if(has_bingo(boards[q])) {
                        bingo = 1;
                        break;
                    }
                }
            }
            if (bingo == 0) {
                //printf("No winners yet.\n\n\n");
            }
            else {
                //printf("Winner! Next game...\n");
                // first game
                if (i == 0) {
                    all_games_average = count;
                }
                else {
                    all_games_average = (all_games_average * (i) + count) / (i + 1.0);
                }
            }
        }
        printf("The number of calls required for this game was %d.\n", count);
    }
    for (int board_num = 0; board_num < M; board_num++)
    {
        free(boards[board_num]);
    }
    free(boards);
    printf("The average number of calls require to reach bingo across all games was %f.\n", all_games_average);
    return 0;
}


















