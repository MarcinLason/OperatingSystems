#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h> 
#include <sys/types.h>
#include <time.h>
#include <sys/times.h>
#include <linux/sched.h>
#include <stdlib.h>
#include <unistd.h>
#define N 150000
#define SIZE 4096

int counter = 0; 
clock_t startTime;
clock_t childStartTime;
clock_t summaryTime = 0;
struct tms CPU;

int fun(void * args) {
    childStartTime = clock();
    counter++;
    summaryTime += clock() - childStartTime;
    _exit(0);
}
int main() {
    clock_t startTime = clock(); 
    void * memory = malloc((N+1)*SIZE);
    void * s = (void *)(memory);

    for (int i = 0; i < N; i++) { 
        clone(&fun, s+((i+1)*SIZE), CLONE_VFORK| CLONE_VM ,0);
        wait();
    }
    
    if (times(&CPU) < 0) {
        perror("times error");
    }
    
    else {
        printf("Parent utime = %f\n", ((double) CPU.tms_utime)/CLOCKS_PER_SEC);
        printf("Child utime = %f\n", ((double) CPU.tms_cutime)/CLOCKS_PER_SEC);
        printf("Parent systime = %f\n", ((double) CPU.tms_stime)/CLOCKS_PER_SEC);
        printf("Child systime = %f\n", ((double) CPU.tms_cstime)/CLOCKS_PER_SEC);
    }
      
    printf("Parent real time = %ld clock ticks\n",(clock()-startTime));
    printf("Child real time = %ld clock ticks \n", summaryTime);
    printf( "global counter = %d\n", counter);
    return 0;
}