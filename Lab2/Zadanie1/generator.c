/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



/* 
 * File:   generator.c
 * Author: Marcin Lasoń
 *
 * Created on 13 March 2016, 13:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void help() {
    printf("Welcome in the help sector.\n\n");
    printf("Correct way to start program:\n");
    printf("./generator [Name] [S] [N] \n\n");
    printf("Name - name of the file which will be generated.\n");
    printf("S - size of the single record.\n");
    printf("N - number of records which will be generated.");
}

int main(int argc, char** argv) {
    
    if(argc > 4) {
        printf("You passed too much arguments.\n");
        return 0; 
    }
    
    if(argc == 2 && strcmp(argv[1], "-h") == 0){
        help();
        return 0;
    }
    
    if(argc < 4) {
        printf("You passed not enough arguments.\n");
        return 0;
    }
    
    
    FILE * file;
    srand(time(NULL));
    
    int S = atoi(argv[2]); //size of the single record
    char records[S];
    int N = atoi(argv[3]); //number of records to generate
    
    file = fopen(argv[1],"w");
    if (!file)
    {
        perror ("Cannot open.");
        exit (1);
    }
    
    for (int i = 0 ; i < N; i++)
        for(int j = 0 ; j < S ; j++)
    {
        records[j] = '!' + (rand() % 90);
        fwrite(&records[j], 1, sizeof(char), file);
    }
    
    fclose(file);
    return 0;
}
