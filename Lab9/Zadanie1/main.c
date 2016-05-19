#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define P 5
#define THINKTIME 10
#define EATTIME 10

typedef struct ThreadArgs {
    int ID;
    sem_t *butler;
    sem_t *leftFork;
    sem_t *rightFork;
} threadArgs;

void think(int ID)
{
    printf("Philosopher[%d] is thinking...\n", ID);
    sleep(rand() % THINKTIME);
}

void eat(int ID)
{
    printf("Philosopher[%d] is eating...\n", ID);
    sleep(rand() % EATTIME);
    printf("Philosopher[%d] has stopped eating.\n", ID);
}

void acquireSemaphore(sem_t *semaphore)
{
    if(sem_wait(semaphore) < 0) {
        perror("Could not acquire semaphore");
        exit(EXIT_FAILURE);
    }
}

void releaseSemaphore(sem_t *semaphore)
{
    if(sem_post(semaphore) < 0) {
        perror("Could not acquire semaphore");
        exit(EXIT_FAILURE);
    }
}

void *run(void *param)
{
    threadArgs *args;
    args = (threadArgs *) param;
    printf("Philosopher[%d] STARTED.\n", args->ID);

    while(1) {
        think(args->ID);

        acquireSemaphore(args->butler);
        acquireSemaphore(args->leftFork);
        printf("Philosopher[%d] took fork[%d]\n", args->ID, args->ID);
        acquireSemaphore(args->rightFork);
        printf("Philosopher[%d] took fork[%d]\n", args->ID, (args->ID+1)%P);

        eat(args->ID);
        
        releaseSemaphore(args->leftFork);
        printf("Philosopher[%d] put down fork[%d]\n", args->ID, args->ID);
        releaseSemaphore(args->rightFork);
        printf("Philosopher[%d] put down fork[%d]\n", args->ID, (args->ID+1)%P);
        releaseSemaphore(args->butler);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    int i;
    sem_t forks[P];
    sem_t butler;
    pthread_t threads[P];
    threadArgs args[P];
    srand(time(NULL));

    for(i = 0; i < P; i+=1) {
        sem_init(&forks[i], 0, 1);
    }
    sem_init(&butler, 0, P-1);
    
    for(i = 0; i < P; i+=1) { //creating philosophers
        args[i].ID = i+1;
        args[i].butler = &butler;
        args[i].leftFork = &forks[i];
        args[i].rightFork = &forks[(i+1)%P];
        pthread_create(&threads[i], NULL, run, &args[i]);
    }

    for(i = 0; i < P; i+=1) {
        pthread_join(threads[i], NULL);
    }

    return EXIT_SUCCESS;
}