# Project 9: High-Level Language

[Specification](https://www.nand2tetris.org/project09)

Minesweeper written in the Jack programming language.

How to run: Compile the .jack files in the directory using the provided JackCompiler and load the directory in the provided VMEmulator. Choose the 'No animation' option in the 'Animate' drop down menu. Then click on run.

[Demo](https://www.youtube.com/watch?v=UaHtC1wUINE)

Official documentation for the Jack language can be found in Chapter 9 of the book.

Jack is a Java-like, object-based, and weakly-typed programming language implemented using a Stack virtual machine. 

__Some Characteristics of Jack__:
* Has only three primitive types: int, boolean, and char.
* Supports user-defined data types.
* No support for inheritance.
* There can only be one class per .jack file and class name should be identical to file name.
* No public class variables. All static and field member variables have to be accessed using getter and setter functions/methods.
* You can define multiple constructors of a class and name them differently.
* Syntax can be restrictive and verbose.
* No operator precedence.
* Generally aimed at being a sufficiently rich programming language while simplifying the process of compiler writing.