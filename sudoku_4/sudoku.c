/********************************************************************** 

Program: sudoku
Author: John Whelchel
Date: 7/26/13

Description: 

This program reads a sudoku puzzle from stdin, solves it, and prints
the result. The board given must be a comma seperated file with 9 lines
and 9 numbers per line, with zero (or nothing) representing empty spaces.

** CHECK_ONLY Needs to be fixed, currently commented out. Doesn't work 
for easy...

Usage:

% sudoku < puzzle.csv

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct node {
    struct node* next;
    int value;
} node;

typedef struct point {
    int x;
    int y;
    int seen;
} point;

int list_length(node* head) {
    int len = 1;
    if(head == NULL) return 0;
    while(head->next != NULL) {
        len++;
        head = head->next;
    }
    return len;
}

node* list_remove(node* head, int i) {
    node* prev;
    node* new_head;

    // Remove first in list if necessary.
    new_head = head;
    if (head == NULL) return NULL;
    else if (head->value == i) {
            new_head = head->next;
            free(head);
            return new_head;
    }
    if (head->next == NULL) return new_head;
    do {
        prev = head;
        head = head->next;
        if (head->value == i) {
            prev->next = prev->next->next;
            free(head);
            return new_head;
        }
        // Past the point where it could be.
        else if (head->value > i) {
            return new_head;
        }
    } while(head->next != NULL);
    // Number not found
    return new_head;
}

int in_list(node* head, int i) { 
    node* prev;
    prev = head;
    if (head == NULL) return 0;
    if (prev->value == i) return 1;
    while(prev->next != NULL) {
        prev = prev->next;
        if (prev->value == i) return 1;
    }
    return 0;
}

// Print 9x9 sudoku puzzle
void print_puzzle(int puzzle[9][9]) {
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) printf("\n");
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0) printf("  ");
            if (puzzle[i][j] == 0) printf("  _");
            else {
                printf("%3d", (int) puzzle[i][j]);
            }
        }
        printf("\n");
    }
}

// Print possibilities list
void print_possibilities(int puzzle[9][9], node* possibilities[9][9]) {
    node* next;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] != 0) {
                continue;
            }
            next = possibilities[i][j];
            if (next != NULL) {
                printf("Possibilities for puzzle[%d][%d]:", i + 1, j + 1);
                printf("%2d", next->value);
                do {
                    next = next->next;
                    printf(", %1d", next->value);
                } while (next->next != NULL);
                printf("\n");
            }
        }
    }
}

// Check to see for each row, quadrant, and column if one square
// has a possibile number that no other square has. If so, set.
void check_only(int puzzle[9][9], node* possibilities[9][9]) {

    point only[9]; // array from 1 to 9 representing the best (i, j) for number
    
    // rows
    for (int i = 0; i < 9; i++) {
        // clear out only
        for (int z = 0; z < 9; z++) {
            only[z].seen = 0;
        }
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] != 0) continue; // square is done.

            // iterate over possibilites for square[i][j]
            node * poss_next;
            poss_next = possibilities[i][j];
            while(1) {
                // if first time seen
                if (only[poss_next->value - 1].seen == 0 ) {
                    only[poss_next->value - 1].x = i;
                    only[poss_next->value - 1].y = j;
                    only[poss_next->value - 1].seen = 1;
                }
                else if (only[poss_next->value - 1].seen == 1 ) {
                    // more than one possible spot for this number in this row.
                    only[poss_next->value - 1].seen = 2;
                }
                if (poss_next->next == NULL) break;
                poss_next=poss_next->next;
            }
        }
        // row i finished (0 - 8 in j)
        // if seen is one, that means it's the only place for that number
        for (int z = 0; z < 9; z++) {
            if (only[z].seen == 1) {
                //printf("Fixed one square at [%d][%d]. Set as %d.\n", only[z].x,only[z].y,z + 1);
                puzzle[only[z].x][only[z].y] = z + 1;
            }
        }
    }
    // columns
    for (int i = 0; i < 9; i++) {
        // clear out only
        for (int z = 0; z < 9; z++) {
            only[z].seen = 0;
        }
        for (int j = 0; j < 9; j++) {
            if (puzzle[j][i] != 0) continue;
            node * poss_next;
            poss_next = possibilities[j][i];
            while(1) {
                // if first time seen
                if (only[poss_next->value - 1].seen == 0 ) {
                    only[poss_next->value - 1].x = j;
                    only[poss_next->value - 1].y = i;
                    only[poss_next->value - 1].seen = 1;
                }
                else if (only[poss_next->value - 1].seen == 1 ) {
                    // more than one possible spot for this number in this row.
                    only[poss_next->value - 1].seen = 2;
                }
                if (poss_next->next == NULL) break;
                poss_next=poss_next->next;
            }
        }
        // if seen is one, that means it's the only place for that number
        for (int z = 0; z < 9; z++) {
            if (only[z].seen == 1) {
                //printf("Fixed one square at [%d][%d]. Set as %d.\n",only[z].x,only[z].y,z + 1 );
                puzzle[only[z].x][only[z].y] = z + 1;
            }
        }
    }

    point only_q[9];
    // quadrants
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // clear out only for each quadrant
            for (int z = 0; z < 9; z++) {
                only_q[z].seen = 0; 
            }
            // iterate in each quadrant
            for (int l = 0; l < 3; l++) {
                for (int m = 0; m < 3; m++) {
                    if (puzzle[i*3 + l][j*3 + m] != 0) continue;
                    node * poss_next;
                    poss_next = possibilities[i*3 + l][j*3 + m];
                    while(1) {
                        // if first time seen
                        if (only_q[poss_next->value - 1].seen == 0) {
                            only_q[poss_next->value - 1].x = i*3 + l;
                            only_q[poss_next->value - 1].y = j*3 + m;
                            only_q[poss_next->value - 1].seen = 1;
                        }
                        // if seen before
                        else if (only_q[poss_next->value - 1].seen == 1) {
                            only_q[poss_next->value - 1].seen = 2;
                        }
                        if (poss_next->next == NULL) break;
                        poss_next=poss_next->next;
                    }
                }
            }
            // if seen is one, that means it's the only place for that number
            for (int z = 0; z < 9; z++) {
                if (only_q[z].seen == 1) {
                    //printf("Fixed one square at [%d][%d]. Set as %d.\n",only_q[z].x,only_q[z].y,z + 1 );
                    puzzle[only_q[z].x][only_q[z].y] = z + 1;
                }
            }
        }
    }
    return; 
}

int fill_in_easy(int puzzle[9][9], node* possibilities[9][9]) {

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] != 0) continue;
            // Update possibilities list
            else {
                for (int k = 0; k < 9; k++) {
                    
                    // Check row
                    if (puzzle[i][k] != 0) {
                        possibilities[i][j] = list_remove(possibilities[i][j], puzzle[i][k]);
                    }

                    // Check column
                    if (puzzle[k][j] != 0) {
                        possibilities[i][j] = list_remove(possibilities[i][j], puzzle[k][j]);
                    }
                }
                // Check 'quadrant'
                int r_quadrant = i / 3;
                int c_quadrant = j / 3;
                for (int l = 0; l < 3; l++) {
                    for (int m = 0; m < 3; m++) {
                        if (puzzle[r_quadrant*3 + l][c_quadrant*3 + m] != 0) {
                            possibilities[i][j] = list_remove(possibilities[i][j], puzzle[r_quadrant*3 + l][c_quadrant*3 + m]);
                        }
                    }
                }
            }
            // Check to see if just size one
            if (list_length(possibilities[i][j]) == 1) {
                //printf("Fixed one square at [%d][%d]. Set as %d.\n",i,j,possibilities[i][j]->value );
                puzzle[i][j] = possibilities[i][j]->value + .1;
            }
        }
    }

    //print_possibilities(puzzle, possibilities);
    // Check to see if only possible spot for any number by row, column, and quadrant.
    //check_only(puzzle, possibilities);

    return 0;
}

void free_possibilites(node* possibilities[9][9]) {
    node* curr;
    node* prev;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (possibilities[i][j] == NULL) continue;
            curr = possibilities[i][j];
            prev = curr;
            if (curr->next == NULL) {
                free(curr);
                continue;
            }
            curr = curr->next;
            for (;curr->next != NULL; curr=curr->next) {
                free(prev);
                prev = curr;
            }
            free(curr);
        }
    }
}

node* new_possibilities_list() {
    node* next;
    node* head;

    head = calloc(1, sizeof(node));
    head->value = 1;
    next = head;

    for (int i = 2; i < 10; i++) {
        next->next = calloc(1, sizeof(node));
        next = next->next;
        next->value = i;
    }
    next = NULL;
    return head;
}

int allowed(int puzzle[9][9], int i, int j, int guess) {

    int a;
    int b;

    for (int k = 0; k < 9; k++) {
        if (puzzle[i][k] == guess) {
            return 0;
        }
        if (puzzle[k][j] == guess) {
            return 0;
        }
    }
    a = i / 3;
    b = j / 3;
    for (int c = 0; c < 3; c++) {
        for (int d = 0; d < 3; d++) {
            if(puzzle[a*3 + c][b*3 + d] == guess) {
                return 0;
            }
        }
    }
    return 1;
}

int backtrack(int puzzle[9][9], node* possibilities[9][9], int i, int j, int guess) {
    int guesscount = 1;

    if (j == 9) {
        j = 0;
        i = i + 1;
    }
    if (i == 9 ) {
        return 0;
    }
    //printf("Backtracking...Guessing %d on square [%d][%d]\n", guess, i, j);

    // If impossible for that number to be there based on previous calcs
    if (!(in_list(possibilities[i][j], guess))) {
        return -1;
    }
    
    // Ok via old list, but still might not be good because of new guesses. Go // through and check
    else {
        if (allowed(puzzle, i, j, guess)) {
            //printf("Guess acceptable. Moving forward.\n");

            int new_guess = (guess+1) % 9;
            if (new_guess == 0) new_guess = 9;
            int new_j = j + 1;
            int new_i = i;

            puzzle[i][j] = guess;
            //print_puzzle(puzzle);
            // Skip ahead to next open spot.
            for (; puzzle[new_i][new_j] != 0; new_j++) {
                if (new_j == 9) {
                    new_j = 0;
                    new_i++;
                }
            }
            
            while (backtrack(puzzle, possibilities, new_i, new_j, new_guess) == -1) {
                //printf("Guess of %d at [%d][%d] failed. Guessing one higher.\n", new_guess, new_i, new_j);
                new_guess = (new_guess + 1) % 9;
                if (new_guess == 0) new_guess = 9;
                // guessed all the way around...
                if (guesscount == 9) { 
                    // if (new_j == 9) {
                    //     continue;
                    // }
                    puzzle[i][j] = 0;
                    return -1;
                }
                guesscount++;
            }
            return 0;
        }
        // try higher guess
        else {
            return -1;
        }
    }
}   


int main(int argc, char const *argv[]) {
    
    // Variables for loading puzzle
    int puzzle[9][9];
    char line[25];       // little extra just to give room
    char seperator[] = ",";
    char* digit;
    
    node* possibilities[9][9]; // list of possibilities for each square

    int first_i = 0;
    int first_j = 0;
    int first_guess = 1;

    // Load puzzle
    for (int i = 0; i < 9; i++) {
        fgets(line, 25, stdin);
        digit = strtok(line, seperator);
        for (int j = 0; j < 9; j++) {
            if (digit == NULL) {
                printf("Improperly formatted puzzle given; not enough digits per line.\n");
                exit(1);
            }
            puzzle[i][j] = strtol(digit, NULL, 10) + .1;
            if (puzzle[i][j] != 0) {
                possibilities[i][j] = NULL;
            }
            else {
                possibilities[i][j] = new_possibilities_list();
            }
            digit = strtok(NULL, seperator);
        }
    }

    printf("\nYour puzzle:\n");
    print_puzzle(puzzle);
    printf("\nSolving...\n");
    fill_in_easy(puzzle, possibilities);
    printf("\nCurrent state of puzzle after easy finds.\n");
    print_puzzle(puzzle);
    printf("\nGuessing and backtracking...\n");

    // Skip ahead to next open spot.
    for (; puzzle[first_i][first_j] != 0; first_j++) {
        if (first_j == 9) {
            first_j = 0;
            first_i++;
        }
    }

    while(backtrack(puzzle, possibilities, first_i, first_j, first_guess) == -1) {
        first_guess++;
        if (first_guess == 10) {
            printf("I couldn't solve it :(\n");
            exit(1);
        }
    }
    printf("\nSolved.\n");
    print_puzzle(puzzle);
    free_possibilites(possibilities);
    return 0;
}
