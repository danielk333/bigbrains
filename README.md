# C Brainfuck Interpreter/Compiler/Transpiler/Visualizer

Hobby project brainfuck tools in c to mess with my brain and get better at programming.

Idea is simple and quite stupid:
- If interpreting:
  1) Lex the brainfuck code
  2) Simulate a tape and execute token stream 
  3) ????
  4) Profit
- If transpiling: 
  1) Lex the brainfuck code
  2) Transform token stream to a specific instruction set: c or assembler (fasm) code
  3) Write instruction code to file, this can then be compiled
  4) Execute compilex code at your convenience
  5) ????
  6) Profit

Then there is also a brainfuck visualizer and IDE in one! Written in raylib. 
Coming soon^TM
- Visualizing: 
  1) Lex the brainfuck code / start a new brainfuck file
  2) Visualize the token stream and tape simulation
  3) ????
  4) Profit


## Requirements

Tools
- gcc
- fasm

Optional
- raylib

### Install on Archlinux

```bash
sudo pacman -S gcc fasm raylib
```

## Usage

Compile compiler (!) / interpreter
```bash
gcc src/main.c -o bigbrains
```

Transpile brainfuck
```bash
./bin/bigbrains transpile examples/hello-world-short.bf
```

Interpret and run brainfuck
```bash
./bin/bigbrains run examples/hello-world-short.bf
```

Visualize and write brainfuck
```bash
./bin/bigbrains visualize examples/hello-world-short.bf
```

