#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if(argc < 3){
        printf("Usage: %s <text file> <word1> <word2> ... <wordn>", argv[0]);
        return 1;
    }

    // it assumes no word exceeds length of 1023
    char word[1024];
    int occurences[argc - 2];

    // this for does not need parallelism
    for(int i = 0; i < argc - 1; i++) {
        occurences[i] = 0;
    }

    // this is a very expensive operation that iterates
    // over the text argc - 2 times
    // it is not to be fast is just to compare this
    // version with the openmp version
    #pragma omp parallel for private(word)
    for(int i = 2; i < argc; i++) {
        FILE *file = fopen(argv[1], "r");
    
        while (fscanf(file, " %1023s", word) == 1) {
            if(strcmp(argv[i], word) == 0) {
                occurences[i - 2]++;
            }
        }
    }
    
    for(int i = 2; i < argc; i++) {
        printf("%s -> %i times\n", argv[i], occurences[i - 2]);
    }
}
