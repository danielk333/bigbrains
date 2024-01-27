CFLAGS := -g
LFLAGS :=

BIN_PATH := bin
SRC_PATH := src

SRC_FILES := main.c
SRC := $(addprefix $(SRC_PATH)/,$(SRC_FILES))

all: $(BIN_PATH)/bigbrains
build: $(BIN_PATH)/bigbrains

$(BIN_PATH)/bigbrains: $(SRC)
	gcc $(LFLAGS) $(CFLAGS) -o $@ $(SRC)

run_example: $(BIN_PATH)/bigbrains
	./$(BIN_PATH)/bigbrains run examples/hello-world-short.bf

compile_example: $(BIN_PATH)/bigbrains
	./$(BIN_PATH)/bigbrains build examples/hello-world-short.bf

clean: 
	rm -v $(BIN_PATH)/bigbrains