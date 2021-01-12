# Simple Code Compiler 
# By Brandon S. Chin

Written in C++, **Simple Code Compiler** uses a recursive descent parser and lexical analyzer to parse and interpret a basic language.
Within this repository, there are also example testcases located in the _testcases_ folder with the expected output of each testcase located in the _expected outputs_ folder.


## Use instructions

After compiling the code into an executable (or downloading the "Simple Code Compiler.exe" file), the executable may be run from the command line.
**Simple Code Compiler** may take one command line argument of a file containing code that fits the simple language (shown below).
If no additional command line argument is provided, **Simple Code Compiler** will instead use standard input.
Upon reaching the end of file (if a file was provided) or the end of standard input (by inputting Ctrl + z), the code provided will be parsed, interpreted, and run if no errors exist.
If errors exist in the code provided, **Simple Code Compiler** will give errors showing the line number it encountered an error on as well as what error it ran into.


## Language
**Simple Code Compiler** utilizes the following language and grammer:

* Prog := Slist
* Slist := Ssep { Slist } | Stmt Ssep { Slist }
* Ssep := NL | SC
* Stmt := IfStmt | PrintStmt | SetStmt | LoopStmt
* IfStmt := IF Expr BEGIN Slist END
* PrintStmt := PRINT Expr
* SetStmt := SET IDENT Expr
* LoopStmt := LOOP Expr BEGIN Slist END
* Expr := Prod { (PLUS | MINUS) Prod }
* Prod := Primary { (STAR | SLASH) Primary }
* Primary := IDENT | ICONST | SCONST | LPAREN Expr RPAREN


## Credits
The following parts of this project were provided entirely or partially as a starter skeleton code by the late Professor Gerard Ryan:
* gettoken.cpp
* gettoken.h
* parse.h
* tokens.h
* tree.h
* value.h
* All testcases and expected outputs
