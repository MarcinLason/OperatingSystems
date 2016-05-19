#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include "functions.h"

int main(int argc, char* argv[])
{
    int *memAddr;
    int semID;
    char sign = 'x';

    if(argc != 3) {
        printf("You have to pass path and id!\n");
        exit(EXIT_FAILURE);
    }

    key_t key = ftok(argv[1], atoi(argv[2]));
    if(key < 0) {
        printf("Error while creating a key!\n");
        exit(EXIT_FAILURE);
    }
    printf("Created key: %d \n", key);
    printf("Entry 'q' when you want to quit\n");
    
    memAddr = getMemory(key, 0666 | IPC_CREAT);
    if((semID = semget(key, SEMVALUE, 0666 | IPC_CREAT)) == -1) {
        printf("Error while creating semaphore!\n");
        exit(EXIT_FAILURE);
    }
    memAddr[FILLINDEX] = 0;
    memAddr[CONSINDEX] = 0;
    memAddr[PRODINDEX] = 0;
       
    while(sign != 'q') {
        scanf("%c", &sign);
    }
    return EXIT_SUCCESS;
}
