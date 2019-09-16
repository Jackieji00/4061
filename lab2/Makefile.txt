main: a.o b.o
	\\Complete recipe for main
	gcc a.o -o main
	gcc b.o -o main
a.o: a.c lib1.h a.h
	\\Complete dependencies for a.o
	\\Complete recipe for a.o
	gcc -c a.c
b.o: b.c a.h
	\\Complete dependencies for b.o
	\\Complete recipe for b.o
	gcc -c b.c
clean:
	rm *.o main
