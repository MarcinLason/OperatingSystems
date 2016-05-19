/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   wrfifo.c
 * Author: student
 *
 * Created on 11 April 2016, 12:39
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#define BUFFERSIZE 1000
#define PIPESIZE 1000
/*
 * 
 */
void sigHandler (int signo) {
    if(signo == SIGTSTP){
        printf("\nProgram finished work successfully.\n");
        exit(0);
    }
}

int main(int argc, char** argv) {
    char pipe_buffer[PIPESIZE];
    char buffer[BUFFERSIZE];
    int handler;
    char * passedName = (char*)malloc(strlen(argv[1])); //passed name of the FIFO
    
    if(argc != 2) {
        printf("You have to pass name of the FIFO created by rdfifo program.");
        exit(EXIT_FAILURE);
    }
    
    if(signal(SIGTSTP, sigHandler) == SIG_ERR) {
        printf("\n SignalError.\n");
    }
    
    
    printf("To close program please use CTRL + Z\n\n");
    strcpy(passedName, argv[1]);
    
    
    
    handler = open(passedName, O_WRONLY);
    if(handler < 0) {
        printf("FIFO open error\n");
        exit(EXIT_FAILURE);
    }
    
    
    
    while(fgets(buffer, BUFFERSIZE, stdin)) {
        char timeBuffer[15];
        time_t messageTime;
        pid_t pid = getpid();

        time(&messageTime);
        
        strftime(timeBuffer,15, "%H:%M:%S", localtime(&messageTime));
        sprintf(pipe_buffer, "PID: %d, %s - %s", pid, timeBuffer, buffer);
        write(handler, pipe_buffer, PIPESIZE);
    }
    
    close(handler);
    free(passedName);
    return (EXIT_SUCCESS);
}

