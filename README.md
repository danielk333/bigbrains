# C Brainfuck Compiler

Hobby project brainfuck compiler in c to mess with my brain and get better at programming.

Idea is simple and quite stupid:
- If interpreting:
  1) Lex the brainfuck code
  2) Simulate a tape and execute token stream 
  3) ????
  4) Profit
- If compiling: 
  1) Lex the brainfuck code
  2) Transform token stream to a specific instruction set: c or assembler (fasm) code
  3) Write instruction code to file and compile instructions to machine code
  4) Execute machine code at your convenience
  5) ????
  6) Profit

## Requirements

Tools
- gcc
- fasm

### Install on Archlinux

```bash
sudo pacman -S gcc fasm
```

## Usage

Compile compiler (!) / interpreter
```bash
gcc src/main.c -o bigbrains
```

Compile brainfuck
```bash
./bin/bigbrains build examples/hello-world-short.bf
```

Interpret and run brainfuck
```bash
./bin/bigbrains run examples/hello-world-short.bf
```
