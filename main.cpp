#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "parser.h"

/*
 * main.cpp
 */

int lineNumber;

void error(int linenum, const string& message) {
    cout << linenum+1 << ":" << message;
}

int ID_Counter = 0;
int SET_Counter = 0;
int PLUS_Counter = 0;
int STAR_Counter = 0;

void postTrav(ParseTree* RootNode){
    //STAR_Counter += RootNode->starCount();
    
    //cout << "\n" << RootNode->GetType() << "\n";
    if (RootNode->getLeft()){
        cout << "L";
        postTrav(RootNode->getLeft());
        cout << "u";
    }
    if (RootNode->getRight()){
        cout << "R";
        postTrav(RootNode->getRight());
        cout << "U";
    }
    cout << "N";
}
void trav(ParseTree* RootNode){
    
    
    if(RootNode->GetType() == DEC_TYPE){
        ID_Counter++;
    }else if (RootNode->GetType() == SET_TYPE){
        SET_Counter++;
    } else if (RootNode->GetType() == STAR_TYPE){
        STAR_Counter++;
    }
    else if (RootNode->GetType() == ADD_TYPE){
        PLUS_Counter++;
    }
    
    if (RootNode->getLeft()){
        trav(RootNode->getLeft());
    }
    if (RootNode->getRight()){
        trav(RootNode->getRight());
    }
}

int
main(int argc, char *argv[])
{
    istream *in = &cin;
    ifstream file;
    bool vflag = false;
    
    for( int i=1; i<argc; i++ ) {
        string arg( argv[i] );
        if( arg == "-t" )
            vflag = true;
        else if( arg[0] == '-' ) {
            cerr << arg << " INVALID FLAG" << endl;
            return 0;
        }
        else if( in != &cin ) {
            cerr << "TOO MANY FILES" << endl;
            return 0;
        }
        else {
            file.open(arg);
            if( file.is_open() == false ) {
                cerr << arg << " FILE NOT FOUND" << endl;
                return 0;
            }
            
            in = &file;
        }
    }
    
    ParseTree *tree = Prog(in);
    
    if( tree == 0 ) {
        // there was some kind of parse error
        return 1;
    }
    trav(tree);
    
    if (vflag){
        postTrav(tree);
        cout << endl;
    }
    cout << "Total number of identifiers: "<< ID_Counter << endl;
    cout << "Total number of set: "<< SET_Counter << endl;
    cout << "Total number of +: "<< PLUS_Counter << endl;
    cout << "Total number of *: "<< STAR_Counter << endl;
    return 0;
}


///             Program Instructions

/*
                    Grammer
    Prog ::= StmtList
    StmtList ::= { Stmt T_SC } { StmtList }
    Stmt ::= Decl | Set | Print
    Decl ::= T_INT T_ID | T_STRING T_ID
    Set ::= T_SET T_ID Expr
    Print ::= T_PRINT Expr | T_PRINTLN Expr
    Expr ::= Term { (T_PLUS|T_MINUS) Expr }
    Term ::= Primary { (T_STAR|T_SLASH) Term }
    Primary ::= T_ICONST | T_SCONST | T_ID | T_LPAREN Expr T_RPAREN
 
 */



