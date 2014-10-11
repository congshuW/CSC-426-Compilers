//Congshu Wang Revised Sep 11, 2014

//Added some test codes to let setPrintStatus be true.  Added a line of code to cout how many lines have compiled.

//This driver program should be used to test the first part of the YASL
//compiler.

//This version tested in Visual Studio .NET 2014

#include "stdafx.h"  // A visual studio requirement

#include "filemngr.h"
#include <iostream>
#include "scanner.h"

int main(int argc, char* argv[])
{
	scannerClass scanner;
	tokenClass theToken;
	do
	{
		theToken = scanner.getToken();
		theToken.display();
	} while (theToken.getTokenType() != EOF_T);
	scanner.closeSourceProgram();
	cout << "YASLC-CW has just compiled " << scanner.numOfLineCounted() << " lines of code." << endl;
	cin.get();  // wait user to response
	return (0);
}
