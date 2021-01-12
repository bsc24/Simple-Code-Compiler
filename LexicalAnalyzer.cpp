#include "tokens.h"
Token getNextToken(istream& in, int& linenum)
{
    //cp $LIB/public/p3/cases.tar.gz .
    
    char ch;
    string word = "";
    string rawInput;
    TokenType type = ERR;
    bool firstChar = true;
    bool inId = false;
    bool inInt = false;
    bool inString = false;
    bool inComment = false;
    bool finished = false;
    while (in.get(ch))
    {
        
        rawInput += ch;
        if (firstChar)
        {
            word += ch;
            firstChar = false;
            if (word == "+")    //PLUS
            {
                type = PLUS;
                finished = true;
            }
            else if (word == "-")    //MINUS
            {
                type = MINUS;
                finished = true;
            }
            else if (word == "*")    //STAR
            {
                type = STAR;
                finished = true;
            }
            else if (word == "/")    //SLASH
            {
                type = SLASH;
                finished = true;
            }
            else if (word == "(")    //LPAREN
            {
                type = LPAREN;
                finished = true;
            }
            else if (word == ")")    //RPAREN
            {
                type = RPAREN;
                finished = true;
            }
            else if (word == ";")    //SC
            {
                type = SC;
                finished = true;
            }
            else if (word == "\n")    //NL
            {
                type = NL;
                linenum++;
                finished = true;
            }
            else if (isalpha(word[0]))    //ID
            {
                type = ID;
                inId = true;
            }
            else if (isdigit(word[0]))    //ICONST
            {
                type = ICONST;
                inInt = true;
            }
            else if (word[0] == '"')    //SCONST
            {
                type = SCONST;
                word = "";
                inString = true;
            }
            else if (word[0] == '#')    //comments
            {
                inComment = true;
                word = "";
            }
            else if (isspace(ch))
            {
                word = rawInput = "";
                firstChar = true;
            }
            else
            {
                type = ERR;
                finished = true;
            }
        }
        else if (inId)    //ID
        {
            if (isalnum(ch))
            {
                word += ch;
            }
            else if (isspace(ch) || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ispunct(ch))
            {
                in.putback(ch);
                inId = false;
                finished = true;
            }
            else
            {
                word += ch;
                type = ERR;
                inId = false;
                finished = true;
            }
        }
        else if (inInt)    //ICONST
        {
            if (isdigit(ch))
            {
                word += ch;
            }
            else if (isalpha(ch))
            {
                word += ch;
                type = ERR;
                inInt = false;
                finished = true;
            }
            else if (isspace(ch) || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ispunct(ch))
            {
                in.putback(ch);
                inInt = false;
                finished = true;
            }
            else
            {
                in.putback(ch);
                inInt = false;
                finished = true;
            }
        }
        else if (inString)    //SCONST
        {
            if (ch == '\\')    //
            {
                in.get(ch);
                if (ch == 'n')
                {
                    word += '\n';
                }
                else
                {
                    word += ch;
                }
            }
            else if (ch == '"')    //encounter another double quote
            {
                inString = false;
                finished = true;
            }
            else if (ch == '\n')    //must be all on the same line, causes error
            {
                word += ch;
                type = ERR;
                inString = false;
                finished = true;
            }
            else
            {
                word += ch;
            }
        }
        else if (inComment)    //comment, no tokentype
        {
            if (ch != '\n')
            {
                continue;
            }
            else    //once a newline appears
            {
                in.putback(ch);
                firstChar = true;
            }
        }
        
        if (finished)
        {
            if (type == ID)
            {
                if (word == "print")    //PRINT
                {
                    type = PRINT;
                }
                else if (word == "set")    //SET
                {
                    type = SET;
                }
                else if (word == "if")    //IF
                {
                    type = IF;
                }
                else if (word == "loop")    //LOOP
                {
                    type = LOOP;
                }
                else if (word == "begin")    //BEGIN
                {
                    type = BEGIN;
                }
                else if (word == "end")    //END
                {
                    type = END;
                }
            }
            else if (type == ERR)
            {
                word = rawInput;
            }

            Token holder(type, word, linenum);

            return holder;
        }
    }
    
    Token holder(DONE, "", linenum);
    return holder;
    
}


ostream& operator<<(ostream& out, const Token& tok)
{
    switch (tok.GetTokenType())
    {
        case PRINT: out << "PRINT";
            break;
        case SET: out << "SET";
            break;
        case IF: out << "IF";
            break;
        case LOOP: out << "LOOP";
            break;
        case BEGIN: out << "BEGIN";
            break;
        case END: out << "END";
            break;
        case ID: out << "ID";
            break;
        case ICONST: out << "ICONST";
            break;
        case SCONST: out << "SCONST";
            break;
        case PLUS: out << "PLUS";
            break;
        case MINUS: out << "MINUS";
            break;
        case STAR: out << "STAR";
            break;
        case SLASH: out << "SLASH";
            break;
        case LPAREN: out << "LPAREN";
            break;
        case RPAREN: out << "RPAREN";
            break;
        case SC: out << "SC";
            break;
        case NL: out << "NL";
            break;
        case ERR: out << "ERR";
            break;
        case DONE: out << "DONE";
            break;
    }
    
    /*
    string*[] types = {"PRINT", "SET", "IF", "LOOP", "BEGIN", "END", "ID", "ICONST", "SCONST", "PLUS", "MINUS", "STAR", "SLASH", "LPAREN", "RPAREN", "SC", "NL", "ERR", "DONE"};
    out << types[tok.GetTokenType()];
    */
    
    if (tok.GetTokenType() == ID || tok.GetTokenType() == ICONST || tok.GetTokenType() == SCONST)
    {
        out << "(" + tok.GetLexeme() + ")";
    }
    
    return out;
}
