#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include "functions.h"

void putNumber(int *memoryAddr)
{
    char *timestamp;
    int pointer = memoryAddr[PRODINDEX];
    int producingNumber = rand() % 5000;
    time_t currTime;
    
    memoryAddr[pointer] = producingNumber;
    memoryAddr[PRODINDEX] = (pointer + 1) % ARRAYSIZE;
    memoryAddr[FILLINDEX] += 1;

    time(&currTime);
    timestamp = malloc(sizeof(char)*16);
    strftime(timestamp, 15, "%H:%M:%S", localtime(&currTime));
    printf("[%d %s] Dodalem %d na pozycje %d. Liczba zadan oczekujacych: %d\n", getpid(), timestamp, producingNumber, pointer, memoryAddr[FILLINDEX]);
    free(timestamp);
}

void produce(int *memoryAddr, int semID)
{
    while(1) {
        getSemaphore(semID);
        if(memoryAddr[FILLINDEX] < ARRAYSIZE) {
            putNumber(memoryAddr);
        }
        releaseSemaphore(semID);
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    int *memAddr;
    int semID;

    if(argc != 3) {
        printf("You have to pass pathname and id!\n");
        exit(EXIT_FAILURE);
    }

    key_t key = ftok(argv[1], atoi(argv[2]));
    if(key < 0) {
        printf("Error while creating a key!\n");
        exit(EXIT_FAILURE);
    }
    
    memAddr = getMemory(key, 0);
    if((semID = semget(key, SEMVALUE, 0)) == -1) {
        printf("Error while getting the semaphoreID!\n");
        exit(EXIT_FAILURE);
    }

    produce(memAddr, semID);
    return EXIT_SUCCESS;
}
