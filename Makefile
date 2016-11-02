project: neun.o neun.h
	gcc neun.o -Wall -lncurses -o project
neun.o : neun.h neun.c
	gcc -c neun.c -Wall -lncurses
clean :
	rm *.o
