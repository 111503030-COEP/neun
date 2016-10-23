project : stack.o neun.c stack.h
	gcc neun.o stack.o -Wall -lncurses -o project
stack.o : stack.c stack.h
	gcc -c stack.c -Wall
neun.o : stack.h neun.c
	gcc -c neun.c -Wall -lncurses
clean :
	rm *.o
