//Congshu Wang created on Sep 27, 2014
#ifndef _parser
#define _parser

#include "scanner.h"
#include "pstack.h"
#include "general.h"

using namespace std;

class parserClass
{
public:
	void parse_expr();//the main method that parses the expression
	int checkPrecedence(int tmt,int token); //a method returns precedence value by checking inputs' types in its precedence table
	bool isValidRHS(int tempArrayType[], int tempArraySubType[], int counter);//a method returns boolean value to check if inputs are valid YASL expressions
	bool noTerminalsHaveBeenPopped(int tempArrayType[], int counter);// a helper method to check whether there is a terminal token stores in the input array

private:
	scannerClass theScanner;
	tokenClass startToken;
	tokenClass token;
	tokenClass tokenPushOnTheStack;
	int topTerm;
	int row;
	int col;
};
#endif