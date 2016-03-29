/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sender.c
 * Author: student
 *
 * Created on 25 March 2016, 23:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
int ENTRY_NUMB;
int COUNTER;
/*
 * 
 */
void handler (int signo) {
    if(signo == SIGUSR1) {
        COUNTER = COUNTER + 1;
    }
    if(signo == SIGUSR2){
        printf("\n\n");
        printf("I sent to the Catcher [%d] signals, but he resent to my only [%d]!\n\n", ENTRY_NUMB, COUNTER);
        exit(0);
    }
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
    
    //====Saving sender's pid to the file================
    FILE *myfile;
    myfile = fopen("pidsender.txt","w");
    if (!myfile)
    {
        printf("Unable to open file!");
        return 1;
    }
    pid_t pidsender = getpid();
    char str1[10];

    sprintf(str1, "%d", pidsender);
    fwrite(str1, sizeof(char),numPlaces(pidsender) , myfile);
    fclose(myfile);
    //====================================================
    
    
    //====Reading catcher's pid from the file==============
    FILE *readfile;
    readfile = fopen("pidcatcher.txt", "r");
   
    if (!readfile)
    {
        printf("Unable to open file!");
        return 1;
    }
    
    pid_t pidcatcher;
    char str[10];
    fread(&str,sizeof(char),10,readfile);
    fclose(readfile); 
    pidcatcher= atoi(str);
    
    //======================================================
    
    
    if(signal(SIGUSR1, handler) == SIG_ERR)
        printf("\n SignalError.\n");
    if(signal(SIGUSR2, handler) == SIG_ERR)
        printf("\n SignalError.\n");
    
    
    for(int i = 0; i < ENTRY_NUMB; i++){
        kill(pidcatcher, SIGUSR1);
    }
    kill(pidcatcher, SIGUSR2);
    
    while(1){
        
    }
    
    return (EXIT_SUCCESS);
}

