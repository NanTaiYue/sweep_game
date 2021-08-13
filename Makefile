all : sweep

clean :
	rm -f sweep

sweep : main.c
	gcc -Wall -g -o $@ $^