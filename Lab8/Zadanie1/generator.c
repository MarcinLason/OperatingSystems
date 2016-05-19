#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"

int main(int argc, char* argv[])
{
    char buffer[BSIZE];
    int id;
    record_t record;
    FILE *file;

    if(argc != 2) {
        printf("Wrong number of arguments!\n");
        printf("You have to pass filename\n");
        exit(EXIT_FAILURE);
    }
    
    file = fopen(argv[1], "w+");
    if(file == NULL) {
        printf("Error while opening the file!\n");
        exit(EXIT_FAILURE);
    }
    
    id = 1;
    while(fgets(buffer, BSIZE, stdin)) {
        record.data.ID = id;
        strncpy(record.data.text, buffer, BSIZE);
        fwrite(record.bytes, RSIZE, 1, file);
        id += 1;
    }
    fclose(file);
}