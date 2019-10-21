main: rec7.o rec7_2.o
	gcc rec7.o -o main
	gcc rec7_2.o -o main2

rec7.o: rec7.c
	gcc -c rec7.c

rec7_2.o: rec7_2.c
	gcc -c rec7_2.c

clean:
	rm *.o main
