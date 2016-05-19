#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include "structures.h"
#define SIZE sizeof(message_t)-sizeof(long int) // size of the message
int myQueueID;

int checkNumber(int number) //checking if number is prime number
{
    if(number <= 1 || number % 2 == 0) {
        return 0;
    }

    int i;
    for(i = 3; i <= sqrt(number); i += 2) {
        if(number % i == 0) {
            return 0;
        }
    }
    return 1;
}

int connectServer(int serverQueueID)
{
    message_t msg;
    msg.mtype = NEW_CLIENT;
    msg.client_id = (int32_t) myQueueID;

    printf("Server queue id: %d\n", serverQueueID);
    printf("My queue id: %d\n", myQueueID);
    
    if(msgsnd(serverQueueID, &msg, SIZE, 0) < 0) {
        printf("Error while connecting to the server's queue!\n");
        exit(EXIT_FAILURE);
    }

    if(msgrcv(myQueueID, &msg, SIZE, SERVER_ACCEPTANCE, 0) < 0) {
        printf("Error while reading server's communicate to my queue!\n");
        exit(EXIT_FAILURE);
    }
    return msg.client_id;
}

void sendReadyMessage(int myID, int serverQueueID)
{
    message_t msg;
    msg.mtype = CLIENT_READY;
    msg.client_id = myID;

    if(msgsnd(serverQueueID, &msg, SIZE, 0) < 0) {
        printf("Error while sending ready status to server!\n");
    }
}

void sendResponse(int myID, int number, int serverQueueID)
{
    message_t msg;
    msg.mtype = CLIENT_RESPONSE;
    msg.number = number;
    msg.is_prime = checkNumber(number);
    msg.client_id = myID;

    if(msgsnd(serverQueueID, &msg, SIZE, 0) < 0) {
        printf("Error while sending response!\n");
    }
}

void handleMessage(int myID, int serverQueueID)
{
    message_t msg;
    sendReadyMessage(myID, serverQueueID);

    while(msgrcv(myQueueID, &msg, SIZE, SERVER_RESPONSE, 0) >= 0) {
        sendResponse(myID, msg.number, serverQueueID);
        sleep(1);
        sendReadyMessage(myID, serverQueueID);
    }
}

void closeMyQueue()
{
    if(msgctl(myQueueID, IPC_RMID, 0) == -1){
        printf("Error with closing client's queue!\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    char* path;
    int serverID;
    int myID;
    int serverQueueID;

    if(argc != 3) {
        printf("Wrong amount of arguments! Please pass PATHNAME and ID.\n");
        exit(EXIT_FAILURE);
    }

    path = strdup(argv[1]);
    serverID = atoi(argv[2]);    
    key_t serverKey = ftok(path, serverID);

    if(serverKey < 0) {
        printf("Ftok error, wrong serverKEY!\n");
        exit(EXIT_FAILURE);
    }

    serverQueueID = msgget(serverKey, S_IWUSR| S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    myQueueID = msgget(IPC_PRIVATE, S_IWUSR| S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if(serverQueueID < 0 || myQueueID < 0) {
        printf("Msgget error, wrong serverQueueID!\n");
        exit(EXIT_FAILURE);
    }
    
    signal(SIGINT, closeMyQueue);
    myID = connectServer(serverQueueID);
    handleMessage(myID, serverQueueID);
    return 0;
}