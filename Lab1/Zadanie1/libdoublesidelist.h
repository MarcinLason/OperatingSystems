#ifndef LIBDOUBLESIDELIST_H_
#define LIBDOUBLESIDELIST_H_

typedef struct node node;
typedef struct person person;

struct person {
	char Firstname[20];
	char Lastname[20];
	char DateofBirth[10]; //in convention "27.04.1995"
	char Email[30];
	char Phone[9];
	char Address[50];
}; 

struct node {
    person * data;
    node * next;
    node * prev;
};

node * createList(void);
person * createPerson(char * Firstname, char * Lastname, char * DateofBirth, char * Email, char * Phone, char * Address);
void removeList(node * head);
node *  insert(node * head, person * AddingPerson);
node * search (node * head, char * lastName);
node * deleteElement (node * head, person * p);
node * sort(node * head);
node * mergeList(node * list1, node * list2);
node * bisectList(node * head);
void print(char * text, int size);
void printPerson(person * p);
void printLetters(char * text, int size);
void printList(node * head);
#endif
