#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "functions.h"

void putNumbers(int *memoryAddr, sem_t *semaphoreDesc)
{
    while(1) {
        getSemaphore(semaphoreDesc);
        char *timestamp;
        int i;
        int number;
        int counter;
        int index;
        time_t currTime;

        timestamp = malloc(sizeof(char)*16);
        counter = rand() % ARRAYSIZE;
        number = rand() % 15;

        for(i=0; i<counter; i+=1) {
            index = rand() % ARRAYSIZE;
            memoryAddr[index] = number + i;

            time(&currTime);
            strftime(timestamp, 15, "%H:%M:%S", localtime(&currTime));
            printf("(%d %s) Wpisalem liczbe %d na pozycje %d. Pozostalo %d zadan.\n", getpid(), timestamp, number + i, index, counter-i-1);
        }
        free(timestamp); 
        releaseSemaphore(semaphoreDesc);
        sleep(3);
    }
}

int main(int argc, char *argv[])
{
    int *memoryAddr;
    sem_t *semaphoreDesc;

    if(argc != 2) {
        printf("You have to pass the name of the shared memory!\n");
        exit(EXIT_FAILURE);
    }

    memoryAddr = getMemory(argv[1], PROT_READ | PROT_WRITE);
    semaphoreDesc = openSemaphore("/writer");
    putNumbers(memoryAddr, semaphoreDesc);
    sem_close(semaphoreDesc);
    return EXIT_SUCCESS;
}