//Congshu Wang Revised Oct 7, 2014

//This driver program should be used to test the first part of the YASL
//compiler.

//This version tested in Visual Studio .NET 2014

#include "stdafx.h"  // A visual studio requirement

#include "filemngr.h"
#include <iostream>
#include "scanner.h"
#include "parser.h"

int main(int argc, char* argv[])
{
	parserClass parser;

	parser.parseProgram();
	cout << "YASLC-CW has just compiled " << parser.numLinesProcessed() << " lines of code." << endl;
	cin.get();
}
