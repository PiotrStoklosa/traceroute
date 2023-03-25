CPP = gcc
FLAGS = -std=gnu17 -Wall -Wextra

traceroute: Makefile traceroute.o receive.o send.o
		$(CPP) $(FLAGS) -o traceroute receive.o send.o traceroute.o

receive.o: Makefile receive.c
		$(CPP) $(FLAGS) -c receive.c -o receive.o

send.o: Makefile send.c
		$(CPP) $(FLAGS) -c send.c -o send.o

traceroute.o: Makefile traceroute.c traceroute.h
		$(CPP) $(FLAGS) -c traceroute.c -o traceroute.o

clean :
	rm -rf *.o

distclean :
	rm -rf *.o traceroute