#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include "structures.h"
#define N 20 //maximum amount of clients
#define SIZE sizeof(message_t)-sizeof(long int) //size of the message

int clientsQueues[N]; //array of clients queues
int clientsCounter = 0;
int serverQueueID;

void connectClient(int32_t clientQueueID)
{
    message_t msg;

    if(clientsCounter >= N) {
        printf("Exceeded client limit. Connection refused \n");
        return;
    }

    clientsQueues[clientsCounter] = clientQueueID;
    msg.mtype = SERVER_ACCEPTANCE;
    msg.client_id = clientsCounter;

    if(msgsnd(clientQueueID, &msg, SIZE, 0) < 0) {
        printf("Cannot open client connection. Msgsnd error\n");
    }
    clientsCounter = clientsCounter + 1;
}

void closeMyQueue()
{
    if(msgctl(serverQueueID, IPC_RMID, 0) == -1){
        printf("Error while closing server's queue!\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void sendNumber(int client_id)
{
    message_t msg;

    if(client_id >= clientsCounter) {
        printf("Error while sending number! Invalid client id!\n");
        return;
    }

    msg.mtype = SERVER_RESPONSE;
    msg.number = (rand() % 10000) + 1;

    if(msgsnd(clientsQueues[client_id], &msg, SIZE, 0) < 0) {
        printf("Error while sending random number!\n");
    }
}

void handleResponse(int client_id, int number, int is_prime)
{
    if(is_prime <= 0) return;
    printf("Liczba pierwsza: %d (klient: %d)\n", number, client_id);
}

void handleMessage(int serverQueueID)
{
    message_t msg;
    message_type_t type;

    while(msgrcv(serverQueueID, &msg, SIZE, 0, 0) >= 0) {
        type = (message_type_t) msg.mtype;
        
        if(type == NEW_CLIENT) {
            printf("I got new client [id: %d]\n", msg.client_id);
            connectClient(msg.client_id);
        }
        
        if(type == CLIENT_READY) {
            sendNumber(msg.client_id);
        }
        
        if(type == CLIENT_RESPONSE) {
            handleResponse(msg.client_id, msg.number, (int)msg.is_prime);
        }
    }
    printf("Error with msgrcv while handling message!\n");
}

int main(int argc, char* argv[])
{
    char* path;
    int serverID;
    
    if(argc != 3) {
        printf("Wrong amount of arguments! Please pass PATHNAME and ID.\n");
        exit(EXIT_FAILURE);
    }
    
    path = strdup(argv[1]);
    serverID = atoi(argv[2]); 
    key_t serverKEY = ftok(path, serverID);
    srand(time(NULL));

    if(serverKEY < 0) {
        printf("Ftok error, wrong serverKEY!\n");
        exit(EXIT_FAILURE);
    }
    
    serverQueueID = msgget(serverKEY, IPC_CREAT | S_IWUSR| S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(serverQueueID < 0) {
        printf("Msgget error, wrong serverQueueID!\n");
        exit(EXIT_FAILURE);
    }
    
    signal(SIGINT, closeMyQueue);
    handleMessage(serverQueueID);
    return 0;
}