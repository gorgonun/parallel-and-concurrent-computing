#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if(argc < 3){
        printf("Usage: %s <text file> <word1> <word2> ... <wordn>", argv[0]);
        return 1;
    }

    int occurences[argc - 2];

    for(int i = 0; i < argc - 1; i++) {
        occurences[i] = 0;
    }

    // it assumes no word exceeds length of 1023
    char word[1024];

    char * buffer = 0;
    long length;
    FILE * f = fopen (argv[1], "rb");

    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(length + 1);
    fread(buffer, 1, length, f);
    buffer[length] = 0;
    fclose(f);

    // this is a very expensive operation that iterates
    // over the text argc - 2 times
    // it is not to be fast is just to compare this
    // version with the openmp version
    for(int i = 2; i < argc; i++) {
        int offset;
        char *ptr = buffer;
        while(sscanf(ptr, "%1023s%n", word, &offset) == 1) {
            ptr += offset;
            if(strcmp(argv[i], word) == 0) {
                occurences[i - 2]++;
            }
        }
    }
    
    for(int i = 2; i < argc; i++) {
        printf("%s -> %i times\n", argv[i], occurences[i - 2]);
    }
}
