all: main

main: src/main.c
	gcc -g src/main.c -o bin/bigbrains

run: main
	./bin/bigbrains examples/hello-world-short.bf

clean: 
	rm -v main