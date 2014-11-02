//Congshu Wang created on Oct 7, 2014

#ifndef _pstack
#define _pstack

#include "scanner.h"

class StackCell
{
public:
	int type;
	int subtype;
	StackCell *next;
};

class pStackClass
{
public:
	pStackClass();
	void push(tokenClass theToken);
	int pop();//remove and return top thing on the stack
	bool isTerminalOnTop(); //check if there is a terminal token on top of the stack
	int returnTopMostTerminal(); //return the type of the top most terminal token on the stack
	int returnLastTerminalPopped();//return the type of last popped terminal token
	void pushEOnStack();//special method to push E_T on the stack
	int returnLastPoppedTokenSubType();// return the subtype of last popped token
	void helper(); //walk through stack and print out things
	
private:
	StackCell *top;
	int lastTerminalPopped;
	int lastPoppedTokenType;
	int lastPoppedTokenSubType;
};

#endif