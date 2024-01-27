CFLAGS := -g
LFLAGS :=

BIN_PATH := bin

SRC := src/main.c src/interpreter.c src/compiler.c

TOOLS := bin/embedder

all: bin/bigbrains
build: bin/bigbrains
tools: $(TOOLS)

bin/bigbrains: $(TOOLS) $(SRC)
	mv src/{,_}interpreter_source.c
	bin/embedder src/interpreter.c interpreter_source > src/interpreter_source.c
	
	gcc $(LFLAGS) $(CFLAGS) -o $@ src/main.c
	
	rm src/interpreter_source.c
	mv src/{_,}interpreter_source.c

bin/embedder: src/embedder.c
	gcc $(LFLAGS) $(CFLAGS) -o $@ $<

run_example: bin/bigbrains
	./bin/bigbrains run examples/hello-world-short.bf

compile_example: bin/bigbrains
	./bin/bigbrains build examples/hello-world-short.bf

clean: 
	rm -v bin/bigbrains