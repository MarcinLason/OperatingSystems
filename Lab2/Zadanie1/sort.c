/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sort.c
 * Author: Marcin Lasoń
 *
 * Created on 13 March 2016, 21:41
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h> 
#include <sys/resource.h>

void help() {
    printf("Welcome in the help sector.\n\n");
    printf("Correct way to start program:\n");
    printf("./sort [Name] [S] [Type] \n\n");
    printf("Name - name of the file which will be generated.\n");
    printf("S - size of the single record.\n");
    printf("Type - type of functions which should be used to sort records.\n");
}

int libSort(int recordsLength, char * name){
    FILE * file;
    char compare[recordsLength];
    char current[recordsLength] ; // current element that we are placing  
    int recordsNumber;
    
  
    file = fopen(name, "r+");
    if (!file)
    {
        perror ("Cannot open the file.");
        exit (1);
    }
    
    
    fseek(file, 0, SEEK_END); 
    recordsNumber = ftell(file)/recordsLength;
    fseek(file, 0, SEEK_SET);
      
    
    for(int i = 1; i < recordsNumber; i++ ){
        fseek(file, -(i+1) * recordsLength, SEEK_END );
        fread(&current,recordsLength,1,file); 
        fread(&compare,recordsLength, 1, file);
        fseek(file, -recordsLength, SEEK_CUR );
       
        while(ftell(file)< recordsNumber * recordsLength && current[0] > compare[0]){
            fseek(file, -recordsLength , SEEK_CUR);
            fwrite(&compare, 1, sizeof(char) * recordsLength, file);
            fseek(file, recordsLength , SEEK_CUR);
             
            if(ftell(file) < recordsNumber * recordsLength){
                fread(&compare, recordsLength, 1, file);
                fseek(file, -recordsLength, SEEK_CUR );
            }else break;      
        }   
        fseek(file, -recordsLength , SEEK_CUR);
        fwrite(&current,1, sizeof(char) * recordsLength, file);
    }
    fclose(file); 
    return 0;
}

int sysSort(int recordsLength, char * name){
    int recordsNumber;
    int file = open(name, O_CREAT | O_RDWR, S_IRUSR);    
    if (!file)
    {
        perror ("Cannot open the file.");
        exit (1);
    }
    
    
    char * y =malloc(sizeof(char)*recordsLength) ;
    char * cmp=malloc(sizeof(char)*recordsLength) ;
    recordsNumber = lseek(file, 0, SEEK_END)/recordsLength;
    lseek(file, 0, SEEK_SET);
      
    for(int i=1 ; i < recordsNumber ; i++ ){
        lseek(file, -(i+1)* recordsLength, SEEK_END );
        read(file,y, sizeof(char)* recordsLength); 
        read(file, cmp, recordsLength* sizeof(char));
        lseek(file, -recordsLength, SEEK_CUR );
       
        while(lseek(file, 0, SEEK_CUR)< recordsNumber * recordsLength && y[0] > cmp[0]){
            lseek(file, -recordsLength , SEEK_CUR);
            write(file,cmp,recordsLength* sizeof(char));
            lseek(file, recordsLength , SEEK_CUR);
             
            if(lseek(file, 0, SEEK_CUR)< recordsNumber * recordsLength){
                read(file, cmp, recordsLength * sizeof(char));
                lseek(file, -recordsLength, SEEK_CUR );
            }else break;      
        }   
        lseek(file, -recordsLength , SEEK_CUR);
        write(file, y, recordsLength* sizeof(char));
    }                    
        close(file);
        free(y);
        free(cmp);
        return 0;
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
    
    
    char * type =  argv[3];
    
    if (strcmp(type,"sys")!=0 &&  (strcmp(type,"lib")!=0)) {
        perror ("You typed wrong type, choose between lib and sys!.");
        exit (1);
    }  
    
    
    char * fileName = argv[1];
    int length = (atoi)(argv[2]);
    struct rusage times;
    struct timeval userBefore, userAfter, systemBefore, systemAfter;
    long userm, users, systemm, systems;
    

    
  if (strcmp(type, "lib") == 0){
      
    getrusage(RUSAGE_SELF, &times);
    userBefore = times.ru_utime;
    systemBefore = times.ru_stime;
    
    libSort(length,fileName);
    getrusage(RUSAGE_SELF, &times);
    userAfter = times.ru_utime;
    systemAfter = times.ru_stime;
    
   
    if(userAfter.tv_usec < userBefore.tv_usec){
         userm = userBefore.tv_usec - userAfter.tv_usec; 
         users = userAfter.tv_sec - userBefore.tv_sec -1;
    }
    else{
         userm = userAfter.tv_usec - userBefore.tv_usec; 
         users = userAfter.tv_sec - userBefore.tv_sec ;
    }
       
    if(systemAfter.tv_usec < systemBefore.tv_usec){
         systemm = systemBefore.tv_usec - systemAfter.tv_usec; 
         systems = systemAfter.tv_sec - systemBefore.tv_sec -1;
    }
    else{ 
         systemm =systemAfter.tv_usec - systemBefore.tv_usec; 
         systems = systemAfter.tv_sec - systemBefore.tv_sec ;
    }
    printf("User time = %ld.%06ld\n", users, userm);
    printf("System time = %ld.%06ld\n\n", systems,systemm);
    }
    
    
    
    if (strcmp(type,"sys")==0){
    getrusage(RUSAGE_SELF, &times);
    userBefore = times.ru_utime;
    systemBefore = times.ru_stime;
         
    sysSort(length,fileName);
       
    getrusage(RUSAGE_SELF, &times);
    userAfter = times.ru_utime;
    systemAfter = times.ru_stime;
    
        
    if(userAfter.tv_usec < userBefore.tv_usec){
         userm = userBefore.tv_usec - userAfter.tv_usec; 
         users = userAfter.tv_sec - userBefore.tv_sec -1;
    }
    else{
         userm = userAfter.tv_usec - userBefore.tv_usec; 
         users = userAfter.tv_sec - userBefore.tv_sec ;
    }
    
    if(systemAfter.tv_usec < systemBefore.tv_usec){
         systemm = systemBefore.tv_usec - systemAfter.tv_usec; 
         systems = systemAfter.tv_sec - systemBefore.tv_sec -1;
    }
    else{
         systemm = systemAfter.tv_usec - systemBefore.tv_usec; 
         systems = systemAfter.tv_sec - systemBefore.tv_sec ;
    }     
    
    printf("User time = %ld.%06ld\n", users, userm);
    printf("System time = %ld.%06ld\n\n", systems, systemm);

    }
  
    return 0;
    
}