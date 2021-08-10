// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

(WAIT_FOR_KEYDOWN)
@KBD
D=M
@PAINT_BLACK
D;JNE
@WAIT_FOR_KEYDOWN
0;JMP

(PAINT_BLACK)
@fill_value
M=-1
@PAINT
0;JMP

(WAIT_FOR_KEYUP)
@KBD
D=M
@PAINT_WHITE
D;JEQ
@WAIT_FOR_KEYUP
0;JMP

(PAINT_WHITE)
@fill_value
M=0
@PAINT
0;JMP

(PAINT)
@iterator
M=0

(PAINT_LOOP)
@iterator
D=M
@8192  // Total number of words in Screen Memory Map
D=A-D
@END_PAINT_LOOP
D;JEQ
@iterator
D=M
@SCREEN
D=D+A
@current_memory_word
M=D
@fill_value
D=M
@current_memory_word
A=M
M=D
@iterator
M=M+1
@PAINT_LOOP
0;JMP

(END_PAINT_LOOP)
@fill_value
D=M
@WAIT_FOR_KEYDOWN
D;JEQ
@WAIT_FOR_KEYUP
0;JMP
