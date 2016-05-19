/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   structures.h
 * Author: student
 *
 * Created on 16 April 2016, 23:09
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Message {
    long int mtype;
    int32_t client_id;
    int32_t number;
    int8_t is_prime;
} message_t;


typedef enum MessageType {
    NEW_CLIENT = 1,
    CLIENT_READY,
    CLIENT_RESPONSE,
    SERVER_ACCEPTANCE,
    SERVER_RESPONSE
} message_type_t;

#endif