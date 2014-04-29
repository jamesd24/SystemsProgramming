all: clean sync

sync: sync.c sync.h
	clang -std=gnu99 -Wall sync.c -g -o sync -lpthread

clean:
	rm -f sync
