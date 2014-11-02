//Congshu Wang revised on Sep 3, 2014

//This file contains new added methods pushBack(), printCurrentLine(), setPrintStatus(bool), numLinesProcessed()  
//and some var such as bool autoPrintStatus, int lineCounter that are going to be used in fileManagerClass

//This file contains a class definition for managing the
//file containing the YASL source program to be compiled.

//The definitions of these routines can be found in filemngr.cpp

#ifndef _filemngr
#define _filemngr

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>    //to allow exit
#include "general.h"

using namespace std;
using std::ifstream;

class fileManagerClass
{ public:
     fileManagerClass();
     int getNextChar();
     void closeSourceProgram();
	 void pushBack(); //new added pushBack method in fileManagerClass
	 void printCurrentLine(); //new added printCurrentLine method in fileManagerClass
	 void setPrintStatus(bool); //new added setPrintStatus method in fileManagerClass
	 int numLinesProcessed(); //new added numLinesProcessed method in fileManagerClass

  private:
     char currentLine[MAXCELLSPERSTRING];
     int charToReadNext;
     int currentLineNumber;
     ifstream fileToCompile;
	 bool autoPrintStatus;  //new added boolean variable with default false in fileManagerClass's constructor
	 int lineCounter; // new added int variable with default value 0 in fileManagerClass's constructor
}  ;

#endif
