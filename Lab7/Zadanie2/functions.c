#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"

static int *memoryAddr;
void unmap_memory()
{
    munmap(memoryAddr, MEMSIZE);
}

int* getMemory(char *memoryName, int flags)
{
    int memoryDesc;
    memoryDesc = shm_open(memoryName, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if(memoryDesc == -1) {
        printf("Shm_open error!\n");
        exit(EXIT_FAILURE);
    }

    memoryAddr = mmap(NULL, MEMSIZE, flags, MAP_SHARED, memoryDesc, 0);
    if(memoryAddr == NULL) {
        printf("Mmap error!\n");
        exit(EXIT_FAILURE);
    }

    atexit(unmap_memory);
    return memoryAddr;
}

sem_t* openSemaphore(char* name)
{
    sem_t *semaphore;
    semaphore = sem_open(name, O_RDWR);

    if(semaphore == SEM_FAILED){
        printf("Sem_open error!\n");
        exit(EXIT_FAILURE);
    }
    return semaphore;
}

void getSemaphore(sem_t *semaphore)
{
    if(sem_wait(semaphore)) {
        printf("Semaphore error!\n");
        exit(EXIT_FAILURE);
    }
}

void releaseSemaphore(sem_t *semaphore)
{
    if(sem_post(semaphore) == -1) {
        printf("Semaphore error!\n");
        exit(EXIT_FAILURE);
    }
}