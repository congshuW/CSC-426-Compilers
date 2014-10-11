//Congshu Wang Revised Sep 3, 2014

//Added some test codes to let setPrintStatus be true.  Added a line of code to cout how many lines have compiled.

//This driver program should be used to test the first part of the YASL
//compiler.

//This version tested in Visual Studio .NET 2014


#include "stdafx.h"  // A visual studio requirement


#include "filemngr.h"
#include <iostream>

int main(int argc, char* argv[])
{
	fileManagerClass theFile;     //Define the sourceprogram object
	char ch;
	int i = 0;// for test

	while ((ch = theFile.getNextChar()) != EOF)
	{
		cout << ch;
		i++;// for test
		if (i == 8)// for test
		{// for test
			theFile.setPrintStatus(true);// for test
		}// for test
	}
	cout << "YASLC-CW has just compiled " << theFile.numLinesProcessed() << " lines of code." << endl;
	theFile.closeSourceProgram();
	cin.get();

	return (0);
}

