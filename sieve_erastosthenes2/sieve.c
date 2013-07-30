/********************************************************************** 

Program: sieve
Author: John Whelchel
Date: 7/23/13

Description: 

This program computes all prime numbers up to and including N using
the ancient algorithm known as the Sieve of Erastosthenes. Basic
brute force with large array. Works up to about a max of 2.5*10^8, and then
starts to break aftarward.

sievediff used a different implementation out of curiosity that skipped
every number ending in five. Ended up being wasteful and slower by a little
bit. It also was before the square/square root stuff.

Caculates all primes up to 5*1^7 in 10.224 seconds. (sievediff was 11.1)

Usage:

$ sieve N

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Usage: sieve N, where N is the desired maximum value for the sieve.\n");
        return -1;
    }

    long long N = (long long) strtoull(argv[1], NULL, 10);

    // Stop sieve at sqrt of max # in sieve, because all non-primes are 
    // already gone.
    int M = (int) sqrt((double) N);

    if (N < 0) {
        printf("N must be positive.\n");
        return -1;
    }
    if (N < 2) {
        printf("No primes.\n");
        return 0;
    }

    int * primes;
    primes = calloc(N, sizeof(int));

    // 0 is untouched, 1 is not prime
    printf("2\n");
    for (int i = 3; i <= M; i=i+2) {
        if (primes[i] == 1) continue;
        printf("%d\n", i);
        for (int j = i * i; j <= N; j=j+i) {
            if (primes[j] == 1) continue;
                primes[j] = 1;
        }
    }
    if (!(M % 2)) M = M + 1;
    for (int i = M; i <= N; i=i+2) {
        if (primes[i] == 0) {
            printf("%d\n", i);
        }
    }

    return 0;
}
