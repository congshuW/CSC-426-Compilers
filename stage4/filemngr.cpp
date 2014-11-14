//Congshu Wang. Revised on Sep 3, 2014

//Added pushBack(), printCurrentLine(), setPrintStatus(), numLinesProcessed methods for stage 1

//This file contains implementations of the 
//fileManagerClass

//The class header is found in filemngr.h

#include "stdafx.h"  // Required for visual studio to work correctly

#include "filemngr.h"


fileManagerClass::fileManagerClass()
//Precondition: None.
//Postcondition: The constructor has opened the file which is to be
//               processed and has inialized the currentLineNumber to 0
{
	autoPrintStatus = false; //default false for boolean private variable autoPrintStatus

	char filename[MAXCELLSPERSTRING];

	cout << "Enter file name to compile: ";
	cin.getline(filename, MAXCELLSPERSTRING);
	fileToCompile.open(filename);

	if (fileToCompile.fail())
	{
		cout << "Error, the source file: " << filename << " was not opened." << endl;
		cout << "Press return to end program." << endl;
		cin.get();
		exit(1);
	}
	currentLineNumber = 0;
	lineCounter = 0; // count how many lines have been read into the buffer so far
}

int fileManagerClass::getNextChar()
//Precondition:  The source file associated with the owning object has
//               been prepared for reading.
//Postcondition: The next character from the input file has been read and
//               returned by the function.  If another chacter could not
//               be read because END OF FILE was reached then EOF is
//               returned.
{
	if ((currentLineNumber == 0) || (charToReadNext == strlen(currentLine)))
	{
		if (fileToCompile.peek() == EOF)
		{
			return (EOF);
		}
		lineCounter++; //when it is not EOF but end of a line or when 1st line starts
		fileToCompile.getline(currentLine, MAXCELLSPERSTRING);
		strcat(currentLine, "\n");
		if (autoPrintStatus == true)
		//check if the autoPrintStatus is ture to print the current line before moving to next line
		{
			printCurrentLine();
		}
		currentLineNumber++;
		charToReadNext = 0;
	}
	return(currentLine[charToReadNext++]);
}


void fileManagerClass::closeSourceProgram()
//Precondition:  The file belonging to the object owning this routine has
//               been opened.
//Postcondition: The file belonging to the object owning this routine has
//               been closed.
{
	fileToCompile.close();
}

void fileManagerClass::pushBack()
//if charToReadNext not 0, decrement by 1, else stay 0
{
	if (charToReadNext != 0)
	{
		--charToReadNext;
	}
	else
		charToReadNext = 0;
}

void fileManagerClass::printCurrentLine()
//print line number followed by a hyphen and a space followed by the cotent in the current array of char
{
	cout << currentLineNumber + 1 << "- " << currentLine << endl;
}

void fileManagerClass::setPrintStatus(bool newStatus)
//change autoPrintStatus to match bool variable newStatus
{
	autoPrintStatus = newStatus;
}

int fileManagerClass::numLinesProcessed()
//return the cumulative number of lines that have been read into the buffer so far
{
	return lineCounter;
}