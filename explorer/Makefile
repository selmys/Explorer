all: s8 c8
CC=c++

classes.o:	classes.cpp classes.h
	$(CC)	-c classes.cpp -o classes.o

s8: s8.o classes.o
	$(CC) s8.o classes.o -o s8
s8.o: s8.cpp classes.h
	$(CC) -c s8.cpp -o s8.o

c8: c8.o
	$(CC) c8.o -o c8
c8.o: c8.cpp
	$(CC) -c c8.cpp -o c8.o

clean:
	rm s8 c8 s8.o c8.o classes.o
