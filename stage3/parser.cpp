//Congshu Wang created on Sep 27, 2014

#include "stdafx.h"
#include "parser.h"

void parserClass::parse_expr()
{
	pStackClass theStack;
	startToken.setToken(";", SEMICOLN_T, NONE);
	theStack.push(startToken);
	token = theScanner.getToken();
	while (1)
	{
		if ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == SEMICOLN_T))
		{
			return;
		}
		else
		{
			topTerm = theStack.returnTopMostTerminal();
			if (checkPrecedence(topTerm, token.getTokenType()) == LT || checkPrecedence(topTerm, token.getTokenType()) == EQ)
			{
				theStack.push(token);
				token = theScanner.getToken();
				//theStack.helper();
			}
			else if (checkPrecedence(topTerm, token.getTokenType()) == GT)
			{
				int counter = 0;
				int tempArrayType[3];
				int tempArraySubType[3];
				do
				{
					tempArrayType[counter] = theStack.pop();//call pop to return the last popped token type and get its subtype
					tempArraySubType[counter] = theStack.returnLastPoppedTokenSubType();//put the assigned subtype to subtype array
					++counter;
				} while (noTerminalsHaveBeenPopped(tempArrayType, counter) || theStack.isTerminalOnTop() == false || checkPrecedence(theStack.returnTopMostTerminal(), theStack.returnLastTerminalPopped()) != LT);
				if (isValidRHS(tempArrayType, tempArraySubType, counter))
				{
					theStack.pushEOnStack();
				}
				else
				{
					cout << "Syntax Error: Error In Expression" << endl;
					theScanner.printCurrentLine();
					cin.get();
					exit(1);
				}
			}
			else
			{
				cout << "Syntax Error: Invalid YASL Expression." << endl;
				theScanner.printCurrentLine();
				cin.get();
				exit(1);
			}
		}
	}
}

bool parserClass::isValidRHS(int tempArrayType[], int tempArraySubType[], int counter)
{
	if (counter == 1)
	{
		switch (tempArrayType[0])
		{
		case IDEN_T:
			if (theScanner.returnExprDebugStatus() == true)
				cout << "E -> id" << endl;
			return true;
			break;
		case INTCONST_T:
			if (theScanner.returnExprDebugStatus() == true)
				cout << "E -> integer-constant" << endl;
			return true;
			break;
		case TRUE_T:
			if (theScanner.returnExprDebugStatus() == true)
				cout << "E -> true" << endl;
			return true;
			break;
		case FALSE_T:
			if (theScanner.returnExprDebugStatus() == true)
				cout << "E -> false" << endl;
			return true;
			break;
		default:
			return false;
			break;
		}
	}
	else if (counter == 3)
	{
		if(tempArrayType[0] == E_T) //if the expression stored in tempArrayType starts with E_T
		{
			if ((tempArraySubType[1] == ADDITION_ST) && tempArrayType[2] == E_T )
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E + E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == MULTIPLICATION_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E * E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == SUBSTRACTION_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E - E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == DIVISION_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E div E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == MODULO_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E mod E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == OR_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E or E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == AND_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E and E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == EQUAL_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E == E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == LESSTHAN_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E < E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == LESSEQ_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E <= E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == GREATTHAN_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E > E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == GREATEQ_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E >= E" << endl;
				return true;
			}
			else if ((tempArraySubType[1] == NOTEQ_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E <> E" << endl;
				return true;
			}
			else
				return false;
		}
		else if (tempArrayType[0] == CLOSEPARENT_T)
		{
			if ((tempArrayType[1] == E_T) && tempArrayType[2] == OPENPARENT_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> (E)" << endl;
				return true;
			}
			else
				return false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

int parserClass::checkPrecedence(int tmt, int token)
{
	static int table[11][11] = {
		/*row RELOP*/	GT, LT, LT, LT, GT, LT, LT, LT, LT, GT, ER,
		/*row ADDOP*/	GT, GT, LT, LT, GT, LT, LT, LT, LT, GT, ER,
		/*row MULOP*/	GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, ER,
		/*row (*/		LT, LT, LT, LT, EQ, LT, LT, LT, LT, ER, ER,
		/*row )*/		GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER,
		/*row id*/		GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER,
		/*row ic*/		GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER,
		/*row TRUE*/	GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER,
		/*row FALSE*/	GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, ER,
		/*row ;*/		LT, LT, LT, LT, ER, LT, LT, LT, LT, ER, ER,
		/*row Other*/	ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER
	};
	if (tmt == RELOP_T)
	{
		row = RELOP_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else if (tmt == ADDOPS_T)
	{
		row = ADDOPS_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else if (tmt == MULOPS_T)
	{
		row = MULOPS_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else if (tmt == OPENPARENT_T)
	{
		row = OPENPARENT_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else if (tmt == CLOSEPARENT_T)
	{
		row = CLOSEPARENT_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else if (tmt == IDEN_T)
	{
		row = IDEN_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else if (tmt == INTCONST_T)
	{
		row = INTCONST_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else if (tmt == TRUE_T)
	{
		row = TRUE_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else if (tmt == FALSE_T)
	{
		row = FALSE_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else if (tmt == SEMICOLN_T)
	{
		row = SEMICOLN_T;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	else
	{
		row = OTHER;
		switch (token)
		{
		case RELOP_T:
			col = RELOP_T;
			break;
		case ADDOPS_T:
			col = ADDOPS_T;
			break;
		case MULOPS_T:
			col = MULOPS_T;
			break;
		case OPENPARENT_T:
			col = OPENPARENT_T;
			break;
		case CLOSEPARENT_T:
			col = CLOSEPARENT_T;
			break;
		case IDEN_T:
			col = IDEN_T;
			break;
		case INTCONST_T:
			col = INTCONST_T;
			break;
		case TRUE_T:
			col = TRUE_T;
			break;
		case FALSE_T:
			col = FALSE_T;
			break;
		case SEMICOLN_T:
			col = SEMICOLN_T;
			break;
		default:
			col = OTHER;
			break;
		}
	}
	return table[row][col];
}

bool parserClass::noTerminalsHaveBeenPopped(int tat[], int c)
{
	for (int i = 0; i < c; i++)
	{
		if (tat[i] != E_T)
			return false;
	}
	return true;
}