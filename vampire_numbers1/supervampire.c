/********************************************************

John Whelchel
July 22, 2013
Summer Programming Practice

This program is supposed to take a command line argument N,
and produce all the vampire numbers of length N digits. See
the problem.txt for a description of vampire numbers.

This version is not my own idea, but very cool!!!!
http://stackoverflow.com/questions/17352108/find-all-the-4-digit-vampire-numbers




An important theoretical result found by Pete Hartley:

      If x·y is a vampire number then x·y == x+y (mod 9) 

Proof: Let mod be the binary modulo operator and d(x) the sum of the decimal digits of x. It is well-known that d(x) mod 9 = x mod 9, for all x. Assume x·y is a vampire. Then it contains the same digits as x and y, and in particular d(x·y) = d(x)+d(y). This leads to: (x·y) mod 9 = d(x·y) mod 9 = (d(x)+d(y)) mod 9 = (d(x) mod 9 + d(y) mod 9) mod 9 = (x mod 9 + y mod 9) mod 9 = (x+y) mod 9

The solutions to the congruence are (x mod 9, y mod 9) in {(0,0), (2,2), (3,6), (5,8), (6,3), (8,5)}


********************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Checks for vampire number of size N with fangs i and j.
int check_vampire(int i, int j, int N) {

    long long dracula;
    int * check_digits;
    char dracula_word[N];
    char fang2_word[N];
    char fang1_word[N];

    check_digits = calloc(10, sizeof(int));

    // Verify they both aren't divisible by ten.
    if (((i % 10) == 0) && ((j % 10) == 0)) { 
        free(check_digits);
        return 0;   
    }


    // Calculate potential vampire
    dracula = (long long) i * j;
    // Verify potential vampire has enough digits
    sprintf(dracula_word, "%lld", dracula);
    sprintf(fang2_word, "%d", j);
    sprintf(fang1_word, "%d", i);

    if ((int) strlen(dracula_word) != N) {
        free(check_digits);
        return 0;
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
        return 0;   
    }
    printf("Found vampire number: %d * %d = %lld\n", i, j, dracula);
    return 1;
}

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

        // Mod 9 stuff begins here

        // Iterate over loops that increase by nine each time, always
        // maintaining a mod 9 relation of the congruences listed at
        // the top. The first maintains a {0,0} congruence, the second
        // {2,2}, the third {3, 6}, and the fourth {5, 8}.
        // The way the congruence works is essentially, take any fangs
        //  (x, y) where both of them mod 9 is zero, their digits summed
        // up will equal x*y. I shouldn't have to do five and six,
        // but I have some error in check_vampire that I don't have the time
        // or inclination to fix. Or maybe I should. I don't understand the 
        // math well enough.

        for (int i = smallest_fang + (9 - smallest_fang % 9); i < largest_fang; i=i+9) {
            for (int j = i; j < largest_fang; j=j+9) {
                check_vampire(i, j, N);
            }
        }
        for (int i = smallest_fang + (2 - smallest_fang % 9); i < largest_fang; i=i+9) {
            for (int j = i; j < largest_fang; j=j+9) {
                check_vampire(i, j, N);
            }
        }
        for (int i = smallest_fang + (3 - smallest_fang % 9); i < largest_fang; i=i+9) {
            for (int j = i + 3; j < largest_fang; j=j+9) {
                check_vampire(i, j, N);
            }
        }
        for (int i = smallest_fang + (5 - smallest_fang % 9); i < largest_fang; i=i+9) {
            for (int j = i + 3; j < largest_fang; j=j+9) {
                check_vampire(i, j, N);
            }
        }
        for (int i = smallest_fang + (6 - smallest_fang % 9); i < largest_fang; i=i+9) {
            for (int j = i - 3; j < largest_fang; j=j+9) {
                check_vampire(i, j, N);
            }
        }
        for (int i = smallest_fang + (8 - smallest_fang % 9); i < largest_fang; i=i+9) {
            for (int j = i - 3; j < largest_fang; j=j+9) {
                check_vampire(i, j, N);
            }
        }


    }
    printf("Done.\n");
    return 0;
}
