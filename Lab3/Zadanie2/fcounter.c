#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>


int main(int argc, char * argv[])
{
char * path;
char * flagv;
char * flagw;

path = argv[1];
flagv = argv[2];
flagw = argv[3];
       
if(path == NULL)
{
    path = (char *)malloc(sizeof(char) * 2);
    strcpy(path, "../..");
}
	
if(flagv == "-v")
printf("PATH = %s\n", path);

DIR * dir;
dir = opendir(path);
int error_code = errno;
if(dir == NULL)
{
    fprintf(stderr, "Blad podczas otwierania katalogu\n");
    exit(error_code);
}

struct dirent * d;
int files_count = 0;
int files_dirs = 0;

pid_t PID;
int child_count = 0;
char tmp[512];

while((d = readdir(dir)) != NULL)
{
    if(strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0)
    {
        strcpy(tmp, path);
        strcat(tmp, "/");
        strcat(tmp, d->d_name); 
	
        
        if(strcmp(flagv, "-v") == 0)
        printf("Sprawdzam %s\n", tmp);

        struct stat s;
        
        if(stat(tmp, &s) < 0)
        fprintf(stderr, "stat error\n");

        if(S_ISDIR(s.st_mode))
	{	 
            child_count++;
            PID = fork();

            if(PID == -1)
            {
		child_count--;
		fprintf(stderr, "Blad fork\n");
		return -2;
            }
	
            else
                if(PID == 0)
		{   
                    
                    char *args[] = {"fcounter", tmp, flagv, flagw, "/", 0}; 
                    char *env[] = { 0 };
                    if(flagv == "-v")
                    {                            
			if(execve("fcounter", args, env) < 0)
			fprintf(stderr, "Blad execc\n");
                    }
			
                    else
			if(execve("fcounter", args, env) < 0)
			fprintf(stderr, "Blad exec\n");
		}
	}
			
        else
	if(S_ISREG(s.st_mode))
        {
            files_count++;
        }    
    }
}
       
pid_t pid = getpid();
printf("PID: %d, znalazl tyle plikow: %d\n", pid, files_count);	
int s;
while(child_count--)
{
    wait(&s);
    files_dirs += WEXITSTATUS(s);
}
if(flagw == "-w"){
    sleep(5);
}

return(files_count + files_dirs);        
}
