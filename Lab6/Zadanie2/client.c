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
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include "structures.h"

int clientID;
mqd_t serverDesc; 
mqd_t clientDesc;
char myQueueName[LENGTH];

int checkNumber(int number)
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

int connectServer()
{
    queueMessageType msg;
    msg.message.type = NEW_CLIENT;
    strcpy(msg.message.queueName, myQueueName);

    if(mq_send(serverDesc, msg.bytes, SIZE, 0) == -1) {
        printf("Error while opening client connection!\n");
        exit(EXIT_FAILURE);
    }

    if(mq_receive(clientDesc, msg.bytes, SIZE+15, 0) < 0) {
        printf("Error while opening client connection!\n");
        exit(EXIT_FAILURE);
    }

    if(msg.message.type != SERVER_ACCEPTANCE) {
        printf("Error, server refused connection!\n");
        exit(EXIT_FAILURE);
    }
    return msg.message.clientID;
}

void sendReadyMessage()
{
    queueMessageType msg;
    msg.message.type = CLIENT_READY;
    msg.message.clientID = clientID;

    if(mq_send(serverDesc, msg.bytes, SIZE, 0) < 0) {
        printf("Error while sending ready message!\n");
    }
}

void sendResponse(int number)
{
    queueMessageType msg;
    msg.message.type = CLIENT_RESPONSE;
    msg.message.number = number;
    msg.message.isPrime = checkNumber(number);
    msg.message.clientID = clientID;

    if(mq_send(serverDesc, msg.bytes, SIZE, 0) < 0) {
        printf("Error while sending response!\n");
    }
}

void handleMessage()
{
    queueMessageType msg;
    sendReadyMessage();

    while(mq_receive(clientDesc, msg.bytes, SIZE+15, NULL) >= 0) {
        if(msg.message.type == SERVER_RESPONSE) {
            sendResponse(msg.message.number);
            sleep(1);
            sendReadyMessage();
        } else {
            printf("Error! Client received unexpected message!\n");
        }
    }
    printf("Error while receiving the message!\n");
}

void closeMyQueue()
{
    queueMessageType msg;
    msg.message.type = CLOSE_QUEUE;
    msg.message.clientID = clientID;

    if(mq_send(serverDesc, msg.bytes, SIZE, 0) < 0) {
        printf("Send error in closeMyQueue()\n");
    }

    if(mq_close(clientDesc) == -1) {
        printf("Close error in closeMyQueue()\n");
        exit(EXIT_FAILURE);
    }

    if(mq_unlink(myQueueName) == -1) {
        printf("Unlink error in closeMyQueue()\n");
    }
    exit(EXIT_SUCCESS);
}

void generateQueueName(char *name, int length)
{
    int i;
    if(length < 3) {
        printf("Error while generating queue's name. Queue name is too short!\n");
    }

    name[0] = '/';
    name[length-1] = '\0';

    for(i = 1; i < length - 1; i+=1) {
        name[i] = rand() % ('z' - '0') + '0';
    }
}

int main(int argc, char* argv[])
{
    char *serverQueueName;
    srand(time(NULL));
    struct mq_attr attributes;

    if(argc != 2) {
        printf("You have to pass name of the queue!\n");
        exit(EXIT_FAILURE);
    }

    serverQueueName = strdup(argv[1]);
    generateQueueName(myQueueName, LENGTH);

    attributes.mq_flags = 0;
    attributes.mq_maxmsg = 10;
    attributes.mq_msgsize = SIZE;
    attributes.mq_curmsgs = 0;

    clientDesc = mq_open(myQueueName, O_RDONLY | O_CREAT, 0666, &attributes);
    serverDesc = mq_open(serverQueueName, O_WRONLY);
    signal(SIGINT, closeMyQueue);

    if(clientDesc < 0 || serverDesc < 0) {
        printf("Error while opening client or server queue!\n");
        exit(EXIT_FAILURE);
    }

    clientID = connectServer();
    handleMessage();
    free(serverQueueName);
    return 0;
}