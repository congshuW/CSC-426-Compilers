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
	void parseProgram();
	int numLinesProcessed();// a method  that returns the number of lines processed by calling the similar method in scanner class
	void errorexit(string msg1, string msg2); // an error helper method that prints out customized error message

	void parse_stmt();	// a method that parses statements
	void parse_flIf(); //a method that parses <follow-if> 
	void parse_flId(); // a method that parses <follow-id>
	void parse_flCout(); // a method that parses <follow-cout>
	void parse_flCin(); // a method that parses <follow-cin>
	void parse_flBegin(); // a method that parses <follow-begin>
	void parse_stmtail(); // a method that parses <statement-tail>
	void parse_flexpr(); // a method that parses <follow-expression>

	void parse_block(); // a method that parses functions
	void parse_type(); // a method that parses <type>
	void parse_varDecs(); // a method that parses <var-decs>
	void parse_identList(); // a method that parses <ident-list>
	void parse_identail(); // a method that parsers <ident-tail> in <var-decs>
	void parse_funDecs(); // a method that parses <function-decs>
	void parse_funcDecTail(); // a method that parses <function-decs-tail>
	void parse_paramList(); // a method that parses <param-list>
	void parse_typeTail(); // a method that parses <type-tail>
	void parse_identail2(); // a method that parses <ident-tail> in <func-decs>
	void parse_progbody(); // a method that parses <prog-body>

	void parse_program(); // a method that parses program

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