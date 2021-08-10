# Project 10: Compiler I: Syntax Analysis

[Specification](https://www.nand2tetris.org/project10)


A recursive-descent source code(Jack) analyzer written in Python 3. Naively assumes error-free Jack code(No error-checking).

Execution: `python3 JackAnalyzer.py [inputfile.jack|DirectoryName]` 

Outputs an xml file of the tokens it has read, one for each .jack file.
```
$ ls Square
Main.jack  SquareGame.jack  Square.jack
```
```
$ cat Square/Main.jack
...
...
...
class Main {
    static boolean test;    // Added for testing -- there is no static keyword
                            // in the Square files.
    function void main() {
      var SquareGame game;
      let game = SquareGame.new();
      do game.run();
      do game.dispose();
      return;
    }

    function void test() {  // Added to test Jack syntax that is not use in
        var int i, j;       // the Square files.
        var String s;
        var Array a;
        if (false) {
            let s = "string constant";
            let s = null;
            let a[1] = a[2];
        }
        else {              // There is no else keyword in the Square files.
            let i = i * (-j);
            let j = j / (-2);   // note: unary negate constant 2
            let i = i | j;
        }
        return;
    }
}
```
```
$ python3 JackAnalyzer.py Square
$ ls Square
Main.jack  Main.xml  Square.jack  Square.xml  SquareGame.jack  SquareGame.xml
```
```
$ cat Square/Main.xml
<class>
<keyword> class </keyword>
<identifier> Main </identifier>
<symbol> { </symbol>
<classVarDec>
<keyword> static </keyword>
<keyword> boolean </keyword>
<identifier> test </identifier>
<symbol> ; </symbol>
</classVarDec>
<subroutineDec>
<keyword> function </keyword>
<keyword> void </keyword>
<identifier> main </identifier>
<symbol> ( </symbol>
<parameterList>
</parameterList>
<symbol> ) </symbol>
<subroutineBody>
<symbol> { </symbol>
<varDec>
<keyword> var </keyword>
<identifier> SquareGame </identifier>
<identifier> game </identifier>
<symbol> ; </symbol>
</varDec>
<statements>
<letStatement>
<keyword> let </keyword>
<identifier> game </identifier>
<symbol> = </symbol>
<expression>
<term>
<identifier> SquareGame </identifier>
<symbol> . </symbol>
<identifier> new </identifier>
<symbol> ( </symbol>
<expressionList>
</expressionList>
<symbol> ) </symbol>
</term>
</expression>
<symbol> ; </symbol>
</letStatement>
...
...
...
<letStatement>
<keyword> let </keyword>
<identifier> i </identifier>
<symbol> = </symbol>
<expression>
<term>
<identifier> i </identifier>
</term>
<symbol> | </symbol>
<term>
<identifier> j </identifier>
</term>
</expression>
<symbol> ; </symbol>
</letStatement>
</statements>
<symbol> } </symbol>
</ifStatement>
<returnStatement>
<keyword> return </keyword>
<symbol> ; </symbol>
</returnStatement>
</statements>
<symbol> } </symbol>
</subroutineBody>
</subroutineDec>
<symbol> } </symbol>
</class>
```