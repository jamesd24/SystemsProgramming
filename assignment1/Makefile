all: clean hello cat terminal

hello: hello.c
	gcc -Wall -std=c99 hello.c -o hello
	
cat:
	gcc -Wall -std=c99 cat.c cat.h -o cat

terminal: terminal.o
	clang -o terminal terminal.o
	
terminal.o: terminal.c terminal.h
	clang -c -Wall -std=c99 terminal.c -o terminal.o
	

clean:
	rm -f hello
	rm -f cat
	rm -f terminal
	rm -f terminal.o