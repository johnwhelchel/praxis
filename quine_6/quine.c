/********************************************************************** 

Program: quine
Author: John Whelchel
Date: 7/29/13

Description: 

This program takes no input and produces it's source as ouput. Thought
it was an interesting thought experiment and wanted to read more
about so I thought I'd do this one.

Usage:

% quine

**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[]) {
    FILE * fd;
    char line[100];
    fd = fopen("quine.c", "r");
    while(fgets(line, 99, fd) != NULL) {
        printf("%s", line);
    }
    return 0;
}
