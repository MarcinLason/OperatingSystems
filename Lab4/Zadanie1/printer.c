/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Marcin Lasoń
 *
 * Created on 23 March 2016, 13:37
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int MAX; //max amount of repetitions * 3
int CURR_VALUE; 
char * TEXT; //original text, passed by user
int INCREASE; //flag thats says if we increase amount of repetitions or decrease

char * reverse (char * text) {
    int length = strlen(text);
    int halfALength = length/2;
    char temp;
    char * newText = (char *)malloc(length * sizeof(char));
    strcpy(newText, text);
    int i = 0;
    int k = 0;
    
    for(i = 0, k = (length - 1); i < halfALength; i++,k--)
    {
        temp = newText[k];
        newText[k] = newText[i];
        newText[i] = temp;
    }
    return newText;
}

void handler (int signo) {
    
    if(signo == SIGINT){
        printf("Odebrano sygnał SIGINT.\n");
        exit(0);
    }
    
    if(signo == SIGTSTP) {
        if(CURR_VALUE == MAX && INCREASE == 1) {
            INCREASE = 0;
            CURR_VALUE = MAX - 1;
            return;
        }
        
        if(CURR_VALUE == 0 && INCREASE == 0) {
            INCREASE = 1;
            CURR_VALUE = 1;
            return;
        }
        
        if(CURR_VALUE < MAX && INCREASE == 1){
            CURR_VALUE = CURR_VALUE + 1;
            return;
        }
        
        if(CURR_VALUE > 0 && INCREASE == 0) {
            CURR_VALUE = CURR_VALUE - 1;
            return;
        }     
    }
}


int main(int argc, char** argv) {
    
    if (argc != 3) {
        printf("You have to pass text argument and number of max number of repetitions.");
        return(0);
    }
    
    TEXT = argv[1];
    MAX = (3 * atoi(argv[2])) - 1;
    CURR_VALUE = 0;
    INCREASE = 1;
    
    struct sigaction action;
    action.sa_flags = NULL;
    sigfillset(&action.sa_mask);
    action.sa_sigaction = handler;
    sigaction(SIGINT, &action, NULL);
    
    int length;
    int modulo;
    
    //if(signal(SIGINT, handler) == SIG_ERR)
      //  printf("\n SignalError.\n");
    
    if(signal(SIGTSTP, handler) == SIG_ERR)
        printf("\n SignalError.\n");
    
    
    while(1){
        length = (CURR_VALUE/3) + 1;
        modulo = CURR_VALUE % 3;
        
        if(modulo == 1) {
            for(int i = 0; i < (length -1); i++){
                printf("%s", reverse(TEXT));
            }
            printf("%s\n", reverse(TEXT));
        }
        
        else {
            for(int i = 0; i < (length -1); i++){
                printf("%s", TEXT);
            }
            printf("%s\n", TEXT);
        }
    }
    return (EXIT_SUCCESS);
}

