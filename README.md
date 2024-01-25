# C Brainfuck Compiler

Hobby project brainfuck compiler in c to mess with my brain and get better at programming.

Idea is simple and quite stupid
1) Lex the brainfuck code
2) Tranform the token stream to XX code
3) Write XX code to file and compile XX code
4) You now have a compiled binary based on the brainfuck source code
5) ????
6) Profit

## Usage

Compile compiler (!)
```bash
gcc src/main.c -o bigbrains
```

Compile brainfuck
```bash
./bin/bigbrains examples/hello-world-short.bf
```

## TODO

- [ ] Currently just executes the code, choose assembly type and turn token-stream into that
- [ ] Maybe https://flatassembler.net/ ?