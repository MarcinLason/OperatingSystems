/* 
 * File:   catcher.c
 * Author: Marcin Lasoń
 *  *
 * Created on 25 March 2016, 23:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

int COUNTER;
int RECEIVING;
void handlerOne (int signo) {
    COUNTER = COUNTER + 1;
}
void handlerTwo(int signo) {
        printf("\n\n");
        printf("Catcher: I received [%d] SIGUSR1 signals from sender.\n", COUNTER);
        printf("Catcher: Now I will resend all of the signals.\n");
        RECEIVING = 0;  
}

int numPlaces (int n) {
   if (n < 10) return 1;
    return 1 + numPlaces (n / 10);
}

int main(int argc, char** argv) {
    COUNTER = 0;
    RECEIVING = 1;
    
    //====Saving catcher's pid to the file==================
    FILE *myfile;
  
    myfile = fopen("pidcatcher.txt","w");
    if (!myfile)
    {
        printf("Unable to open file!");
        return 1;
    }
    pid_t pidcatcher = getpid();
    char str[10];

    sprintf(str, "%d", pidcatcher);
    fwrite(str, sizeof(char),numPlaces(pidcatcher) , myfile);
    fclose(myfile);
    
    
    struct sigaction first_action, second_action;
    first_action.sa_sigaction = &handlerOne;
    first_action.sa_flags = SA_SIGINFO;

    if(sigaction((SIGRTMIN + 3), &first_action, NULL) < 0) {
        fputs("An error occured while setting SIGUSR2 signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
    
    second_action.sa_sigaction = &handlerTwo;
    second_action.sa_flags = SA_SIGINFO;
    
    if(sigaction((SIGRTMIN + 4), &second_action, NULL) < 0) {
        fputs("An error occured while setting SIGUSR2 signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
    
   
    while(RECEIVING) {    
        pause();  
    } 
    
    FILE *readfile;
        readfile = fopen("pidsender.txt", "r");
   
        if (!readfile)
        {
            printf("Unable to open file!");
            return 1;
        }
    
        pid_t pidsender;
        char str2[10];
        fread(&str2,sizeof(char),10,readfile);
        fclose(readfile); 
        pidsender = atoi(str2);
        union sigval value;
        
        for(int i = 0; i < COUNTER; i++) {
            if(sigqueue(pidsender, (SIGRTMIN + 3), value) < 0) {
            fputs("Kill error", stderr);
            exit(EXIT_FAILURE);
            }
        }
        
        if(sigqueue(pidsender, (SIGRTMIN + 4), value) < 0) {
            fputs("Kill error", stderr);
            exit(EXIT_FAILURE);
        }
        printf("Catcher: Signals were sent. Goodbye.\n\n");
        exit(0);
    
    return (EXIT_SUCCESS);
}

