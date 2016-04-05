/* 
 * File:   sender.c
 * Author: Marcin Lasoń
 *
 * Created on 25 March 2016, 23:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
int ENTRY_NUMB;
int COUNTER;

void handlerOne (int signo) { //handler for (SIGRTMIN + 3) signals
    COUNTER = COUNTER + 1;
}
void handlerTwo(int signo) { //handler for (SIGRTMIN + 4) signals
    printf("\n\n");
    printf("I sent to the Catcher [%d] signals and he resent to me [%d]!\n\n", ENTRY_NUMB, COUNTER);
    exit(0);
}

int numPlaces (int n) {
   if (n < 10) return 1;
    return 1 + numPlaces (n / 10);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("You have to pass how many signals you want to send.");
        return(0);
    }
    ENTRY_NUMB = atoi(argv[1]);
    COUNTER = 0;
    pid_t pidsender; //pid of the sender
    pid_t pidcatcher; //pid of the catcher
    
    //====Saving sender's pid to the file================
    FILE *myfile;
    myfile = fopen("pidsender.txt","w");
    if (!myfile)
    {
        printf("Unable to open file!");
        return 1;
    }
    pidsender = getpid();
    char str1[10];

    sprintf(str1, "%d", pidsender);
    fwrite(str1, sizeof(char),numPlaces(pidsender) , myfile);
    fclose(myfile);

    //====Reading catcher's pid from the file==============
    FILE *readfile;
    readfile = fopen("pidcatcher.txt", "r");
   
    if (!readfile)
    {
        printf("Unable to open file!");
        return 1;
    }
    
    char str[10];
    fread(&str,sizeof(char),10,readfile);
    fclose(readfile); 
    pidcatcher= atoi(str);
    //======================================================
    
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
    
    union sigval value;
    
    for(int i = 0; i < ENTRY_NUMB; i++){
        if(sigqueue(pidcatcher, (SIGRTMIN + 3), value) < 0) {
            fputs("Sigqueue error", stderr);
            exit(EXIT_FAILURE);
        }
    }
     
    if(sigqueue(pidcatcher, (SIGRTMIN + 4), value) < 0) {
            fputs("Sigqueue error", stderr);
            exit(EXIT_FAILURE);
        }
    
    while(1){
        pause();
    }
    
    return (EXIT_SUCCESS);
}

