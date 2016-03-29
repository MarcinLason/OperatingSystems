/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Marcin Lasoń
 *
 * Created on 14 March 2016, 23:12
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int file;
int menu()
{
        system("clear");
        int x;
        printf("MENU:\n");
        printf("1) Ustaw rygiel do odczytu na wybrany znak pliku.\n");
        printf("2) Ustaw rygiel do zapisu na wybrany znak pliku.\n");
        printf("3) Wyświetl liste zaryglowanych znaków pliku.\n");
        printf("4) Zwolnij wybrany rygiel.\n");
        printf("5) Odczytaj wybrany znak pliku.\n");
        printf("6) Zmien wybrany znaku pliku.\n");
        printf("7) Opusc program.\n");
        printf("Podaj liczbe, ktora wybrales.");
        scanf("%d", &x);
        return x;
}

void changeSign(int position, char newSign) {
    lseek(file, position, SEEK_SET); 
    write(file, &newSign, 1);
    lseek(file, 0, SEEK_SET); 
}

void lockFile(off_t indeks, int type){
    static struct flock lock;
    lock.l_type = type;
    lock.l_start = indeks;
    lock.l_whence = SEEK_SET;
    lock.l_len = 1;
    lock.l_pid = getpid();
    fcntl(file, F_SETLK, &lock);
}

char readPosition(int position) {
    char sign[1];
    lseek(file, position, SEEK_SET);
    if(read(file, sign, 1) <= 0){
        perror("Can't read the sign.");
    }
    lseek(file, 0, SEEK_SET);
    return sign[0];
}

void lockedList(){
    struct flock lockData;
    lockData.l_whence = SEEK_SET;
    lockData.l_start = 0;
    lockData.l_len = 1;
    lockData.l_type = F_WRLCK;
    lseek(file, 0, SEEK_SET);
    size_t length = lseek(file, 0, SEEK_END) + 1;
         
   
    for(int i = 1; i<length; i++) {
        if(fcntl(file, F_GETLK, &lockData) < 0) {
            printf("fctnl error\n");
        }

        if(lockData.l_type != F_UNLCK) {
            printf("Byte: %ld    ", lockData.l_start);
            printf("Type: %s    ", lockData.l_type == F_WRLCK ? "write" : "read");
            printf("PID: %d\n\n", lockData.l_pid);
        }            
        lockData.l_start = i;
        lockData.l_type = F_WRLCK;
    }
}
   
int main(int argc, char *argv[])
{
    if( argc!=2 )
    {
        printf("You have to pass 2 arguments.\n");
        return 0;
    }
    
    char filename[50];
    strcpy(filename, argv[1]);
    file = open(filename, O_RDWR);  
    if (!file)
    {
        perror ("Cannot open.");
        exit (1);
    }
      
    else{
        while(1)
        {
            int byteNumber;
            char newSign;
            int usersChoice = menu();
               
            if(usersChoice != 7 && usersChoice != 3)
            {
                printf("Podaj numer bajtu: ");
                scanf("%d", &byteNumber);
            }

            switch(usersChoice)
            {
                case 1:
                lockFile(byteNumber, F_RDLCK);
                break;
            
                case 2:
                lockFile(byteNumber, F_WRLCK);
                break;
                
                case 3:
                lockedList();
                break;
                
                case 4:
                lockFile(byteNumber, F_UNLCK);
                break;
                
                case 5:
                printf("%c\n", readPosition(byteNumber));
                break;

                case 6:
                printf("New sign: ");
                getchar(); 
                newSign = getchar();
                changeSign(byteNumber, newSign);
                break;
                
                case 7:
                close(file);
                return 0;
                }
               
                getchar();
                printf("Wcisnij dowolny przycisk...\n");
                getchar();
        }
}}
