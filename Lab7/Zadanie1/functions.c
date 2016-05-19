#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "functions.h"

static int memoryID;
void removeMemory()
{
    if(shmctl(memoryID, IPC_RMID, 0) == -1) {
        printf("Error while removing the memory!\n");
        exit(EXIT_FAILURE);
    }
}

int* getMemory(key_t key, int flags)
{
    int *memoryAddr;
    memoryID = shmget(key, MEMSIZE, flags);

    if(memoryID < 0) {
        printf("Shmget error while getting the memory ID!\n");
        exit(EXIT_FAILURE);
    }

    memoryAddr = shmat(memoryID, NULL, 0);
    if(memoryAddr == NULL) {
        printf("Shmat error while getting the memory address!\n");
        exit(EXIT_FAILURE);
    }

    if(flags & IPC_CREAT) {
        atexit(removeMemory);
    }
    return memoryAddr;
}

void getSemaphore(int semID)
{
    struct sembuf semOpperations[2];
    semOpperations[0].sem_num = 0; 
    semOpperations[0].sem_op = 0;
    semOpperations[0].sem_flg = 0;
    semOpperations[1].sem_num = 0;
    semOpperations[1].sem_op = 1;
    semOpperations[1].sem_flg = 0;

    if (semop(semID, semOpperations, 2) == -1) {
        printf("Error while getting the semaphore!\n");
        exit(EXIT_FAILURE);
    }
}

void releaseSemaphore(int semID)
{
    struct sembuf semOperations[1];
    semOperations[0].sem_num = 0;        
    semOperations[0].sem_op = -1;        
    semOperations[0].sem_flg = 0;
    if (semop(semID, semOperations, 1) == -1) {
        printf("Error while releasing the semaphore!\n");
        exit(EXIT_FAILURE);
    }
}