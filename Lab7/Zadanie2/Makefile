all:
	gcc -Wall -o starter starter.c functions.c -I. -lrt -lpthread
	gcc -Wall -o writer writer.c functions.c -I. -lrt -lpthread
	gcc -Wall -o reader reader.c functions.c -I. -lrt -lpthread

clean:
	rm starter writer reader
