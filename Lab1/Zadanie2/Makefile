SRC = libdoublesidelist.c
OBJECT = libdoublesidelist.o
STAT = libdoublesidelist.a
DYN = libdoublesidelist.so
DYNAMIC = dynamic
SHARED = shared
STATIC = static
CC = gcc

all: $(STATIC) $(DYNAMIC) $(SHARED)


$(STATIC): $(SRC)
	$(CC) -Wall -c $(SRC) -o $(OBJECT)
	$(AR) ruv $(STAT) $(OBJECT)
	ranlib $(STAT)
	$(CC) -Wall -o $(STATIC) main.c -L. -ldoublesidelist

run_stat:
	./static

$(SHARED): $(SRC)
	$(CC) -Wall -fPIC -c $(SRC)
	$(CC) -Wall -fPIC -shared -o $(DYN) $(OBJECT)
	$(CC) -L. -o $(SHARED) main.c -ldoublesidelist

run_shared:
	LD_LIBRARY_PATH= -rpath.:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH= -rpath.:$LD_LIBRARY_PATH
	./shared

$(DYNAMIC): maindynamic.c
	$(MAKE) $(SHARED)
	$(CC) -Wall -o $(DYNAMIC) maindynamic.c -ldl

run_d:
	LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
	./dynamic


clean:
	$(RM) $(OBJECT) $(STAT) $(DYN) $(DYNAMIC) $(SHARED) $(STATIC)
