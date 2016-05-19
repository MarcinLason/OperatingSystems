#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include "structures.h"

static int numberOfThreads;
static pthread_t *threadsID;
static pthread_key_t bufferKey;

void cancelThread(pthread_t threadID)
{
    int i;
    for(i = 0; i < numberOfThreads; i+=1) {
        if(!pthread_equal(threadID, threadsID[i])) {
            pthread_cancel(threadsID[i]);
        }
    }
}

void closeBuffer(void *threadBuffer)
{
    free((char *) threadBuffer);
}

int findText(size_t recordsCounter, char *wantedText)
{
    int i;
    char *buffer;
    record_t record;
    buffer = (char *) pthread_getspecific(bufferKey);

    for(i = 0; i < recordsCounter; i+=1) {
        memcpy(record.bytes, &buffer[i*RSIZE], RSIZE);
        if (strstr(record.data.text, wantedText) != NULL) {
            return record.data.ID;
        }
    }
    return -1;
}

void *readRecords(void *args)
{
    int id;
    threadArgs *p = (threadArgs *) args;
    char *buffer;
    size_t readValue;

    size_t bufferSize = RSIZE*p->recordsNumber;
    buffer = (char *) malloc(sizeof(char)*bufferSize);
    pthread_setspecific(bufferKey, buffer);

    while((readValue = read(p->fileDesc, buffer, bufferSize)) > 0) {
        pthread_testcancel();
        if((id = findText(readValue/RSIZE, p->text)) != -1) {
            printf("TID: %ld Record id: %d\n", syscall(SYS_gettid), id);
            cancelThread(pthread_self());
            break;
        }
    }

    if(readValue == -1) {
        printf("Error while reading the file!\n");
    }
    return NULL;
}

void startSearching(int threadsNumber, threadArgs args)
{
    int i;
    pthread_key_create(&bufferKey, closeBuffer);

    for(i = 0; i < threadsNumber; i+=1) {
        pthread_create(&threadsID[i], NULL, readRecords, &args);
    }

    for(i = 0; i < threadsNumber; i+=1) {
        pthread_join(threadsID[i], NULL);
    }
    free(threadsID);
}

int main(int argc, char* argv[])
{
    int recordsToRead;
    int fileDesc;

    if(argc != 5) {
        printf("Wrong number of arguments!\n");
        printf("You have to pass: number of threads, filename, number of records, text to find.\n");
        exit(EXIT_FAILURE);
    }
    numberOfThreads = atoi(argv[1]);
    recordsToRead = atoi(argv[3]);

    if((fileDesc = open(argv[2], O_RDONLY)) == -1) {
        printf("Error while opening the file!\n");
        return EXIT_FAILURE;
    }
    
    threadsID = malloc(sizeof(pthread_t)*numberOfThreads);
    threadArgs args;
    args.fileDesc = fileDesc;
    args.recordsNumber = recordsToRead;
    args.text = argv[4];

    startSearching(numberOfThreads, args);
    close(fileDesc);
    return EXIT_SUCCESS;
}