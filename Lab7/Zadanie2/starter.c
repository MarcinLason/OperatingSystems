#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "functions.h"

int main(int argc, char* argv[])
{
    int memoryDesc;
    sem_t *wSemaphore;
    sem_t *rSemaphore;
    char sign = 'x';

    if(argc != 2) {
        printf("You have to pass name of the shared memory!\n");
        exit(EXIT_FAILURE);
    }

    memoryDesc = shm_open(argv[1], O_RDWR|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(memoryDesc == -1) {
        printf("Shm_open error!\n");
        exit(EXIT_FAILURE);
    }

    if(ftruncate(memoryDesc, MEMSIZE) == -1) {
        printf("Ftruncate error!\n");
        exit(EXIT_FAILURE);
    }

    rSemaphore = sem_open("/reader", O_RDWR|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 1);
    if(rSemaphore == SEM_FAILED){
        printf("Error while opening reader semaphore!\n");
        exit(EXIT_FAILURE);
    }

    wSemaphore = sem_open("/writer", O_RDWR|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 1);
    if(wSemaphore == SEM_FAILED){
        printf("Error while opening writer's semaphore!\n");
        exit(EXIT_FAILURE);
    }

    printf("Entry 'q' when you want to close program.\n");
    while(sign != 'q') {
        scanf(" %c", &sign);
    }

    close(memoryDesc);
    sem_close(rSemaphore);
    sem_close(wSemaphore);
    sem_unlink("/reader");
    sem_unlink("/writer");
    return EXIT_SUCCESS;
}