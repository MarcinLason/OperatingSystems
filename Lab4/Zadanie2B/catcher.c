/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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

int COUNTER;
pid_t pidsender;

void handler (int signo) {
    
    if(signo == SIGUSR1){
        
        if(COUNTER != 0) 
        {
            COUNTER = COUNTER + 1;
            kill(pidsender,SIGUSR1);
        }
        
        if(COUNTER == 0) {
            FILE *readfile;
            readfile = fopen("pidsender.txt", "r");
   
            if (!readfile)
            {
                printf("Unable to open file!");
                return 1;
            }
            char str[10];
            fread(&str,sizeof(char),10,readfile);
            fclose(readfile); 
            pidsender = atoi(str);
        
            COUNTER = COUNTER + 1;
            kill(pidsender,SIGUSR1);
        }
              
    }
    
    if(signo == SIGUSR2) {
        printf("\n\n");
        printf("Catcher: I received [%d] SIGUSR1 signals from sender.\n", COUNTER);
        kill(pidsender, SIGUSR2);
        
        printf("Catcher: Goodbye.\n\n");
        exit(0);
    }
}

int numPlaces (int n) {
   if (n < 10) return 1;
    return 1 + numPlaces (n / 10);
}

/*
 * 
 */
int main(int argc, char** argv) {
    COUNTER = 0;
    
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
    //=======================================================
    
    if(signal(SIGUSR1, handler) == SIG_ERR)
        printf("\n SignalError.\n");
    if(signal(SIGUSR2, handler) == SIG_ERR)
        printf("\n SignalError.\n");
    
    while(1) {    
    
    } 
    
    return (EXIT_SUCCESS);
}

