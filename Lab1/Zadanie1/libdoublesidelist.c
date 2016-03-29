#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libdoublesidelist.h"


node * createList () { //creating a list
    node * head = NULL;
    return head;
}

person * createPerson(char * Firstname, char * Lastname, char * DateofBirth, char * Email, char * Phone, char * Address) { // creating a person
    person * result = (person*)malloc(sizeof(person));
    strcpy(result->Firstname, Firstname);
    strcpy(result->Lastname, Lastname);
    strcpy(result->DateofBirth, DateofBirth);
    strcpy(result->Email, Email);
    strcpy(result->Phone, Phone);
    strcpy(result->Address, Address);
    return result;
}

node * insert(node * head, person * AddingPerson) { //inserting new element to the list
    if(head == NULL) {
        node * result = (node*)malloc(sizeof(node*));
        result->next = NULL;
        result->prev = NULL;
        result->data = AddingPerson;
        head = result;
    }
    
    else {
        node * tmp = (node*)malloc(sizeof(node));
        tmp = head;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = (node*)malloc(sizeof(node));
        tmp->next->next = NULL;
        tmp->next->prev = tmp;
        tmp->next->data = AddingPerson;
    }
        
    return head;
    
}

node * deleteElement (node * head, person * p) {
    if(head == NULL) {
        printf("\n This list is empty, I can not delete required element.\n");
        return NULL;
    }
    
    
    if(strcmp(head->data->Lastname, p->Lastname)==0) {
        node * result = (node*)malloc(sizeof(node));
        printf("ups, musze usunac heada");
        result = head;
        if(head->prev != NULL) {
            result = head->prev;
            result->next = head->next;
        }
        
        if(head->next != NULL) {
            result = head->next;
            result->prev = head->prev;
        }
        
        free(head);
        return result;
    }
    node * toDestroy = (node*)malloc(sizeof(node));
    toDestroy = search(head, p->Lastname);
    
    if(toDestroy == NULL) {
        printf("In the list there is no element which you are looking for.\n");
        return head;
    }
    
    if(toDestroy != NULL) {
        if(toDestroy->prev != NULL){
            toDestroy->prev->next = toDestroy->next;
        }
        if(toDestroy->next != NULL) {
            toDestroy->next->prev = toDestroy->prev;
        }
        free(toDestroy);
        return head;
    }
    return head;
}

node * search (node * head, char * lastName) {
    node * tmp = (node*)malloc(sizeof(node));
    if (head == NULL) {
        printf("I can find anything in the empty list!\n");
        return NULL;
    }
    if (strcmp(head->data->Lastname, lastName) == 0) { //when searching element was in head
        tmp = head;
        return tmp;
    }
    
    tmp = head;
    
    while(tmp->next != NULL){
        if(strcmp(tmp->next->data->Lastname, lastName) == 0) {
            return tmp->next;
        }
        tmp = tmp->next;
    }
    
    tmp = head;
    while(tmp->prev != NULL) {
        if(strcmp(tmp->prev->data->Lastname, lastName) == 0) {
            return tmp->prev;
        }
        tmp = tmp->prev;
    }
    
    return NULL;
}

void removeList(node * head) {
    if (head == NULL) { //do nothing when I receive empty list
        return;
    }
    
    if(head->prev == NULL && head->next == NULL) { //when head is the only one node on the list, I simply do free(head) and return
        free(head);
        return;
    }
    node * tmp1 = (node *)malloc(sizeof(node));
    node * tmp2 = (node *)malloc(sizeof(node));
    
    tmp1 = head;
    while(tmp1->prev != NULL) { //Setting tmp1 on the first element of the list
        tmp1 = tmp1->prev;
    }
    
    while (tmp1->next != NULL) { //tmp1 is going from the first element to the last and do free on each element
        tmp2 = tmp1->next;
        free(tmp1);
        tmp1 = tmp2;
    }
    free(tmp2);
    printf("\n");
    printf("List has been successfully removed! \n");
    return;
} 

node * sort(node * head) {
    while(head->prev != NULL) { //setting head to the first position of the list
        head = head->prev;
    }
    node * list1 = (node*)malloc(sizeof(node));
    node * list2 = (node*)malloc(sizeof(node));
    list1 = head;
    
    if(list1 == NULL || list1->next == NULL) {
        return list1;
    }
    list2 = bisectList(list1);
    return mergeList(sort(list1), sort(list2));
}

node * mergeList(node * list1, node * list2) {
    if(list1 == NULL) {
        return list2;
    }
    if(list2 == NULL) {
        return list1;
    }
    
    if (strcoll(list1->data->Lastname, list2->data->Lastname) < 0)
    {
        list1->next = mergeList(list1->next,list2);
        list1->next->prev = list1;
        list1->prev = NULL;
        return list1;
    }
    if (strcoll(list1->data->Lastname, list2->data->Lastname) > 0)
    {
        list2->next = mergeList(list1,list2->next);
        list2->next->prev = list2;
        list2->prev = NULL;
        return list2;
    }
    
    else {
        if(strcoll(list1->data->Firstname, list2->data->Firstname) < 0) {
            list1->next = mergeList(list1->next,list2);
            list1->next->prev = list1;
            list1->prev = NULL;
            return list1;     
        }
        
        else {
            list2->next = mergeList(list1,list2->next);
            list2->next->prev = list2;
            list2->prev = NULL;
            return list2;
        }
    }

}

node * bisectList(node * head) {
    node * fast = (node*)malloc(sizeof(node));
    node * slow = (node*)malloc(sizeof(node));
    node * prev = (node*)malloc(sizeof(node));
    
    fast = head;
    slow = head;
    prev = NULL;
    
    while (fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }

    if (prev != NULL)
    {
        prev->next = NULL;
        slow->prev = NULL;
    }
    return slow;
}

void print(char * text, int size){
    int i = 0;
    while(i < size) {
        if((text[i] > 63 && text[i] < 91) || (text[i] > 96 && text[i] < 123) || (text[i] == 46) || (text[i] > 47 && text[i] < 58)){
            printf("%c", text[i]);
        }
        i = i + 1;
    }
    printf(",");
}

void printLetters(char * text, int size) {
    int i = 0;
    while(i < size) {
        if((text[i] > 63 && text[i] < 91) || (text[i] > 96 && text[i] < 123) || (text[i] == 46)) {
            printf("%c", text[i]);
        }
        i = i + 1;
    }
    printf(",");
}

void printPerson(person * p) {
    printLetters(p->Firstname, 20);
    printLetters(p->Lastname, 20);
    print(p->DateofBirth, 10);
    printLetters(p->Email, 30);
    print(p->Phone, 10);
    printLetters(p->Address, 50);
    printf(";\n");
}

void printList(node * head) {
    if(head == NULL) {
        printf("The list is empty");
        return;
    }
    
    node * tmp = (node*)malloc(sizeof(node));
    tmp = head;
    printf("\nElements of the list\n");
    while(tmp->prev != NULL) {
        tmp = tmp->prev;
    }
    
    while(tmp->next != NULL) {
        printPerson(tmp->data);
        tmp = tmp->next;
        
    }
    return;
}

