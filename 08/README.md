# Project 8: VM II: Program Control

[Specification](https://www.nand2tetris.org/project08)

A VM Translator written in C++. Translates vm files to assembly. Naively assumes error-free vm code(No error-checking).

Offical documentation for the VM language can be found in the book. 

Part II deals with branching and function commands.

__Branching commands__:
* label *label* 
* goto *label* 
* if-goto *label* 

__Function commands__:
* function *functionName nVars*
* call *functionName nArgs*
* return

Compilation: `g++ -std=c++17 main.cpp parser.cpp code_writer.cpp code_generators.cpp -o VMTranslator`

Execution: `./VMTranslator [inputfile.vm|DirectoryName]`

Outputs a file inputfile.asm or DirectoryName.asm.

```
$ ls StaticsTest
Class1.vm  Class2.vm  Sys.vm
```
```
$ cat StaticsTest/Class1.vm
....
// Stores two supplied arguments in static[0] and static[1].
function Class1.set 0
push argument 0
pop static 0
push argument 1
pop static 1
push constant 0
return

// Returns static[0] - static[1].
function Class1.get 0
push static 0
push static 1
sub
return
```
```
$ cat StaticsTest/Class2.vm
...
// Stores two supplied arguments in static[0] and static[1].
function Class2.set 0
push argument 0
pop static 0
push argument 1
pop static 1
push constant 0
return

// Returns static[0] - static[1].
function Class2.get 0
push static 0
push static 1
sub
return
```
```
$ cat StaticsTest/Sys.vm
// Tests that different functions, stored in two different 
// class files, manipulate the static segment correctly. 
function Sys.init 0
push constant 6
push constant 8
call Class1.set 2
pop temp 0 // Dumps the return value
push constant 23
push constant 15
call Class2.set 2
pop temp 0 // Dumps the return value
call Class1.get 0
call Class2.get 0
label WHILE
goto WHILE
```
```
$ ./VMTranslator StaticsTest
$ ls StaticsTest
Class1.vm  Class2.vm  StaticsTest.asm  Sys.vm
```
```
$ cat StaticsTest/StaticsTest.asm
@256
D=A
@SP
M=D
// CALL
@RETURN_Sys.init_0
D=A
@SP
A=M
M=D
@SP
M=M+1
...
...
...
@Sys.init
0;JMP
(RETURN_Sys.init_0)
(Class1.set)
@0
D=A
@SP
A=M
D=A
@SP
M=D
...
...
...
(Class2.set)
@0
D=A
@SP
A=M
D=A
@SP
M=D
...
...
...
(Sys.init)
@0
D=A
@SP
A=M
D=A
@SP
M=D
...
...
...
...
(RETURN_Class2.get_4)
(WHILE)
@WHILE
0;JMP
(INF)
@INF
0;JMP
```