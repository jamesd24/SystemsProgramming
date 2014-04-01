all: clean hello cat terminal

hello: hello.c
	gcc -Wall hello.c -o hello
	
cat:
	gcc -Wall cat.c cat.h -o cat

terminal:
	gcc -Wall terminal.c terminal.h -o terminal	

clean:
	rm -f hello
	rm -f cat
	rm -f terminal
