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
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include "structures.h"

mqd_t clients[LIMIT];
mqd_t serverDesc;
int clientsCounter = 0;
char* myQueueName;

void openConnection(char* qName)
{
    queueMessageType msg;
    mqd_t clientDesc = mq_open(qName, O_WRONLY);

    if(clientDesc < 0) {
        printf("Error while opening client's queue!\n");
        exit(EXIT_FAILURE);
    }
    
    clients[clientsCounter] = clientDesc;
    msg.message.type = SERVER_ACCEPTANCE;
    msg.message.clientID = clientsCounter;

    if(mq_send(clientDesc, msg.bytes, SIZE, 0) < 0) {
        printf("Error! Cannot open client connection!\n");
    }
    clientsCounter = clientsCounter + 1;
}

void closeConnection(int clientID)
{
    mqd_t clientDesc = clients[clientID];
    printf("Closing client %d queue\n", clientID);

    if(mq_close(clientDesc) == -1) {
        printf("Error while closing connection with client %d!", clientID);
    }
    clients[clientID] = -1;
}

void closeMyQueue()
{
    int i;
    for(i = 0; i < clientsCounter; i+=1) {
        if(clients[i] >= 0) {
            closeConnection(i);
        }
    }

    if(mq_close(serverDesc) == -1) {
        printf("Error while closing server's queue!\n");
    }

    if(mq_unlink(myQueueName) == -1) {
        printf("Error while unlinking server's descriptor!\n");
    }
    exit(EXIT_SUCCESS);
}

void sendNumber(int clientID)
{
    queueMessageType msg;
    if(clientID >= clientsCounter) {
        printf("Error while sending number! Invalid client id!\n");
        return;
    }

    msg.message.type = SERVER_RESPONSE;
    msg.message.number = (rand() % 10000) + 1;

    if(mq_send(clients[clientID], msg.bytes, SIZE, 0) == -1) {
        printf("Error while sending number to client %d!\n", clientID);
    }
}

void handleResponse(int clientID, int number, int isPrime)
{
    if(isPrime <= 0) return;
    printf("Liczba pierwsza: %d (klient: %d)\n", number, clientID);
}

void handleMessage(mqd_t queue_mqd)
{
    queueMessageType msg;
    message_type_t type;

    while(mq_receive(queue_mqd, msg.bytes, SIZE+15, NULL) >= 0) {
        type = (message_type_t) msg.message.type;
        
        if(type == NEW_CLIENT) {
            printf("New client: [%s]\n", msg.message.queueName);
            openConnection(msg.message.queueName);
        }
        if(type == CLIENT_READY) {
            sendNumber(msg.message.clientID);
        }
        
        if(type == CLIENT_RESPONSE) {
            handleResponse(msg.message.clientID, msg.message.number, (int)msg.message.isPrime);
        }
        
        if(type == CLOSE_QUEUE) {
            closeConnection(msg.message.clientID);
        }    
    }
    printf("Error while receiving message!\n");
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    struct mq_attr attributes;
    
    if(argc != 2) {
        printf("You have to pass name of the queue! Remember that it should starts with /! \n");
        exit(EXIT_FAILURE);
    }

    myQueueName = strdup(argv[1]);
    attributes.mq_flags = 0;
    attributes.mq_maxmsg = 10;
    attributes.mq_msgsize = SIZE;
    attributes.mq_curmsgs = 0;
    serverDesc = mq_open(myQueueName, O_RDONLY | O_CREAT, 0666, &attributes);

    if(serverDesc < 0) {
        printf("Error while opening server's queue!\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, closeMyQueue);
    handleMessage(serverDesc);
    free(myQueueName);
    return 0;
}