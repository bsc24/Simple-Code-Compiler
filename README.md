# Simple Code Compiler 
# By Brandon S. Chin

Written in C++, this compiler uses a recursive descent parser and lexical analyzer to parse and interpret a basic language.
Within this repository, there are also example testcases located in the _testcases_ folder with the expected output of each testcase located in the _expected outputs_.


## Language
This compiler works using the following language and grammer:

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
