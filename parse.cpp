#include "parse.h"
#include "gettoken.h"

static int error_count = 0;

void
ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

// Prog is an Slist
ParseTree *Prog(istream& in, int& line)
{
	ParseTree *sl = Slist(in, line);
    
	if( GetToken::Get(in, line) != DONE )
		ParseError(line, "Unrecognized statement");

	if( sl == 0 )
		ParseError(line, "No statements in program");

	if( error_count )
    {
		return 0;
    }
	return sl;
}

// Ssep { Slist } | Stmt Ssep { Slist }
ParseTree *Slist(istream& in, int& line) {
	ParseTree *s = Stmt(in, line);
	if( s == 0 )
		return 0;

	Token t;
	if( (t = GetToken::Get(in, line)) != SC && t != NL ) {
		ParseError(line, "Missing statement separator");
		return 0;
	}

    return new StmtList(s, Slist(in,line));
}

ParseTree *Stmt(istream& in, int& line) {
	ParseTree *s = 0;

	Token t = GetToken::Get(in, line);
	switch( t.GetTokenType() ) {
	case IF:
		s = IfStmt(in, line);
		break;

	case PRINT:
		s = PrintStmt(in, line);
		break;

	case SET:
		s = SetStmt(in, line);
		break;

	case LOOP:
		s = LoopStmt(in, line);
		break;

	case ERR:
		ParseError(line, "Invalid token");
		break;

	case DONE:
		break;

	case NL:
	case SC:
		// an empty statement; get the next one
		s = Stmt(in, line);
		break;

	default:
		GetToken::PushBack(t);
		break;
	}

	return s;
}

ParseTree *IfStmt(istream& in, int& line) {
	// add code to parse an if
    ParseTree *t1 = Expr(in, line);
    if (t1 == 0)
    {
        ParseError(line, "Missing expression after if");
        return 0;
    }
    
    
    Token t = GetToken::Get(in, line);
    if (t.GetTokenType() != BEGIN)
    {
        ParseError(line, "Missing BEGIN after expression");
        return 0;
    }

    ParseTree *t2 = Slist(in, line);
    if (t2 == 0)
    {
        return 0;
    }


    t = GetToken::Get(in, line);
    if (t.GetTokenType() != END)
    {
        ParseError(line, "IfStmnt: Expecting tokentype END");
        return 0;
    }

    return new IfStatement(line, t1, t2);
}

ParseTree *LoopStmt(istream& in, int& line) {
	// add code to parse a loop        Code here should(?) be near identical to IfStmnt
    ParseTree *t1 = Expr(in, line);
    if (t1 == 0)
    {
        return 0;
    }
    
    Token t = GetToken::Get(in, line);
    if (t.GetTokenType() != BEGIN)
    {
        ParseError(line, "LoopStmnt: Expecting tokentype BEGIN");
        return 0;
    }

    ParseTree *t2 = Slist(in, line);
    if (t2 == 0)
    {
        ParseError(line, "Missing statement list after BEGIN");
        return 0;
    }

    t = GetToken::Get(in, line);
    if (t.GetTokenType() != END)
    {
        ParseError(line, "LoopStmnt: Expecting tokentype END");
        return 0;
    }

    return new LoopStatement(line, t1, t2);
}

ParseTree *SetStmt(istream& in, int& line) {
	// add code to parse a set
    Token t = GetToken::Get(in, line);
    if (t.GetTokenType() != ID)
    {
        ParseError(line, "Missing ID after set");
        return 0;
    }
    
    ParseTree *t1 = Expr(in, line);
    if (t1 == 0)
    {
        ParseError(line, "Missing expression after id");
        return 0;
    }
    
    return new SetStatement(line, t.GetLexeme(), t1);
}

ParseTree *PrintStmt(istream& in, int& line) {
	// add code to parse a print
    ParseTree *t1 = Expr(in, line);
    if (t1 == 0)
    {
        ParseError(line, "Missing expression after print");
        return 0;
    }
    
    return new PrintStatement(line, t1);
}

ParseTree *Expr(istream& in, int& line) {
	ParseTree *t1 = Prod(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Token t = GetToken::Get(in, line);

		if( t != PLUS && t != MINUS ) {
			GetToken::PushBack(t);
			return t1;
		}

		ParseTree *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == PLUS )
			t1 = new Addition(t.GetLinenum(), t1, t2);
		else
			t1 = new Subtraction(t.GetLinenum(), t1, t2);
	}
}

ParseTree *Prod(istream& in, int& line) {
	ParseTree *t1 = Primary(in, line);

	// needs more...
    if(t1 == 0) {
        return 0;
    }
    
    while (true) {
        Token t = GetToken::Get(in, line);
        
        if(t != STAR && t != SLASH ) {
            GetToken::PushBack(t);
            return t1;
        }
        
        ParseTree *t2 = Primary(in, line);
        if(t2 == 0) {
            ParseError(line, "Missing expression after operator");
            return 0;
        }
        
        if(t == STAR)
            t1 = new Multiplication(t.GetLinenum(), t1, t2);
        else
            t1 = new Division(t.GetLinenum(), t1, t2);
    }
}

ParseTree *Primary(istream& in, int& line) {
	Token t = GetToken::Get(in, line);
    ParseTree *t1 = 0;

	// needs more...
    switch (t.GetTokenType())
    {
        case (ID):
            return new Ident(t);
            break;

        case (ICONST):
            return new IConst(t);
            break;

        case (SCONST):
            return new SConst(t);
            break;

        case (LPAREN):
            t1 = Expr(in, line);
            
            t = GetToken::Get(in, line);
            if (t.GetTokenType() != RPAREN)
            {
                ParseError(line, "Missing ) after expression");
                return 0;
            }
            break;
            
        default:
            ParseError(line, "Primary expected");
            return 0;
    }
    
    if (t1 == 0)
    {
        return 0;
    }
    
    return t1;
    
}