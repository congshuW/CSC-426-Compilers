//Congshu Wang revised on Sep 27, 2014

#ifndef _scanner
#define _scanner

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>    //to allow exit
#include "general.h"
#include "filemngr.h"
#include <string> //to allow string methods
#include <ctype.h> // to check characters in different categories

using namespace std;

class tokenClass
{
public:
	tokenClass();
	void display();
	int getTokenType();
	int getTokenSubType();
	void setToken(string lex, int tt, int st);  //tt stands for token type, st stands subtype

private:
	string lexeme;  //the lexeme value
	int tokentype;	//the announced token type
	int subtype;	//the announced
};

class scannerClass
{
	public:
		scannerClass();
		void closeSourceProgram();
		tokenClass getToken();
		int getNextState(int, int);
		int numOfLineCounted();
		bool returnExprDebugStatus();//new added setExprDebugStatus method in scannerClass
		void printCurrentLine();//new added method to be used in parser class

	private:
		fileManagerClass theFile;
		string theLex;  //the string to store lexeme from scanner
		int inputChar; // the input char that be read
		int state;
		int col;
		int row;
		bool exprDebugStatus = false;//new added boolean variable with default false in scannerClass's constructor
};
#endif