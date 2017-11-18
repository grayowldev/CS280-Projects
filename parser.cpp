/*
 * parser.cpp
 *
 */


#include <string>
using std::string;
using namespace std;

#include "parser.h"

class ParserToken {
private:
	Token	tok;
	bool	pushedBack;

public:
	ParserToken() : pushedBack(false) {}

	Token getToken(istream *in) {
		if( pushedBack ) {
			pushedBack = false;
			return tok;
		}

		return ::getToken(in);
	}
	void pushbackToken(const Token& t) {
		if( pushedBack ) {
			// error
		}
		tok = t;
		pushedBack = true;
	}
} ParserToken;

ParseTree *
Prog(istream* in)
{
    cout << "In Prog"<<endl;
	return StmtList(in);
}

ParseTree *
StmtList(istream* in)
{
    cout << "StmtList " << endl;
	ParseTree *stmt = Stmt(in);
	if( stmt == 0 )
		return 0;

	return new StatementList( stmt, StmtList(in) );
}

ParseTree *	Stmt(istream* in) {
    cout << "Stmt " << endl;
    ParseTree *decl = Decl(in);
    ParseTree *set = Set(in);
    ParseTree *print = Print(in);
    Token token = ParserToken.getToken(in);
    cout << "Got token: " << token << endl;
    
    if (decl != 0) {
        cout << "In Decl SC check\n";
        cout << "decl token " << token << endl;
        if (token == T_SC){
            cout << "return decl vv" << endl;
            return decl;
        }
    } else if (set != 0) {
        cout << "In Set\n";
        if (token == T_SC){
            cout << "return set vv" << endl;
            return set;
        }
    }
    else if (print != 0) {
        cout << "In Print\n";
        if (token == T_SC){
            cout << "return print vv" << endl;
            return print;
        }
        
    }
	return 0;
}

ParseTree *	Decl(istream* in) {
    cout << "Decl" << endl;
    Token token = ParserToken.getToken(in);
    cout << "got next token " << token << endl;
    if (token == T_INT) {
        ParseTree *expression = Expr(in);
        cout << "returned DeclClass" << endl;
        return new DeclClass(expression->getLineNumber(),expression);
    }else if (token == T_STRING){
        ParseTree *expression = Expr(in);
        cout << "returned DeclClass" << endl;
        return new DeclClass(expression->getLineNumber(),expression);
    }
    else {
        ParserToken.pushbackToken(token);
    }
	return 0;
}

ParseTree *	Set(istream* in) {
    Token token = ParserToken.getToken(in);
    if (token == T_SET) {
        ParseTree *idExpression = Expr(in);
        if (idExpression->GetType() == ID_TYPE){
            ParseTree *expression = Expr(in);
            return new SetClass(expression->getLineNumber(), idExpression,expression);
        }
    }
    
    ParserToken.pushbackToken(token);
	return 0;
}



ParseTree *	Print(istream* in) {
    Token token = ParserToken.getToken(in);
    if (token == T_PRINT || token == T_PRINTLN) {
        ParseTree *expression = Expr(in); // connect stream to expression
        return new PrintClass(expression->getLineNumber(),expression);
    }else {
        ParserToken.pushbackToken(token);
    }
	return 0;
}

ParseTree *Expr(istream* in) {
    //Expression only handles the + -
	ParseTree *t1 = Term(in);  // connect stream to term
	if( t1 == 0 )
		return 0;

	for(;;) {
		Token op = ParserToken.getToken(in);
        cout << "got token(in Expr): " << op << endl;
		if( op != T_PLUS && op != T_MINUS ) {
            ParserToken.pushbackToken(op);
            cout << "putback" << endl;
			return t1;
		}

		ParseTree *t2 = Expr(in);
		if( t2 == 0 ) {
			error(op.GetLinenum(), "expression required after + or - operator");
			return 0;
		}

		// combine t1 and t2 together
		if( op == T_PLUS )
			t1 = new Addition(op.GetLinenum(), t1, t2);
		else
			t1 = new Subtraction(op.GetLinenum(), t1, t2);
	}

	// should never get here...
	return 0;
}

ParseTree *	Term(istream* in) {
    //Expression only handles the + -
    ParseTree *prime = Primary(in);  // connect stream to term
    cout << "primary |" << endl;
    if( prime == 0 )
        return 0;
    
    for(;;) {
        Token op = ParserToken.getToken(in);
        cout << "got token(in Term): " << op << endl;
        if( op != T_STAR && op != T_SLASH ) {
            cout << "putback\n";
            ParserToken.pushbackToken(op);
            return prime;
        }
        
        ParseTree *t2 = Term(in);
        if( t2 == 0 ) {
            error(op.GetLinenum(), "expression required after * or / operator");
            return 0;
        }
        
        // combine t1 and t2 together
        if( op == T_STAR )
            prime = new Multiply(op.GetLinenum(), prime, t2);
        else
            prime = new Divide(op.GetLinenum(), prime, t2);
        
    }
    
    // should never get here...
    return 0;
}

ParseTree *	Primary(istream* in) {
    Token token = ParserToken.getToken(in);
    cout << "got: " << token << endl;
    if (token == T_ICONST) {
        return new IntegerConstant(token);
    }else if (token == T_SCONST){
        return new StringConstant(token);
    }else if (token == T_ID){
        cout << "got an id " << token <<  endl;
        return new Identifier(token);
    }else if (token==T_LPAREN){
        cout << "get a ( " << endl;
        ParseTree *expression = Expr(in);
        Token token = ParserToken.getToken(in);
        if (token == T_RPAREN){
            cout << "found a ) " << endl;
            return new ParanClass(expression->getLineNumber(),expression);
        }
    }
    extern string arg;
    //cout << arg << ":" << endl;
    //error(token.GetLinenum(), "Syntax error primary expected");
	return 0;
}

