#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include "functions.h"

void getNumber(int *memAddr)
{
    int pointer = memAddr[CONSINDEX];
    time_t currTime;
    char *timestamp;

    memAddr[CONSINDEX] = (pointer + 1) % ARRAYSIZE;
    memAddr[FILLINDEX] -= 1;

    time(&currTime);
    timestamp = malloc(sizeof(char)*16);
    strftime(timestamp, 15, "%H:%M:%S", localtime(&currTime));

    if(memAddr[pointer] % 2 == 0) {
        printf("(%d %s) Sprawdzilem liczbe %d na pozycji %d - parzysta. Pozostalo zadan oczekujacych: %d\n",
               getpid(), timestamp, memAddr[pointer], pointer, memAddr[FILLINDEX]);
    } else {
        printf("(%d %s) Sprawdzilem liczbe %d na pozycji %d - nieparzysta. Pozostalo zadan oczekujacych: %d\n",
               getpid(), timestamp, memAddr[pointer], pointer, memAddr[FILLINDEX]);
    }
    free(timestamp);
}

void consume(int *memAddr, int semID)
{
    while(1) {
        getSemaphore(semID);
        if(memAddr[FILLINDEX] > 0) {
            getNumber(memAddr);
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
        printf("You have to pass pathname and ID!\n");
        exit(EXIT_FAILURE);
    }

    key_t key = ftok(argv[1], atoi(argv[2]));
    if(key < 0) {
        printf("Error while creating a key!\n");
        exit(EXIT_FAILURE);
    }
    memAddr = getMemory(key, 0);

    if((semID = semget(key, SEMVALUE, 0)) == -1) {
        printf("Error while getting semID!\n");
        exit(EXIT_FAILURE);
    }

    consume(memAddr, semID);
    return EXIT_SUCCESS;
}
