#include <sys/ipc.h>
#define MEMSIZE 10
#define ARRAYSIZE (MEMSIZE-3)
#define FILLINDEX (MEMSIZE-1)
#define PRODINDEX (MEMSIZE-2)
#define CONSINDEX (MEMSIZE-3)
#define SEMVALUE ARRAYSIZE

int* getMemory(key_t key, int flags);
void getSemaphore(int semID);
void releaseSemaphore(int semID);
