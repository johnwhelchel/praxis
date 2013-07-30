/********************************************************

John Whelchel
July 22, 2013
Summer Programming Practice

This program is supposed to take a command line argument N,
and produce all the vampire numbers of length N digits. See
the problem.txt for a description of vampire numbers.

********************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {

    int N;
    int fangsize;


    if (argc != 2) {
        printf("Usage: %s N, where N is the number of digits in each vampire number.\n", argv[0]);
        return 1;
    }

    else {

        N = (int) strtol(argv[1], (char**) NULL, 10);
        fangsize = N >> 1;

        if ((fangsize*2) != N) {
            printf("Given digit size must be divisible by two.\n");
            return 1;
        }

        printf("Calculating %d digit vampire numbers. Working...\n", N);

        int smallest_fang = pow(10.0, (double) (fangsize - 1));
        int largest_fang = pow(10.0, (double) fangsize);
        // For loop iterating over all numbers with fangsize digits
        for (int i = smallest_fang; i < largest_fang; i++) {

            char fang1_word[N];
            sprintf(fang1_word, "%d", i);

            for (int j = i; j < largest_fang; j++) {

                int dracula;
                char dracula_word[N];
                int * check_digits;
                char fang2_word[N];

                check_digits = calloc(10, sizeof(int));

                // Verify they both aren't divisible by ten.
                if (((i % 10) == 0) && ((j % 10) == 0)) { 
                    free(check_digits);
                    continue;   
                }


                // Calculate potential vampire
                dracula = i * j;

                // Verify potential vampire has enough digits
                sprintf(dracula_word, "%d", dracula);
                sprintf(fang2_word, "%d", j);

                if ((int) strlen(dracula_word) != N) {
                    free(check_digits);
                    continue;
                }


                // Count up all the vampire's digits into check_digits
                for (int k = 0; k < N; k++) {
                    char digit_char;
                    int digit;

                    digit_char = dracula_word[k];
                    digit = (int) strtol( (const char *) &digit_char, (char**) NULL, 10);
                    check_digits[digit]++;
                }

                // See if they all match. No need to make sure any value of
                // check_digits is above zero because digits must already add
                // up.
                int failed = 0;
                for (int k = 0; k < N/2; k++) {
                    char digit_char;
                    int digit;

                    digit_char = fang1_word[k];
                    digit = (int) strtol( (const char *) &digit_char, (char**) NULL, 10);

                    check_digits[digit]--;
                    if (check_digits[digit] < 0) {
                        failed = 1;
                        break;
                    }

                    digit_char = fang2_word[k];
                    digit = (int) strtol( (const char *) &digit_char, (char**) NULL, 10);

                    check_digits[digit]--;
                    if (check_digits[digit] < 0) {
                        failed = 1;
                        break;
                    }
                }

                // Failed at some point during digit check phase
                if (failed) { 
                    free(check_digits);
                    continue;   
                }
                
                // They all seem to match. Print vampire number
                else {
                    printf("Found vampire number: %d * %d = %d\n", i, j, dracula);
                }
            }
        }
    }
    printf("Done.\n");
    return 0;
}
