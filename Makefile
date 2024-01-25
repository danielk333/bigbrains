all: main

main: src/main.c
	gcc -g src/main.c -o bin/main

run: main
	./main examples/hello-world-short.bf

clean: 
	rm -v main