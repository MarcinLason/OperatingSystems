CC = gcc
CFLAGS = -Wall -fPIC
STATIC = libdoublesidelist.a
DYNAMIC = libdoublesidelist.so
SRC = libdoublesidelist.c
OBJECT = libdoublesidelist.o
all: $(STATIC) $(DYNAMIC)

$(DYNAMIC): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -shared -o $(DYNAMIC) $(OBJECT)

$(STATIC): $(SRC)
	$(AR) ruv $(STATIC)
	ranlib $(STATIC)

clean:
	$(RM) $(STATIC) $(DYNAMIC) $(OBJECT)
