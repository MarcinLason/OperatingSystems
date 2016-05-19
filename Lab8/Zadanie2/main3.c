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
    threadSigArg *p;
    p = (threadSigArg *) parameters;
    signal(p->signal, handleSignal);
    printf("TID: %ld PID: %d\n", syscall(SYS_gettid), (int) getpid());
    
    while(1) {}
    return NULL;
}

void runThreads(int numberOfThreads, int signal)
{
    int i;
    threadSigArg args;
    args.signal = signal;
    threadsID = malloc(sizeof(pthread_t)*numberOfThreads);

    for(i = 0; i < numberOfThreads; i+=1) {
        pthread_create(&threadsID[i], NULL, readRecords, &args);
    }
    sleep(3);
    kill(getpid(), signal);
    printf("Signal %d sent!\n", signal);

    for(i = 0; i < numberOfThreads; i+=1) {
        pthread_join(threadsID[i], NULL);
    }
    free(threadsID);
}

int main(int argc, char* argv[])
{
    int recordsToRead;
    int fileDesc;
    int signal;
    int correctValue = 0;
    
    if(argc != 3) {
        printf("Wrong number of arguments!\n");
        printf("You have to pass: number of threads and signal.\n");
        exit(EXIT_FAILURE);
    }
    
    
    if(strcmp(argv[2], "SIGTERM") == 0) {
        signal = SIGTERM;
        correctValue = 1;
    }
    else if(strcmp(argv[2], "SIGKILL") == 0) {
        signal = SIGKILL;
        correctValue = 1;
    }
    else if(strcmp(argv[2], "SIGUSR1") == 0) {
        signal = SIGUSR1;
        correctValue = 1;
    }
    else if(strcmp(argv[2], "SIGSTOP") == 0) {
        signal = SIGSTOP;
        correctValue = 1;
    }
    
    if(correctValue == 0) {
        printf("You passed wrong signal name!\n");
        exit(EXIT_FAILURE);
    }
    printf("PID of the main process: %d\n", (int) getpid());
    numberOfThreads = atoi(argv[1]);
    runThreads(numberOfThreads, signal);
    return EXIT_SUCCESS;
}