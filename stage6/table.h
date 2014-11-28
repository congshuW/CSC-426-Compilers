//Created by Congshu Wang on Nov 3, 2014.
//This is the header file of symbol table.

#ifndef _table
#define _table

#include <string>
#include "general.h"
#include <iostream>

using namespace std;

class SymbolNode
{
public:
	SymbolNode(string lexeme, int kind, int type);
	~SymbolNode(); // destructor to free memory
	string symbolLexeme;
	int symbolKind;
	int symbolType;
	int offset;//the offset of the identifier from the start of the current scope
	int nestingLevel;
	int numOfParams;
	SymbolNode *next;
	SymbolNode *parameterTop;
	string print();
	string printParam();
	bool addParam(SymbolNode* param); // add parameter when symbol name is function name
	bool isTempSymbol();

};

class tableLevel
{
public:
	tableLevel(string name, int nestingLevel);
	~tableLevel(); // destructor to free memory
	string name; // the name of the scoping level (for example, main.foo.fee)
	int nestingLevel; 
	/*The nesting level of the table(level zero for program body, 
	level one for everything defined within the program body,
	level two for those things defined inside of level one scopes, etc.)*/
	int nextOffset;
	/*The next offset to be used when a new entry is added to this level of the stack.  
	This offset should start at 0 when the stack level is created.  
	The offset should be incremented by 1 each time a variable identifier is added to this level of the table.*/
	SymbolNode *top;
	/*A pointer to a self-organizing list of nodes.  Each node contains information about a symbol.*/
	tableLevel *next;
	SymbolNode* lookup(string lexeme);
	bool addSymbol(SymbolNode* node);
	string print();

	int deleteTempSymbol();
};


class tableClass
{
public:
	tableClass(); //constructor that initializes the table to be empty
	~tableClass();// destructor to free memory
	void tableAddLevel(string scope);//add a new level to the table
	void tableDelLevel(); //delete the top-most level of the table
	bool tableAddEntry(SymbolNode *node); //add an entry to the table
	SymbolNode* tableLookUp(string lexeme);
	/*Given a lexeme as a parameter, determine if it appears in the table.  It should return NULL if the specified lexeme is not in the table, otherwise it should return a pointer to the node of the list that contains information about this lexeme.*/
	tableLevel *top;
	string print();
	bool addFuncParam(SymbolNode *param);

private:
	int nextNestingLevel;
};

#endif