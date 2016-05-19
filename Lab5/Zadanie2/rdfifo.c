/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   rdfifo.c
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
    int SIZE = 1000; //size of the buffer
    char buffer[SIZE];
    int messageLength; //handler for received messages
    int handler; //handler for opened FIFO
    char * passedName = (char*)malloc(strlen(argv[1])); //passed name of the FIFO
    
    if(argc != 2) {
        printf("You have to pass name of the FIFO which will be created.");
        exit(EXIT_FAILURE);
    }
    
    
    printf("To close program please use CTRL + Z\n\n");
    strcpy(passedName, argv[1]);
    unlink(passedName); 
    
   
    
    //making FIFO:
    if(mkfifo(passedName, S_IWUSR| S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) {
        printf("Error during creating a FIFO.\n");
        exit(EXIT_FAILURE);
    }
    
    if(signal(SIGTSTP, sigHandler) == SIG_ERR) {
        printf("\n SignalError.\n");
    }
    
    
    
    handler = open(passedName, O_RDONLY);
    while (1) {
        messageLength = read(handler, buffer, SIZE);
        
        if(messageLength > 0) {
            char timeBuffer[15];
            time_t receivedTime;
            time(&receivedTime);

            strftime(timeBuffer, 15, "%H:%M:%S", localtime(&receivedTime));
            printf("%s: %s", timeBuffer, buffer);
        }
        memset(buffer, 0, sizeof(buffer)); //setting 0 on the whole buffer memory
    }
    
    close(handler);
    unlink(passedName);
    free(passedName);
    return (EXIT_SUCCESS);
}

