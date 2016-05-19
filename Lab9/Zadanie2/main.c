#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#define FLY 4
#define STOP 3

typedef struct ThreadArgs {
    int ID;
} threadArg;

static pthread_mutex_t monitor = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t landingCondition = PTHREAD_COND_INITIALIZER;
static pthread_cond_t startingCondition = PTHREAD_COND_INITIALIZER;

static volatile int airplanesInDock;
static volatile int N;
static volatile int K;
static volatile int toStart;
static volatile int toLand;
static volatile bool isAirstripFree = true;

void lock(pthread_mutex_t *mutex)
{
    if(pthread_mutex_lock(mutex) < 0) {
        printf("Error in lock function!\n");
        exit(EXIT_FAILURE);
    }
}

void unlock(pthread_mutex_t *mutex)
{
    if(pthread_mutex_unlock(mutex) < 0) {
        printf("Error in unlock function!\n");
        exit(EXIT_FAILURE);
    }
}

void signal(pthread_cond_t *cond)
{
    if(pthread_cond_signal(cond) < 0) {
        printf("Error in signal function!\n");
        exit(EXIT_FAILURE);
    }
}

void wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    if(pthread_cond_wait(cond, mutex) < 0) {
        printf("Error in wait function!\n");
        exit(EXIT_FAILURE);
    }
}

void notifyWaiters()
{
    if(airplanesInDock < K) {
        if(toLand > 0) {
            signal(&landingCondition);
        } else {
            signal(&startingCondition);
        }
    } else {
        if(toStart > 0) {
            signal(&startingCondition);
        } else if(airplanesInDock < N) {
            signal(&landingCondition);
        }
    }
}

void requestStart(int ID)
{
    lock(&monitor);
    printf("Airplane[%d] requests start.\n", ID);

    while(!isAirstripFree) {
        toStart += 1;
        wait(&startingCondition, &monitor);
        toStart -= 1;
    }
    isAirstripFree = false;
    unlock(&monitor);
}

void start(int ID)
{
    lock(&monitor);
    
    printf("Airplane[%d] is starting\n", ID);
    airplanesInDock -= 1;
    isAirstripFree = true;
    
    printf("Airplane[%d] is flying.\n", ID);
    notifyWaiters();
    unlock(&monitor);
}

void requestLanding(int ID)
{
    lock(&monitor);
    printf("Airplane[%d] requests landing.\n", ID);

    while(!isAirstripFree || airplanesInDock == (N-1)) {
        toLand += 1;
        wait(&landingCondition, &monitor);
        toLand -= 1;
    }
    isAirstripFree = false;
    unlock(&monitor);
}

void land(int ID)
{
    lock(&monitor);
    
    printf("Airplane[%d] is landing.\n", ID);
    airplanesInDock += 1;
    isAirstripFree = true;
    
    printf("Airplane[%d] is waiting.\n", ID);
    notifyWaiters();
    unlock(&monitor);
}

void fly(int ID)
{
    sleep(rand() % FLY);
}

void await(int ID)
{
    sleep(rand() % STOP);
}

void *runAirplane(void *parameters)
{
    threadArg *args;
    args = (threadArg *) parameters;
    
    while(1) {
        requestStart(args->ID);
        start(args->ID);
        fly(args->ID);
        requestLanding(args->ID);
        land(args->ID);
        await(args->ID);
    }
    return NULL;
}

void runAircraft(int amountOfAirplanes)
{
    int i;
    char sign = 'x';
    pthread_t *threadsID;
    threadArg *args;
    
    threadsID = malloc(sizeof(pthread_t)*amountOfAirplanes);
    args = malloc(sizeof(threadArg)*amountOfAirplanes);
    airplanesInDock = amountOfAirplanes;

    for(i = 0; i < amountOfAirplanes; i+=1) {
        args[i].ID = i+1;
        pthread_create(&threadsID[i], NULL, runAirplane, &args[i]);
    }

    while(sign != 'q') {
        scanf(" %c", &sign);
    }

    for(i = 0; i < amountOfAirplanes; i+=1) {
        pthread_cancel(threadsID[i]);
    }

    free(threadsID);
    free(args);
}

int main(int argc, char* argv[])
{
    if(argc != 4) {
        printf("Error! You have to pass: N, K, amount of airplanes.\n");
        printf("N - capacity of aircraft's hangar.\n");
        printf("K - border value when currently flying airplanes get higher priority.\n");
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    K = atoi(argv[2]);
    int amountOfAirplanes = atoi(argv[3]);

    runAircraft(amountOfAirplanes);
    return 0;
}