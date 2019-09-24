main: a.o b.o
	gcc a.o b.o -o main
a.o: a.c a.h lib1.h
	gcc -c a.c
b.o: b.c b.h a.h
	gcc -c b.c
clean:
	rm *.o main
