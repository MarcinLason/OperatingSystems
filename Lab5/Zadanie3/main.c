/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: student
 *
 * Created on 06 April 2016, 13:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 
 */
int main(int argc, char** argv) {
    int length = 256;
    char text[length];
    
    if(argc > 2) {
        printf("You passed too much arguments.\n");
        return 0;
    }
    
    if(argc == 2) {
        freopen(argv[1], "w", stdout);
    }
    
    FILE * file1 = popen("ls -l", "r");
    FILE * file2 = popen("grep ^d", "w");
      
    while (fgets(text, length, file1)){
        fputs(text, file2);
    }  
    
    pclose(file1);
    pclose(file2);
    return 0;
}

