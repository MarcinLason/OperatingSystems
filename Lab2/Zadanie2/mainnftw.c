#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <ftw.h>
#include <errno.h>
#include <stdio.h>
 
#if MAX_FDS < 1
#define MAX_FDS  15
#endif


char passedModes[] = "----------";

void help() {
    printf("Welcome in the help sector.\n\n");
    printf("Correct way to start program:\n");
    printf("./program [Path] [AccessRights] \n\n");
    printf("Path - path to the catalog which will be searched.\n");
    printf("AccessRights - access modes, only files with this modes will be listed.\n");   
}


static int for_each_file(FILE *const in, const char *filepath, const struct stat *info,
                         struct tm *localmtime)
{
    char * realModes = (char*)malloc(11 * sizeof(char));
    struct stat st;
    stat(filepath, &st);
    
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
    
    if(strcmp(passedModes, realModes)== 0)
    {
        printf("%s %s SIZE %.0f B, LAST ACCESS: %04d-%02d-%02d %02d:%02d:%02d.  \n",
            filepath, realModes,(double)info->st_size, localmtime->tm_year + 1900, localmtime->tm_mon + 1, localmtime->tm_mday,
            localmtime->tm_hour, localmtime->tm_min, localmtime->tm_sec);
        //fflush(stdout);
    }
    return 0;
}
 
 
static int for_each(const char *filepath, const struct stat *info,
                    int typeflag, struct FTW *ftwinfo)
{
    const char *const filename = filepath + ftwinfo->base;
    struct tm atime;
    FILE *in;
    if (typeflag == FTW_F) {
 
        localtime_r(&(info->st_atime), &atime);
        for_each_file(in, filepath, info, &atime); 
        return 0;
    }
    return 0;
}
 
 
int main(int argc, char** argv)
{
    
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
    initialPath = argv[1];
    accessModes = argv[2];
    
    passedModes[0] = accessModes[0];
    passedModes[1] = accessModes[1];
    passedModes[2] = accessModes[2];
    passedModes[3] = accessModes[3];
    passedModes[4] = accessModes[4];
    passedModes[5] = accessModes[5];
    passedModes[6] = accessModes[6];
    passedModes[7] = accessModes[7];
    passedModes[8] = accessModes[8];
    passedModes[9] = accessModes[9];
 
    if (nftw(initialPath, for_each, MAX_FDS, FTW_CHDIR)) {
        fprintf(stderr, "Failed.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}