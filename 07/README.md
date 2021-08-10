# Project 7: VM I: Stack Arithmetic

[Specification](https://www.nand2tetris.org/project07)

A VM Translator written in C++. Translates vm files to assembly. Naively assumes error-free vm code(No error-checking).

Offical documentation for the VM language can be found in the book. 

Part I deals with arithmetic and memory access commands.

__Arithmetic / Logical commands__:
* add
* sub
* neg
* eq
* gt
* lt
* and
* or
* not

__Memory Access commands__:
* push *segment i*
* pop *segment i*

__Memory Segments__:
* constant
* local
* argument
* this
* that
* temp
* static 
* pointer

Compilation: `g++ -std=c++17 main.cpp parser.cpp code_writer.cpp code_generators.cpp -o VMTranslator`

Execution: `./VMTranslator inputfile.vm`

Outputs a file inputfile.asm.

```
$ cat StaticTest.vm
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/07/MemoryAccess/StaticTest/StaticTest.vm

// Executes pop and push commands using the static segment.
push constant 111
push constant 333
push constant 888
pop static 8
pop static 3
pop static 1
push static 3
push static 1
sub
push static 8
add

$ ./VMTranslator StaticTest.vm
$ ./cat StaticTest.asm
// PUSH CONSTANT
@111
D=A
@SP
A=M
M=D
@SP
M=M+1
// PUSH CONSTANT
@333
D=A
@SP
A=M
M=D
@SP
M=M+1
...
...
...
// PUSH STATIC
@StaticTest.8
D=M
@SP
A=M
M=D
@SP
M=M+1
// ADD
@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
(INF)
@INF
0;JMP
```