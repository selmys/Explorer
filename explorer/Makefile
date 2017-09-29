all: s8 c8
cc=c++
s8: s8.o
	$(cc) -o s8 s8.o
s8.o: s8.cpp classes.h globals.h
	$(cc) -c s8.cpp
c8: c8.o
	$(cc) -o c8 c8.o
c8.o: c8.cpp 
	$(cc) -c c8.cpp
clean:
	rm s8 c8 s8.o c8.o
