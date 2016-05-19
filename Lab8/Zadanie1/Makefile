all:
	gcc -Wall -o generator generator.c -I.
	gcc -Wall -o asynch asynchronous.c -I. -lpthread
	gcc -Wall -o synch synchronous.c -I. -lpthread
	gcc -Wall -o det detached.c -I. -lpthread
clean:
	rm generator asynch synch det
