//Congshu Wang created on Nov 27, 2014
#ifndef _parser
#define _parser

#include "scanner.h"
#include "pstack.h"
#include "general.h"
#include "table.h"
#include <fstream>
#include <sstream>

using namespace std;

class parserClass
{
public:
	parserClass();
	void parse_expr();//the main method that parses the expression
	int checkPrecedence(int tmt,int token); //a method returns precedence value by checking inputs' types in its precedence table
	bool isValidRHS(int tempArrayType[], int tempArraySubType[], string tempArrayLexeme[], SymbolNode* tempArraySymbol[], int counter);//a method returns boolean value to check if inputs are valid YASL expressions
	bool noTerminalsHaveBeenPopped(int tempArrayType[], int counter);// a helper method to check whether there is a terminal token stores in the input array
	void parseProgram();
	int numLinesProcessed();// a method  that returns the number of lines processed by calling the similar method in scanner class
	void errorexit(string msg1, string msg2); // an error helper method that prints out customized error message
	void dupErrorExit(string msg1); //an error helper method that prints out error messages when duplcate identifiers are found
	void undeclaredErrorExit(string msg1); //an error helper method that prints out error messages when duplcate identifiers are found
	void notSameTypeErrorExit(string msg1, string msg2); // an error helper method that prints out error messages when variable's type mismatch
	void wrongTypeErrorExit(string msg1, string msg2); // an error helper method that prints out error messages when variable has wrong type

	void parse_stmt();	// a method that parses statements
	void parse_flIf(); //a method that parses <follow-if> 
	void parse_flId(); // a method that parses <follow-id>
	void parse_flCout(); // a method that parses <follow-cout>
	void parse_flCin(); // a method that parses <follow-cin>
	void parse_flBegin(); // a method that parses <follow-begin>
	void parse_stmtail(); // a method that parses <statement-tail>
	void parse_flexpr(SymbolNode *param); // a method that parses <follow-expression>

	/*--------------------------*/
	void parse_block(int paramCounter); // a method that parses functions
	/*--------------------------*/
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

	string getNextTempVar(); // a method to generate temporary variable names
	void printInstruction(string instruction, SymbolNode *firstParamPointer, string firstParam, SymbolNode *secondParamPointer, string secondParam); // a helper method for printing an assembly language instruction
	void printInstruction(string instruction, SymbolNode *firstParamPointer, string firstParam);
	void printBranch(string branchName); // a method to print brance name in pali
	string printInstructionHelper(SymbolNode *paramPointer, string param); 
	SymbolNode* addTempVar(int symbolType);// a method to add a temperoary variable on symbol table
	string intToPali(int n); // a mehtod to turn an integer into string format of part of a pali command
	string strToPali(string s);// a mehtod to turn a string into string format of part of a pali command
	SymbolNode* paliArithmetic(string operation, SymbolNode *node1, SymbolNode *node2);
	void checkVariable(int type);
	void checkVariable(SymbolNode *node);
	/*Other YASL Operations For Modulo, And, Or, ==, <, <=, >, >=, <>*/
	SymbolNode* paliMod(SymbolNode *node1, SymbolNode *node2);
	SymbolNode* paliAnd(SymbolNode *node1, SymbolNode *node2);
	SymbolNode* paliOr(SymbolNode *node1, SymbolNode *node2);
	SymbolNode* paliBranch(string operation, SymbolNode *node1, SymbolNode *node2);

private:
	scannerClass theScanner;
	tokenClass startToken;
	tokenClass token;
	tokenClass tokenPushOnTheStack;
	int topTerm;
	int row;
	int col;
	int symbol_type;
	//int numOfFuncParams;
	//int funcParamsCount;
	fstream outfile;
	int globalVarCounter = 0;
	SymbolNode *idLookUpResult;
	SymbolNode *symbolPushOnStack;
	int returnedSymbolType;
	/*-----------------------*/
	string requiredType;
	string returnedType;
	/*-----------------------*/

	string elseLable;
	string afterIfLabel;
};
#endif