/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: student
 *
 * Created on 12 April 2016, 18:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define SIZE 256

void uppercase(){
    char line[SIZE];
    char space[1];
    space[0]='\n';
    
    fputs(space, stdout);
    while(fgets(line, SIZE, stdin)) {
        for(int i = 0; i < strlen(line); i+=1) {
            line[i] = toupper(line[i]);
        }
        fputs(line, stdout);
    }
}


void fold(int N){
    char text[SIZE];
    int counter = 0;

    while(fgets(text, SIZE, stdin)) {
        int length = strlen(text);
        for(int i = 0; i < length; i+=1) {
            
            if(text[i] == '\n') {
                counter = 0;
            }
            if(counter > N) {
                fputc('\n', stdout);
                counter = 0;
            }
                 
            fputc(text[i], stdout);
            counter += 1;
        }
    }
}

int main(int argc, char* argv[])
{
    int length; //max length of the work in the line
    pid_t pid;
    int fdesc[2];
    
    if(argc != 2) {
        printf("You have to pass max number of signs in the single line!\n");
        return EXIT_FAILURE;
    }

    length = atoi(argv[1]);
    if(length > 80){
        printf("This number is too big! I set maximum value - 80.\n");
        length = 80;
    }

    if(pipe(fdesc) < 0) {
        printf("Pipe error!\n");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) < 0) {
        printf("Fork error!\n");
        exit(EXIT_FAILURE);    
    } 
    
    if(pid == 0) {
        close(fdesc[1]);
        if(dup2(fdesc[0], STDIN_FILENO) != STDIN_FILENO) {
            printf("Dup2 error\n");
            exit(EXIT_FAILURE);
        }
        close(fdesc[0]);  
        fold(length);
    }
    
    if(pid > 0) {
        close(fdesc[0]);
        if(dup2(fdesc[1], STDOUT_FILENO) != STDOUT_FILENO) {
            printf("Dup2 error!\n");
            exit(EXIT_FAILURE);
        }
        close(fdesc[1]);
        uppercase();    
    }
       
    return 0;
}