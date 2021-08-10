# Project 6: Assembler

[Specification](https://www.nand2tetris.org/project06)

An assembler written in C++. Naively assumes error-free assembly code(No error-checking).

* [Hack Machine Language](https://b1391bd6-da3d-477d-8c01-38cdf774495a.filesusr.com/ugd/44046b_d70026d8c1424487a451eaba3e372132.pdf)
* [Hack Assembly](https://b1391bd6-da3d-477d-8c01-38cdf774495a.filesusr.com/ugd/44046b_b73759b866b249a0b3a715bf5a18f668.pdf)

Compilation: `g++ -std=c++17 main.cpp parser.cpp code_generators.cpp encoder.cpp -o Assembler`

Execution: `./Assembler inputfile.asm`

Outputs a file inputfile.hack with each assembly statement translated to native Hack machine language.

```
$ cat Add.asm
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/06/add/Add.asm

// Computes R0 = 2 + 3  (R0 refers to RAM[0])

@2
D=A
@3
D=D+A
@0
M=D

$ ./Assembler Add.asm
$ cat Add.hack
0000000000000010
1110110000010000
0000000000000011
1110000010010000
0000000000000000
1110001100001000
```