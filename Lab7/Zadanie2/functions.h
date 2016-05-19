#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MEMSIZE 30
#define ARRAYSIZE (MEMSIZE-1)
#define COUNTERINDEX (MEMSIZE-1)

int* getMemory(char *name, int flags);
sem_t* openSemaphore(char *name);
void getSemaphore(sem_t *semaphore);
void releaseSemaphore(sem_t *semaphore);