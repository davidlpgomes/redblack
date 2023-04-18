FLAGS=-Wall -std=c99
CC=gcc
PROG=redblack

all: $(PROG)

$(PROG): tree.o tester.o
	$(CC) $(FLAGS) tree.o tester.o -o $(PROG)

tester.o: tester.c
	$(CC) -c $(FLAGS) tester.c

tree.o: tree.c tree.h
	$(CC) -c $(FLAGS) tree.c

clean:
	rm -f *.o $(PROG)
