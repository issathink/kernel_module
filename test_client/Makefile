# Make of the client to test the module

CC =gcc
LFLAGS =-lpthread
CFLAGS =-g -W -Wall -Iinclude

HC=tools.h

all:  exec

exec: test.o tools.o 
	$(CC) -o $@ $^        

%.o: %.c $(HC)
	$(CC) $(CFLAGS) -o $@ -c $<
	
clean:
	rm -f exec *.o

.PHONY: all  clean
