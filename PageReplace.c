#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "simulate.c"

int main(int argc, char *argv[]) {

    FILE *file; 

    // Checks for correct input of pages
    if(sscanf(argv[2], "%i", &max) == 0 || max == 0) {
        printf("Error: Page counts can't be 0.\n");
        exit(1);
    }

    // Checks to see if the file is valid
    if((file = fopen(argv[3], "r")) == NULL) {
        fprintf(stderr, "Error: fopen %d (%s)\n", errno, strerror(errno));
        exit(1);
    }

    // Checks to see what kind of algorithm was inputted
    switch(argv[1][0]) {

        case 'F':

            if (strcmp("FIFO", argv[1]) == 0) {
                algorithm = 1;
                algName = argv[1];
                break;
            }

        case 'L':

            if (strcmp("LRU", argv[1]) == 0) {
                algorithm = 2;
                algName = argv[1];
                break;
            }

        default:
            printf("Error: input is incorrect.\n");
            exit(1);
    }

    handlePageRef(file);

    while(head != NULL) {
        if((*head).dPage) {
            writeBackTime += 10;
        }

        struct node *oldHead = head;
        head = (*oldHead).next;

        if(head != NULL) {
            (*head).prev = NULL;
        }

        free(oldHead);
        count--;

    }

    fclose(file);
    printf("Total Page Ref: %u\n", totalPageR);
    printf("Total Page Miss: %u\n", pageMiss);
    printf("Total Page miss (Time): %u\n", pageMissTime);
    printf("Total Write back (Time): %u\n", totalWBT);
}