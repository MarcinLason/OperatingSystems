#ifndef STRUCTURES_H
#define STRUCTURES_H
#define RSIZE 1024
#define BSIZE (RSIZE - sizeof(int))

typedef struct ThreadArgs {
    int fileDesc;
    size_t recordsNumber;
    char *text;
} threadArgs;

typedef struct RecordData {
    int ID;
    char text[BSIZE];
} recordData;

typedef union Record {
    char bytes[RSIZE];
    recordData data;
} record_t;

#endif