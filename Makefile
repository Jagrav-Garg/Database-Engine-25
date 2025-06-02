CC = gcc
CFLAGS = -Wall -g
OBJ = main.o btree.o

btree: $(OBJ)
	$(CC) $(CFLAGS) -o btree $(OBJ)

main.o: main.c btree.h
	$(CC) $(CFLAGS) -c main.c

btree.o: btree.c btree.h
	$(CC) $(CFLAGS) -c btree.c

clean:
	rm -f *.o btree
