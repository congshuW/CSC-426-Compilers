//Congshu Wang revised on Oct 7, 2014

#include "stdafx.h"

#include "scanner.h"

tokenClass::tokenClass()
{
	lexeme = "";
	tokentype = -1;
	subtype = -1;
}

scannerClass::scannerClass()
{
	theLex = ""; // the string to store lexeme from reading scanner
	state = 0;
	exprDebugStatus = false; //set expression debugging status to fault by default
}

tokenClass scannerClass::getToken()
{
	tokenClass token;
	state = 0;
	do
	{
		inputChar = theFile.getNextChar();//get an input char	
		theLex += inputChar; //append input char to theLex
		if (theLex.length() == MAXCELLSPERSTRING)
		{/*to prevent the string overflow when comment length larger than max string length*/
			theLex = "";
		}
		state = getNextState(state, inputChar);
		if (state == 100)
		{
			theFile.pushBack();
			theLex.erase(theLex.length() - 1, 1);
			if (theLex.length() > 12)
			{
				cout << "Lexical Error: Identifier length out of max limit 12. " << endl;
				theFile.printCurrentLine();
				cin.get();
				exit(1);
			}
			/*check identifier with special meaning strings*/
			else if (_stricmp(theLex.c_str(), "program") == 0)
			{
				token.setToken("program", PROGRAM_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "function") == 0)
			{
				token.setToken("function", FUNCTION_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "begin") == 0)
			{
				token.setToken("begin", BEGIN_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "end") == 0)
			{
				token.setToken("end", END_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "if") == 0)
			{
				token.setToken("if", IF_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "then") == 0)
			{
				token.setToken("then", THEN_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "else") == 0)
			{
				token.setToken("else", ELSE_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "while") == 0)
			{
				token.setToken("while", WHILE_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "do") == 0)
			{
				token.setToken("do", DO_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "cout") == 0)
			{
				token.setToken("cout", COUT_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "cin") == 0)
			{
				token.setToken("cin", CIN_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "endl") == 0)
			{
				token.setToken("endl", ENDL_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "int") == 0)
			{
				token.setToken("int", INT_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "boolean") == 0)
			{
				token.setToken("boolean", BOOLEAN_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "true") == 0)
			{
				token.setToken("true", TRUE_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "false") == 0)
			{
				token.setToken("false", FALSE_T, NONE);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "or") == 0)
			{
				token.setToken("or", ADDOPS_T, OR_ST);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "and") == 0)
			{
				token.setToken("and", MULOPS_T, AND_ST);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "div") == 0)
			{
				token.setToken("div", MULOPS_T, DIVISION_ST);
				state = 0;
				theLex = "";
			}
			else if (_stricmp(theLex.c_str(), "mod") == 0)
			{
				token.setToken("mod", MULOPS_T, MODULO_ST);
				state = 0;
				theLex = "";
			}
			else
			{
				token.setToken(theLex, IDEN_T, NONE);
				state = 0;
				theLex = "";
			}
			return token;
		}
		else if (state == 101)
		{
			//end of file
			token.setToken("", EOF_T, NONE);
			return token;
		}
		else if (state == 102)
		{
			if (isspace(inputChar) || inputChar == '\n')
			{
				//if the input char is ws or cr, just erase and back to state 0
				theLex.erase(theLex.length() - 1, 1);
			}
			else if (isdigit(inputChar)) //if input char is digit
			{
				//if next char is still digit, pushback and let program append the next digit to the lexeme
				if (isdigit(theFile.getNextChar()))
				{
					theFile.pushBack();
				}
				else
				{
					theFile.pushBack();
					//lexical error when integer constant has more than 4 digits
					if (theLex.length() > 4)
					{
						cout << "Lexical Error: Integer constant has more than 4 digits. " << endl;
						theFile.printCurrentLine();
						cin.get();
						exit(1);
					}
					else
					{
						token.setToken(theLex, INTCONST_T, NONE);
						state = 0;
						theLex = "";
						return token;
					}
				}
			}
			else if (!isalpha(inputChar) || inputChar != EOF || inputChar != '>' || inputChar != '<' || inputChar != '='
				|| inputChar != '+' || inputChar != '-' || inputChar != '*' || inputChar != '{' || inputChar != '(' || inputChar != ')'
				|| inputChar != 39 || inputChar != ';' || inputChar != ',' || inputChar != '.' || inputChar != '&' || inputChar != '/' || !isdigit(inputChar))
			{
				//can't take symbols other than those listed above
				cout << "Lexical Error: Can't start a token with " << theLex << ". " << endl;
				theFile.printCurrentLine();
				cin.get();
				exit(1);
			}
			state = 0;
		}
		else if (state == 103)
		{
			//>>
			token.setToken(theLex, GREATGREAT_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 104)
		{
			//><
			token.setToken(theLex, SWAP_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 105)
		{
			//>=
			token.setToken(theLex, RELOP_T, GREATEQ_ST);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 106)
		{
			//>
			theFile.pushBack();
			theLex.erase(theLex.length() - 1, 1);
			token.setToken(theLex, RELOP_T, GREATTHAN_ST);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 107)
		{
			//<
			theFile.pushBack();
			theLex.erase(theLex.length() - 1, 1);
			token.setToken(theLex, RELOP_T, LESSTHAN_ST);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 108)
		{
			//<<
			token.setToken(theLex, LESSLESS_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 109)
		{
			//<=
			token.setToken(theLex, RELOP_T, LESSEQ_ST);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 110)
		{
			//<>
			token.setToken(theLex, RELOP_T, NOTEQ_ST);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 111)
		{
			//==
			token.setToken(theLex, RELOP_T, EQUAL_ST);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 112)
		{
			//=
			theFile.pushBack();
			theLex.erase(theLex.length() - 1, 1);
			token.setToken(theLex, ASSIGN_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 113)
		{
			//+
			token.setToken(theLex, ADDOPS_T, ADDITION_ST);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 114)
		{
			//-
			token.setToken(theLex, ADDOPS_T, SUBSTRACTION_ST);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 115)
		{
			//*
			token.setToken(theLex, MULOPS_T, MULTIPLICATION_ST);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 116)
		{
			//error, EOF after reading the opening { of a comment but before reading the closed }
			cout << "Lexical Error: EOF after open { but before closed }" << endl;
			theFile.printCurrentLine();
			cin.get();
			exit(1);
		}
		else if (state == 117)
		{
			//{} comments
			theLex = "";
			state = 0;
		}
		else if (state == 118)
		{
			//(
			token.setToken(theLex, OPENPARENT_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 119)
		{
			//)
			token.setToken(theLex, CLOSEPARENT_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 120)
		{
			//string constant
			if (theLex.length() > 52) //50 chars and 2 single quotes
			{
				cout << "Lexical Error: String constant length out of max limit 50 characters. " << endl;
				theFile.printCurrentLine();
				cin.get();
				exit(1);
			}
			else
			{
				theLex.erase(0, 1);  //erase first single quote
				theLex.erase(theLex.length() - 1, 1);  //erase last single quote
				token.setToken(theLex, STRINGCONST_T, NONE);
				state = 0;
				theLex = "";
				return token;
			}
		}
		else if (state == 121)
		{
			//string constant with embedded carriage return in the middle
			cout << "Lexical Error: String constant contains an embedded carriage return. " << endl;
			theFile.printCurrentLine();
			cin.get();
			exit(1);
		}
		else if (state == 122)
		{
			//string constant has EOF in the middle
			cout << "Lexical Error: String constant contains an EOF. " << endl;
			theFile.printCurrentLine();
			cin.get();
			exit(1);
		}
		else if (state == 123)
		{
			//;
			token.setToken(theLex, SEMICOLN_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 124)
		{
			//,
			token.setToken(theLex, COMMA_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 125)
		{
			//.
			token.setToken(theLex, PERIOD_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 126)
		{
			//&
			token.setToken(theLex, AMPERSAND_T, NONE);
			state = 0;
			theLex = "";
			return token;
		}
		else if (state == 127)
		{
			//compiler directive
			if (theLex[2] == 'p') //it is {$p+} or {$p-}
			{
				if (theLex[3] == '+')
				{
					theFile.setPrintStatus(true);
				}
				else if (theLex[3] == '-')
				{
					theFile.setPrintStatus(false);
				}
			}
			else if (theLex[2] == 'e') //it is {$e+} or {$e-}
			{
				if (theLex[3] == '+')
				{
					exprDebugStatus = true;
				}
				else if (theLex[3] == '-')
				{
					exprDebugStatus = false;
				}
			}
			else  //warning if it is not lower case p or lower case e
			{
				cout << "Lexical Warning: unknown compiler directive. " << endl;
				theFile.printCurrentLine();
			}
			state = 0;
			theLex = "";
		}
		else if (state == 128)
		{
			//the appearance of / is not followed by another / immediately
			cout << "Lexical Error: The appearance of / is not followed by another / immediately. " << endl;
			theFile.printCurrentLine();
			cin.get();
			exit(1);
		}
		else if (state == 13)
		{
			theLex = "";
			state = 0;
		}
	} while (1);
}

int scannerClass::getNextState(int state, int ch)
{
	static int table[13][24] = {
		/*row index 0*/	2, 3, 4, 101, 1, 102, 113, 114, 115, 118, 119, 123, 124, 125, 126, 102, 102, 102, 5, 102, 102, 9, 10, 102,
		/*row index 1*/	100, 100, 100, 100, 1, 1, 100, 100, 100, 100, 100, 100, 100, 100, 100, 1, 100, 100, 100, 100, 100, 100, 100, 100,
		/*row index 2*/	103, 104, 105, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106,
		/*row index 3*/	110, 108, 109, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
		/*row index 4*/	112, 112, 111, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112,
		/*row index 5*/ 12, 12, 12, 116, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 117, 6, 12, 12, 12,
		/*row index 6*/ 12, 12, 12, 116, 7, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 117, 12, 12, 12, 12,
		/*row index 7*/ 12, 12, 12, 116, 12, 12, 8, 8, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 117, 12, 12, 12, 12,
		/*row index 8*/ 12, 12, 12, 116, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 127, 12, 12, 12, 12,
		/*row index 9*/ 9, 9, 9, 122, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 121, 9, 9, 9, 9, 120, 9, 9,
		/*row index 10*/ 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 11, 128,
		/*row index 11*/ 11, 11, 11, 13, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 13, 11, 11, 11, 11, 11, 11, 11,
		/*row index 12*/ 12, 12, 12, 116, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 117, 12, 12, 12, 12
	};
	if (ch == '>')
	{
		col = 0;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '<')
	{
		col = 1;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '=')
	{
		col = 2;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == EOF)
	{
		col = 3;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (isalpha(ch))
	{
		col = 4;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (isdigit(ch))
	{
		col = 5;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '+')
	{
		col = 6;
		if (state == 0)
		{
			row = 0;
		}
		//others
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '-')
	{
		col = 7;
		if (state == 0)
		{
			row = 0;
		}
		//others
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '*')
	{
		col = 8;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '(')
	{
		col = 9;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == ')')
	{
		col = 10;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == ';')
	{
		col = 11;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == ',')
	{
		col = 12;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '.')
	{
		col = 13;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '&')
	{
		col = 14;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '_')
	{
		col = 15;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '\n')
	{
		//if input is carriage return
		col = 16;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (isspace(ch))
	{
		//if input is white space
		col = 17;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '{')
	{
		col = 18;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '}')
	{
		col = 19;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '$')
	{
		col = 20;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == 39) //single quote
	{
		col = 21;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else if (ch == '/')
	{
		col = 22;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	else
	{
		col = 23;
		if (state == 0)
		{
			row = 0;
		}
		else if (state == 1)
		{
			row = 1;
		}
		else if (state == 2)
		{
			row = 2;
		}
		else if (state == 3)
		{
			row = 3;
		}
		else if (state == 4)
		{
			row = 4;
		}
		else if (state == 5)
		{
			row = 5;
		}
		else if (state == 6)
		{
			row = 6;
		}
		else if (state == 7)
		{
			row = 7;
		}
		else if (state == 8)
		{
			row = 8;
		}
		else if (state == 9)
		{
			row = 9;
		}
		else if (state == 10)
		{
			row = 10;
		}
		else if (state == 11)
		{
			row = 11;
		}
		else if (state == 12)
		{
			row = 12;
		}
	}
	return table[row][col];
}

void tokenClass::display()
{
	string printOutString = lexeme;

	/*token type*/
	if (tokentype == ASSIGN_T)
		printOutString += " ASSIGN_T";
	else if (tokentype == RELOP_T)
		printOutString += " RELOP_T";
	else if (tokentype == SWAP_T)
		printOutString += " SWAP_T";
	else if (tokentype == GREATGREAT_T)
		printOutString += " GREATGREAT";
	else if (tokentype == LESSLESS_T)
		printOutString += " LESSLESS";
	else if (tokentype == EOF_T)
		printOutString += " EOF_T";
	else if (tokentype == ADDOPS_T)
		printOutString += " ADDOPS_T";
	else if (tokentype == MULOPS_T)
		printOutString += " MULOPS_T";
	else if (tokentype == OPENPARENT_T)
		printOutString += " OPENPARENT_T";
	else if (tokentype == CLOSEPARENT_T)
		printOutString += " CLOSEPARENT_T";
	else if (tokentype == SEMICOLN_T)
		printOutString += " SEMICOLN_T";
	else if (tokentype == COMMA_T)
		printOutString += " COMMA_T";
	else if (tokentype == PERIOD_T)
		printOutString += " PERIOD_T";
	else if (tokentype == AMPERSAND_T)
		printOutString += " AMPERSAND_T";
	else if (tokentype == PROGRAM_T)
		printOutString += " PROGRAM_T";
	else if (tokentype == FUNCTION_T)
		printOutString += " FUNCTION_T";
	else if (tokentype == BEGIN_T)
		printOutString += " BEGIN_T";
	else if (tokentype == END_T)
		printOutString += " END_T";
	else if (tokentype == IF_T)
		printOutString += " IF_T";
	else if (tokentype == THEN_T)
		printOutString += " THEN_T";
	else if (tokentype == ELSE_T)
		printOutString += " ELSE_T";
	else if (tokentype == WHILE_T)
		printOutString += " WHILE_T";
	else if (tokentype == DO_T)
		printOutString += " DO_T";
	else if (tokentype == COUT_T)
		printOutString += " COUT_T";
	else if (tokentype == CIN_T)
		printOutString += " CIN_T";
	else if (tokentype == ENDL_T)
		printOutString += " ENDL_T";
	else if (tokentype == INT_T)
		printOutString += " INT_T";
	else if (tokentype == BOOLEAN_T)
		printOutString += " BOOLEAN_T";
	else if (tokentype == TRUE_T)
		printOutString += " TRUE_T";
	else if (tokentype == FALSE_T)
		printOutString += " FALSE_T";
	else if (tokentype == IDEN_T)
		printOutString += " IDEN_T";
	else if (tokentype == STRINGCONST_T)
		printOutString += " STRINGCONST_T";
	else if (tokentype == INTCONST_T)
		printOutString += " INTCONST_T";
	else if (tokentype == E_T)
		printOutString += " E_T";

	/*token subtype*/
	if (subtype == NONE)
		printOutString += " NONE";
	else if (subtype == NOTEQ_ST)
		printOutString += " NOTEQ_ST";
	else if (subtype == EQUAL_ST)
		printOutString += " EQUAL_ST";
	else if (subtype == LESSTHAN_ST)
		printOutString += " LESSTHAN_ST";
	else if (subtype == LESSEQ_ST)
		printOutString += " LESSEQ_ST";
	else if (subtype == GREATTHAN_ST)
		printOutString += " GREATTHAN_ST";
	else if (subtype == GREATEQ_ST)
		printOutString += " GREATEQ_ST";
	else if (subtype == ADDITION_ST)
		printOutString += " ADDITION_ST";
	else if (subtype == SUBSTRACTION_ST)
		printOutString += " SUBSTRACTION_ST";
	else if (subtype == OR_ST)
		printOutString += " OR_ST";
	else if (subtype == AND_ST)
		printOutString += " AND_ST";
	else if (subtype == MULTIPLICATION_ST)
		printOutString += " MULTIPLICATION_ST";
	else if (subtype == DIVISION_ST)
		printOutString += " DIVISION_ST";
	else if (subtype == MODULO_ST)
		printOutString += " MODULO_ST";

	cout << printOutString << endl;
}

void tokenClass::setToken(string lex, int tt, int st)
{
	lexeme = lex;
	tokentype = tt;
	subtype = st;
}

int tokenClass::getTokenType()
{
	return tokentype;
}

int tokenClass::getTokenSubType()
{
	return subtype;
}

int scannerClass::numOfLineCounted()
{
	//return the number of line read by scanner
	return theFile.numLinesProcessed();
}

void scannerClass::closeSourceProgram()
{
	//This method closes the file when the driver program is about to terminate.
	//Call the closeSourceProgram() from fileManagerClass
	theFile.closeSourceProgram();
}

bool scannerClass::returnExprDebugStatus()
{
	//return current expression debugging status
	return exprDebugStatus;
}

void scannerClass::printCurrentLine()
{
	//call filemanager class printCurrentLine function
	//this method is used in parser class
	theFile.printCurrentLine();
}