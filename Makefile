UsoDisco: UsoDisco.o command.o list.o
		gcc -o UsoDisco UsoDisco.o command.o list.o -pthread


UsoDisco.o: UsoDisco.c command.c command.h list.c list.h
		gcc -c UsoDisco.c -o UsoDisco.o -pthread

command.o: command.c command.h list.c list.h
		gcc -c command.c -o command.o -pthread

list.o: list.c list.h
		gcc -c list.c -o list.o

clean:
		/bin/rm *.o UsoDisco
