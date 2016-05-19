#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "functions.h"

void checkArray(int number, int *memoryAddr)
{
    char *timestamp;
    int i;
    int counter = 0;
    time_t currTime;

    for(i=0; i<ARRAYSIZE; i+=1) {
        if(memoryAddr[i] == number) {
            counter++;
        }
    }

    time(&currTime);
    timestamp = malloc(sizeof(char)*16);
    strftime(timestamp, 15, "%H:%M:%S", localtime(&currTime));
    printf("(%d %s) Znalazlem %d liczb o wartosci %d\n", getpid(), timestamp, counter, number);
    free(timestamp);
}

void checkMemory(int number, int *memoryAddr, sem_t* reader, sem_t* writer)
{
    while(1) {
        getSemaphore(reader);
        memoryAddr[COUNTERINDEX] += 1;
        
        if(memoryAddr[COUNTERINDEX] == 1) {
            getSemaphore(writer);
        }
        releaseSemaphore(reader);
        checkArray(number, memoryAddr);
        getSemaphore(reader);
        memoryAddr[COUNTERINDEX] -= 1;
        
        if(memoryAddr[COUNTERINDEX] == 0) {
            releaseSemaphore(writer);
        }
        releaseSemaphore(reader);
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    int *memoryAddr;
    int number;
    sem_t *rSemaphoreDesc;
    sem_t *wSemaphoreDesc;

    if(argc != 3) {
        printf("You have to pass the name of shared memory and number to find!\n");
        exit(EXIT_FAILURE);
    }

    memoryAddr = getMemory(argv[1], PROT_READ | PROT_WRITE);
    number = atoi(argv[2]);
    rSemaphoreDesc = openSemaphore("/reader");
    wSemaphoreDesc = openSemaphore("/writer");

    checkMemory(number, memoryAddr, rSemaphoreDesc, wSemaphoreDesc);
    sem_close(rSemaphoreDesc);
    sem_close(wSemaphoreDesc);
    return EXIT_SUCCESS;
}
