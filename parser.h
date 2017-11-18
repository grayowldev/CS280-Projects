/*
 * parser.h
 *
 *  Created on: Oct 23, 2017
 *      Author: gerardryan
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
using std::istream;

#include <string>
using std::string;
using std::stoi;
using namespace std;
#include "lexer.h"

extern void error(int linenum, const string& message);

enum TypeForNode { INT_TYPE, STRING_TYPE, ERROR_TYPE, ID_TYPE, STAR_TYPE, ADD_TYPE, SET_TYPE,DEC_TYPE};

class ParseTree {
	int			linenumber;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int n, ParseTree *l = 0, ParseTree *r = 0) : linenumber(n), left(l), right(r) {}
	virtual ~ParseTree() {}

	ParseTree* getLeft() const { return left; }
	ParseTree* getRight() const { return right; }
	int getLineNumber() const { return linenumber; }

	virtual TypeForNode GetType() const { return ERROR_TYPE; }
	virtual int GetIntValue() const { throw "no integer value"; }
	virtual string GetStringValue() const { throw "no string value"; }
    virtual int starCount(){return 0;}
    virtual int plusCount(){return 0;}
};

class StatementList : public ParseTree {
public:
	StatementList(ParseTree *first, ParseTree *rest) : ParseTree(0, first, rest) {}

};
class PrintClass : public ParseTree {
public:
    PrintClass(int line, ParseTree *first) : ParseTree(line, first, 0) {}
};
class DeclClass : public ParseTree {
public:
    DeclClass(int line, ParseTree *first) : ParseTree(line, first, 0) {}
    TypeForNode GetType() const { return DEC_TYPE; }
};


class SetClass : public ParseTree {
public:
    SetClass(int line, ParseTree *first, ParseTree *second) : ParseTree(line, first, second) {}
    TypeForNode GetType() const { return SET_TYPE; }
};

class ParanClass : public ParseTree {
public:
    ParanClass(int line, ParseTree *op1) : ParseTree(line, op1, 0) {}
    
    // will need to fill in type and value;
    // remember type is a function of
};
class Addition : public ParseTree {
public:
	Addition(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	// will need to fill in type and value;
    int plusCount(){return 1;}   //*****************
    TypeForNode GetType() const { return ADD_TYPE; }
    
	// remember type is a function of
};

class Subtraction : public ParseTree {
public:
	Subtraction(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	// will need to fill in type and value;
	// remember type is a function of
};

class Multiply : public ParseTree {
public:
    Multiply(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}
    
    // will need to fill in type and value;
    int starCount(){return 1;}   //*****************
    TypeForNode GetType() const { return STAR_TYPE; }
    
    // remember type is a function of
};
class Divide : public ParseTree {
public:
    Divide(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}
    
    // will need to fill in type and value;
    // remember type is a function of
};


class IntegerConstant : public ParseTree {
	int	value;

public:
	IntegerConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
		value = stoi( tok.GetLexeme() );
	}

	TypeForNode GetType() const { return INT_TYPE; }
	int GetIntValue() const { return value; }
};

class StringConstant : public ParseTree {
    string    value;
    
public:
    StringConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
        value =  tok.GetLexeme() ;
    }
    
    TypeForNode GetType() const { return STRING_TYPE; }
    string GetStringValue() const { return value; }
};

class Identifier : public ParseTree {
    string    value;
    
public:
    Identifier(const Token& tok) : ParseTree(tok.GetLinenum()) {
        value =  tok.GetLexeme() ;
    }
    
    TypeForNode GetType() const { return ID_TYPE; }
    string GetStringValue() const { return value; }
    
};


extern ParseTree *	Prog(istream* in);
extern ParseTree *	StmtList(istream* in);
extern ParseTree *	Stmt(istream* in);
extern ParseTree *	Decl(istream* in);
extern ParseTree *	Set(istream* in);
extern ParseTree *	Print(istream* in);
extern ParseTree *	Expr(istream* in);
extern ParseTree *	Term(istream* in);
extern ParseTree *	Primary(istream* in);


#endif /* PARSER_H_ */
