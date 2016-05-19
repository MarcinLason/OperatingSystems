#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <signal.h>
#include "structures.h"

static int numberOfThreads;
static pthread_t *threadsID;
static pthread_key_t bufferKey;

void handleSignal(int signo)
{
    printf("Signal handler: TID: %ld PID: %d\n", syscall(SYS_gettid), (int) getpid());
}

void *readRecords(void *parameters)
{
    printf("TID: %ld PID: %d\n", syscall(SYS_gettid), (int) getpid());
    while(1) {}
    return NULL;
}

void *zeroOperation(void *parameters)
{
    int x;
    x = 11;
    x /= 0;
    return NULL;
}

void runThreads(int numberOfThreads)
{
    int i;
    threadsID = malloc(sizeof(pthread_t)*numberOfThreads);

    for(i = 0; i < (numberOfThreads-1); i+=1) {
        pthread_create(&threadsID[i], NULL, readRecords, NULL);
    }
    i+=1;
    pthread_create(&threadsID[i], NULL, zeroOperation, NULL);

    for(i = 0; i < numberOfThreads; i+=1) {
        pthread_join(threadsID[i], NULL);
    }
    free(threadsID);
}

int main(int argc, char* argv[])
{
    int recordsToRead;
    int fileDesc;

    if(argc != 2) {
        printf("Wrong number of arguments!\n");
        printf("You have to pass: number of threads.\n");
        exit(EXIT_FAILURE);
    }
    printf("PID of the main process: %d\n", (int) getpid());
    numberOfThreads = atoi(argv[1]);

    runThreads(numberOfThreads);
    return EXIT_SUCCESS;
}