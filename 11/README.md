# Project 11: Compiler II: Code Generation

[Specification](https://www.nand2tetris.org/project11)

A recursive-descent Jack compiler written in Python 3. Naively assumes error-free Jack code(No error-checking).

Execution: `python3 JackCompiler.py [inputfile.jack|DirectoryName]`

Outputs a .vm file for each .jack file that it parses.

```
$ ls Square
Main.jack  Square.jack  SquareGame.jack
```
```
$ cat Square/Main.jack
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/11/Square/Main.jack

// (same as projects/09/Square/Main.jack)

/** Initializes a new Square Dance game and starts running it. */
class Main {
    function void main() {
        var SquareGame game;
        let game = SquareGame.new();
        do game.run();
        do game.dispose();
        return;
    }
}
```
```
$ python3 JackCompiler.py Square
$ ls Square
Main.jack  Main.vm  Square.jack  Square.vm  SquareGame.jack  SquareGame.vm
```
```
$ cat Square/Main.vm
function Main.main 1
call SquareGame.new 0
pop local 0
push local 0
call SquareGame.run 1
pop temp 0
push local 0
call SquareGame.dispose 1
pop temp 0
push constant 0
return
```