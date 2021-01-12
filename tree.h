/*
 * parsetree.h
 */

#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <map>
#include <string>
#include "value.h"
using std::vector;
using std::map;
using std::string;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };
static map<string, Value> identifiers;

// a "forward declaration" for a class to hold values
class Value;

class ParseTree {
	int		linenum;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

    virtual NodeType GetType() const { return ERRTYPE; }
    virtual bool IsOp() const { return false; }
	virtual bool IsString() const { return false; }
    virtual bool IsIdent() const { return false; }
    virtual Value Eval() const { return Value(); }
    
	int GetLinenum() const { return linenum; }
	ParseTree *GetLeft() const { return left; }
	ParseTree *GetRight() const { return right; }

	int NodeCount() const {
		int nc = 0;
		if( left ) nc += left->NodeCount();
		if( right ) nc += right->NodeCount();
		return nc + 1;
	}
    
    //added by me
    int InteriorCount() const
    {
        int ic = 0;
        if (left) ic += left->InteriorCount();
        if (right) ic += right->InteriorCount();
        
        if (!left && !right)
            return 0;
        else
            return ic + 1;
    }
    
    int OpCount() const
    {
        int oc = 0;
        if (left)  oc += left->OpCount();
        if (right) oc += right->OpCount();
        
        if (this->IsOp()) oc++;
        
        return oc;
    }
    
    //huh looks like this one is useless for Part 1... pretty sure it works as intended though
    int IdentCount() const
    {
        int idc = 0;
        if (left) idc += left->OpCount();
        if (right) idc += right->OpCount();
        
        if (this->IsIdent()) idc++;
        
        return idc;
    }
    
    int StringCount() const
    {
        int sc = 0;
        if (left) sc += left->StringCount();
        if (right) sc += right->StringCount();
        
        if (this->IsString()) sc++;
        
        return sc;
    }
    
    int DepthCount() const
    {
        int ldepth = 0;
        int rdepth = 0;
        if (left) ldepth = left->DepthCount();
        if (right) rdepth = right->DepthCount();
        
        if (rdepth > ldepth)
            return rdepth + 1;
        else
            return ldepth + 1;
    }
    
};


class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    
    
    Value Eval() const {
        ParseTree *leftie = this->GetLeft();
        ParseTree *rightie = this->GetRight();
        if (leftie) { leftie->Eval(); }
        if (rightie) { rightie->Eval(); }
        
        return 0;
    }

};


class IfStatement : public ParseTree {
public:
	IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
    
    //Evalue expression, if expression != 0 proceed with statement
    Value Eval() const {
        ParseTree *ex = this->GetLeft();
        ParseTree *stmnt = this->GetRight();
        Value expr = ex->Eval();
        
        if (!expr.isInt())
        {
            throw std::logic_error("Conditional is not an integer");
        }
        
        if (expr.GetInt() != 0)
        {
            return stmnt->Eval();
        }
        
        return 0;
        //return Value(); //placeholder
    }
};


class SetStatement : public ParseTree {
	string id;

public:
	SetStatement(int line, string id, ParseTree *expr) : ParseTree(line, expr), id(id) {}
    
    Value Eval() const 
    {
        ParseTree *expr = this->GetLeft();
        Value ex = expr->Eval();
        if (ex.isInt()) { identifiers[id] = ex.GetInt(); }
        else { identifiers[id] = ex.GetStr(); }
        
        return 0;
        //return Value();    //placeholder
    }
};


class PrintStatement : public ParseTree {
public:
	PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
    
    Value Eval() const {
        ParseTree *e = this->GetLeft();
        Value expr = e->Eval();
        
        cout << expr;
        return 0;
        //return Value(); //placeholder
    }
};


class LoopStatement : public ParseTree {
public:
	LoopStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
    
    Value Eval() const {
        ParseTree *ex = this->GetLeft();
        ParseTree *stmnt = this->GetRight();
        
        while (ex->Eval().GetInt()) { stmnt->Eval(); }
        
        return 0;
        //return Value(); //placeholder
    }
};


class Addition : public ParseTree {
public:
	Addition(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    bool IsOp() const { return true; }
    
    Value Eval() const {
        ParseTree *leftie = this->GetLeft();
        ParseTree *rightie = this->GetRight();
        return leftie->Eval() + rightie->Eval();
    }
};


class Subtraction : public ParseTree {
public:
	Subtraction(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    bool IsOp() const { return true; }
    
    Value Eval() const {
        ParseTree *leftie = this->GetLeft();
        ParseTree *rightie = this->GetRight();
        return leftie->Eval() - rightie->Eval();
    }
};


class Multiplication : public ParseTree {
public:
	Multiplication(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    bool IsOp() const { return true; }
    
    Value Eval() const {
        ParseTree *leftie = this->GetLeft();
        ParseTree *rightie = this->GetRight();
        return leftie->Eval() * rightie->Eval();
    }
};


class Division : public ParseTree {
public:
	Division(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    bool IsOp() const { return true; }
    
    Value Eval() const {
        ParseTree *leftie = this->GetLeft();
        ParseTree *rightie = this->GetRight();
        return leftie->Eval() / rightie->Eval();
    }
};


class IConst : public ParseTree {
	int val;

public:
	IConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    NodeType GetType() const { return INTTYPE; }
    
    Value Eval() const { return Value(val); }
};


class SConst : public ParseTree {
	string val;

public:
	SConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}

	bool IsString() const { return true; }
    NodeType GetType() const { return STRTYPE; }
    
    Value Eval() const { return Value(val); }
};


class Ident : public ParseTree {
	string id;

public:
	Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	bool IsIdent() const { return true; }
    string GetId() const { return id; }
    
    
    Value Eval() const
    {
        
        if (identifiers.find(id) == identifiers.end())
        {
            throw std::logic_error("Symbol " + id + " not defined");
        }
        else
        {
            Value holder = identifiers[id];
            return holder;
        }
        
    }
    
};

#endif /* TREE_H_ */
