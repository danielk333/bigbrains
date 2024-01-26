all: main

bigbrains: src/main.c
	gcc -g src/main.c -o bin/bigbrains

build: bigbrains

run_example: bigbrains
	./bin/bigbrains run examples/hello-world-short.bf

compile_example: bigbrains
	./bin/bigbrains build examples/hello-world-short.bf

clean: 
	rm -v bin/bigbrains