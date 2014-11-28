//Congshu Wang created on Sep 27, 2014

#include "stdafx.h"
#include "pstack.h"

#include "scanner.h"

pStackClass::pStackClass()
{
	top = NULL;
}

void pStackClass::push(tokenClass theToken, SymbolNode *theSymbol)
{
	//push a token onto the stack and store the token's type & subtype in each cell
	StackCell *tempCell = new StackCell;
	tempCell->next = top;
	top = tempCell;
	tempCell->type = theToken.getTokenType();
	tempCell->subtype = theToken.getTokenSubType();
	tempCell->lexeme = theToken.getLexeme();
	tempCell->symbol = theSymbol;
}

int pStackClass::pop()
{
	//pop the top cell of the stack and return the popped token's type
	if (top == NULL)
		return NONE;
	if (top->type != E_T)//check the type of the top token is going to be popped is terminal
		lastTerminalPopped = top->type;
	lastPoppedTokenType = top->type;
	lastPoppedTokenSubType = top->subtype;
	lastPoppedTokenLexeme = top->lexeme;
	lastPoppedTokenSymbol = top->symbol;
	StackCell *temp = top;
	top = top->next;
	delete temp;
	return lastPoppedTokenType;
}

bool pStackClass::isTerminalOnTop()
{
	//check top token on the stack is terminal or not
	if (top == NULL)
		return false;
	return top->type != E_T;
}

int pStackClass::returnTopMostTerminal()
{
	//return the type of top most terminal token on the stack
	StackCell *temp = top;
	while (temp != NULL)
	{
		if (temp->type != E_T)
			return temp->type;
		temp = temp->next;
	}
	return NONE;
}

int pStackClass::returnLastTerminalPopped()
{
	return lastTerminalPopped;
}

void pStackClass::pushEOnStack(SymbolNode *theSymbol)
{
	//a special method that push E_T's type and subtype onto the stack
	StackCell *tempCell = new StackCell;
	tempCell->next = top;
	top = tempCell;
	tempCell->type = E_T;
	tempCell->subtype = NONE;
	tempCell->symbol = theSymbol;
}

int pStackClass::returnLastPoppedTokenSubType()
{
	return lastPoppedTokenSubType;
}

string pStackClass::returnLastPoppedTokenLexeme()
{
	return lastPoppedTokenLexeme;
}

SymbolNode* pStackClass::returnLastPoppedTokenSymbol()
{
	return lastPoppedTokenSymbol;
}

void pStackClass::helper()
{
	//a helper method that walks through each cell of the stack and prints out the type & subtype stored in each cell
	StackCell *temp = new StackCell;
	temp = top;
	while (temp != NULL)
	{
		cout << temp->type << endl;
		cout << temp->subtype << endl;
		temp = temp->next;
	}
}