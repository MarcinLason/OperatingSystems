#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <stdio.h>
#include <stdlib.h>
#define N 150000

int counter = 0; 

int main (void){
clock_t startTime = clock(); 
pid_t pid;
clock_t childStartTime;
clock_t summaryTime = 0;
struct tms CPU;

for(int i=0; i < N; i++){    
    if((pid = vfork()) < 0){
        printf("Vfork error.");
    }

    if (pid == 0) {
        childStartTime = clock();
        counter++;
        summaryTime += clock()-childStartTime;
        _exit(0);
    }
    
    else{
        wait(0);
    }
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
printf("Child real time = %ld clock ticks \n",summaryTime);
printf("PID = %d, global counter = %d \n", getpid(), counter);
return 0;
}