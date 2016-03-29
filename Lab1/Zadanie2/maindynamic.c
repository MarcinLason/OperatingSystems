#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "libdoublesidelist.h"
#include <sys/time.h> 
#include <sys/resource.h>
#include <dlfcn.h>

void catchError(){
    char *err;
     if ((err = dlerror()) != NULL)  {
            fputs(err, stderr);
            exit(1);
        }
}


struct rusage timer;
clock_t clk[4];

struct timeval u[4];
struct timeval s[4];

void controlPoint(int p){
     getrusage(RUSAGE_SELF, &timer);
       clk[p] = clock();
       u[p] = timer.ru_utime;
       s[p] = timer.ru_stime;
       printf(".............Point %d of time control .............................\n\n" , p);
       
       printf(" real.....%.6ld\n", clk[p]/CLOCKS_PER_SEC);
       printf(" user.....%ld.%06ld\n", u[p].tv_sec, u[p].tv_usec);
       printf(" system...%ld.%06ld\n\n", s[p].tv_sec,s[p].tv_usec);
       if(p>0){
           printf("real passed from start ...%.6ld\n", (clk[p]-clk[0])/CLOCKS_PER_SEC);
           printf("real passed from prev ... %.6ld\n", (clk[p]-clk[p-1])/CLOCKS_PER_SEC);
            
           printf("user passed from start .. %ld.%06ld\n", u[p].tv_sec- u[0].tv_sec, u[p].tv_usec-u[0].tv_usec);
           printf("user passed from prev  .. %ld.%06ld\n", u[p].tv_sec- u[p-1].tv_sec, u[p].tv_usec-u[p-1].tv_usec);
       
          printf(" sys passed from start .. %ld.%06ld\n", s[p].tv_sec- s[0].tv_sec, s[p].tv_usec-s[0].tv_usec);
          printf(" sys passed from prev  .. %ld.%06ld\n\n", s[p].tv_sec- s[p-1].tv_sec, s[p].tv_usec-s[p-1].tv_usec);           
       }
       }

int main(int argc, char *argv[]){
    
    void *handle;
    handle = dlopen ("./libdoublesidelist.so", RTLD_LAZY);
      if (!handle) {
		fputs(dlerror(), stderr);
		exit(1);
	}
    
    
    node *(*createList)(void);
    createList = dlsym(handle, "createList");
    catchError();
    
    person *(*createPerson)(char * Firstname, char * Lastname, char * DateofBirth, char * Email, char * Phone, char * Address);
    createPerson = dlsym(handle, "createPerson");
    catchError();
    
    void (*removeList)(node * head);
    removeList = dlsym(handle, "removeList");
    catchError();
    
    node *(*insert)(node * head, person * AddingPerson);
    insert = dlsym(handle, "insert");
    catchError();
    
    node *(*search)(node * head, char * lastName);
    search = dlsym(handle, "search");
    catchError();
    
    node *(*deleteElement) (node * head, person * p);
    deleteElement = dlsym(handle, "deleteElement");
    catchError();
    
    node *(*sort)(node * head);
    sort = dlsym(handle, "sort");
    catchError();
    
    node *(*mergeList)(node * list1, node * list2);
    mergeList = dlsym(handle, "mergeList");
    catchError();
    
    node *(*bisectList)(node * head);
    bisectList = dlsym(handle, "bisectList");
    catchError();
    
    void (*print)(char * text, int size);
    print = dlsym(handle, "print");
    catchError();
    
    void (*printPerson)(person * p);
    printPerson = dlsym(handle, "printPerson");
    catchError();
    
    void (*printLetters)(char * text, int size);
    printLetters = dlsym(handle, "printLetters");
    catchError();
    
    void (*printList)(node * head);
    printList = dlsym(handle, "printList");
    catchError();
    
    controlPoint(0);
    char Fnames[15][20] = {"Marcin", "Tomasz", "Jacek", "Wojciech", "Rafal", "Andrzej", "Krzysztof", "Lukasz", "Jan", "Aleksandra", "Katarzyna", "Iwona", "Marta", "Anna", "Barbara"};
    char Lnames[10][20] = {"Kowalski", "Nowak", "Matusik", "Tatar", "Gruszka", "Kowal", "Zielinski", "Wojciechowski", "Mazur", "Olszewski"};
    char Dates[15][10] = {"29.02.1966","22.06.1986", "30.01.1977", "11.09.1983", "23.02.1991", "03.03.1936", "19.06.1981", "02.05.1973", "13.07.1988", "27.12.1999", "15.06.1967", "15.03.1972", "04.10.1993", "07.06.1985", "22.07.1945"};
    char Emails[10][30] = {"konto1@gmail.com", "kontakt@poczta.pl", "poczta@onet.pl", "biuro@w.pl", "sport@gmail.com", "prezes@gmail.com", "AGH@aghedu.pl", "kontakt@firma.pl", "mail@onet.pl", "mail@wp.pl"};
    char Phones[15][10] = {"543895432", "235045923", "874235084", "334523642", "975345243", "593985345", "124943054", "754935953", "543955323", "653569532", "754984345", "575456294", "858456345", "986437548", "234875493"};
    char Addresses[15][50] = {"Krakow", "Rzeszow", "Lodz", "Poznan", "NowySacz", "Ilawa", "Wielun", "Bialystok", "Wroclaw", "Olsztyn", "Kety", "Szczecin", "Puck", "Elblag", "Kolobrzeg"};
   
    
    srand(time(NULL));
    int g[6];
    node * list = createList();
    printf("1. I have created a list.\n");
    
    int i = 0;
    int number = 200;
    while(i < number) {
        g[0] = rand()%15;
        g[1] = rand()%10;
        g[2] = rand()%15;
        g[3] = rand()%10;
        g[4] = rand()%15;
        g[5] = rand()%15;
        person * p = createPerson(Fnames[g[0]], Lnames[g[1]], Dates[g[2]], Emails[g[3]], Phones[g[4]], Addresses[g[5]]);
        list = insert(list, p);
        i++;      
    }
    printf("2. To the list I have added 200 people with random generated data.\n");
    
    printList(list);
    controlPoint(1);
    sort(list);
    printf("3. I have sorted the list by lastnames.\n");
    printList(list);
    
    char nazwisko[20] = "Tatar";
    node * wybor = search(list, nazwisko);
   
    controlPoint(2);
    while(wybor != NULL) {
       deleteElement(list, wybor->data);
       wybor = search(list, nazwisko);
    }
    printf("4. I have removed from the list all people named Tatar.\n");
    printList(list);
    
    
    removeList(list);
    printf("5. I have deleted the list. It is the end of the test program.\n");
    controlPoint(3);
       
        dlclose(handle);
	return 0;
}
