all: clean hello cat

hello: hello.c
	gcc -Wall hello.c -o hello
	
cat:
	gcc -Wall cat.c cat.h -o cat
	
clean:
	rm -f hello
	rm -f cat