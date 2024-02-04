CFLAGS := 
LDFLAGS := -lraylib

BIN_PATH := bin
SRC_PATH := src
OBJ_PATH := obj

TARGET_EXEC := $(BIN_PATH)/bigbrains

SRCS := $(wildcard $(SRC_PATH)/*.c)
OBJS := $(addprefix $(OBJ_PATH)/,$(notdir $(SRCS:.c=.o)))

all: $(TARGET_EXEC)
build: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS)
	gcc $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	gcc $(CFLAGS) -c $< -o $@


run_example: bin/bigbrains
	./bin/bigbrains run examples/hello-world-short.bf

transpile_example: bin/bigbrains
	./bin/bigbrains transpile examples/hello-world-short.bf

visualize_example: bin/bigbrains
	./bin/bigbrains visualize examples/hello-world-short.bf


clean: 
	rm -v $(TARGET_EXEC)
	rm -v $(OBJ_PATH)/*.o