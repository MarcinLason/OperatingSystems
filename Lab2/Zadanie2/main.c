/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Marcin Lasoń
 *
 * Created on 12 March 2016, 18:12
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
/*
 * 
 */
void printRegularFileInfo(char * name, char * passedModes, char * relativePath) {
    struct stat st;
    stat(name, &st);
    char * realModes = (char*)malloc(11 * sizeof(char));
    
    (S_ISDIR(st.st_mode)) ? strcpy(realModes, "d") : strcpy(realModes, "-");
    (st.st_mode & S_IRUSR) ? strcat(realModes, "r") : strcat(realModes, "-");
    (st.st_mode & S_IWUSR) ? strcat(realModes, "w") : strcat(realModes, "-");
    (st.st_mode & S_IXUSR) ? strcat(realModes, "x") : strcat(realModes, "-");
    (st.st_mode & S_IRGRP) ? strcat(realModes, "r") : strcat(realModes, "-");
    (st.st_mode & S_IWGRP) ? strcat(realModes, "w") : strcat(realModes, "-");
    (st.st_mode & S_IXGRP) ? strcat(realModes, "x") : strcat(realModes, "-");
    (st.st_mode & S_IROTH) ? strcat(realModes, "r") : strcat(realModes, "-");
    (st.st_mode & S_IWOTH) ? strcat(realModes, "w") : strcat(realModes, "-");
    (st.st_mode & S_IXOTH) ? strcat(realModes, "x") : strcat(realModes, "-");
    
    if(strcmp(passedModes, realModes) == 0) {
        char * date;
        time_t * czas;
        czas = &st.st_atime;
        date = ctime(czas);
        printf("%s ", relativePath);
        printf("SIZE: %ld B ", st.st_size);
        printf("LAST ACCESS: %s ", date);
        printf("File Permissions: \t");
        printf("%s", realModes);
        printf("\n");
    }
    
}

void printCatalog (char * path, char * passedModes, char * relativePath) {
    DIR *d;
    struct dirent *dir;
    if ((d = opendir(path)) == NULL) {
        perror ("Cannot open.");
        exit (1);
    }
    
        while (dir = readdir(d))
        {
            char * name = dir->d_name;
            char * newPath = (char*)malloc(2 + strlen(path) + strlen(name));
            char * newRelativePath = (char*)malloc(2 + strlen(path) + strlen(name));
            strcpy(newRelativePath, relativePath);
            strcat(newRelativePath, name);
            strcpy(newPath, path);
            strcat(newPath, name);
            
            if((unsigned int)dir->d_type == 8) {
                printRegularFileInfo(newPath, passedModes, newRelativePath);
                
            }
            
            if((unsigned int)dir->d_type == 4) {
                if( (strcmp(name, "..")!=0) && (strcmp(name, ".") != 0) ) {
                    strcat(newPath, "/");
                    strcat(newRelativePath, "/");
                    printCatalog(newPath, passedModes, newRelativePath);
                    free(newPath);
                    free(newRelativePath);
                }   
            }  
    }
    closedir(d);
}
void help() {
    printf("Welcome in the help sector.\n\n");
    printf("Correct way to start program:\n");
    printf("./program [Path] [AccessRights] \n\n");
    printf("Path - path to the catalog which will be searched.\n");
    printf("AccessRights - access modes, only files with this modes will be listed.\n");   
}
int main(int argc, char** argv) {    
    
    if(argc > 3) {
        printf("You passed too much arguments.\n");
        return 0; 
    }
    
    if(argc == 2 && strcmp(argv[1], "-h") == 0){
        help();
        return 0;
    }
    
    if(argc < 3) {
        printf("You passed not enough arguments.\n");
        return 0;
    }
    
    char * initialPath;
    char * accessModes;
    char * initialRelativePath = "/";
    initialPath = argv[1];
    accessModes = argv[2];
    printCatalog(initialPath, accessModes, initialRelativePath);
    return(0);
}

