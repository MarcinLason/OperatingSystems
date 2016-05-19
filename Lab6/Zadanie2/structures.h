#ifndef STRUCTURES_H
#define STRUCTURES_H

#define LIMIT 20
#define LENGTH 5 //max length of queue name
#define SIZE sizeof(messageType) //max size of the message

typedef struct Message {
    int type;
    char queueName[LENGTH];
    int clientID;
    int number;
    int isPrime;
} messageType;

typedef union QueueMessage {
    char bytes[SIZE+15];
    messageType message;
} queueMessageType;

typedef enum MessageType {
    NEW_CLIENT = 1,
    CLIENT_READY,
    CLIENT_RESPONSE,
    SERVER_ACCEPTANCE,
    SERVER_RESPONSE,
    CLOSE_QUEUE
} message_type_t;
#endif

