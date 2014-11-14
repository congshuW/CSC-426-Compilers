//Congshu Wang created on Sep 27, 2014

#include "stdafx.h"
#include "parser.h"

void parserClass::parse_expr()
{
	//check if the token is in the first set of an expr
	if (token.getTokenType() != IDEN_T && token.getTokenType() != INTCONST_T && token.getTokenType() != TRUE_T && token.getTokenType() != FALSE_T && token.getTokenType() != OPENPARENT_T)
	{
		cout << "Error: Expression can't be empty / Expecting an expression or a string constant or a keyword [endl] after '<<'" << endl;
		theScanner.printCurrentLine();
		cin.get();
		exit(1);
		
	}
	pStackClass theStack;
	startToken.setToken(";", SEMICOLN_T, NONE);
	theStack.push(startToken);
	//token = theScanner.getToken();
	while (1)
	{
		if (((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == SEMICOLN_T)) || ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == DO_T))
			|| ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == THEN_T)) || ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == COMMA_T))
			|| ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == LESSLESS_T)) || ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == ELSE_T))
			|| ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == CLOSEPARENT_T)) || ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == END_T)))
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
		if (tempArrayType[0] == E_T) //if the expression stored in tempArrayType starts with E_T
		{
			if ((tempArraySubType[1] == ADDITION_ST) && tempArrayType[2] == E_T)
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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
			/***********************/
		case DO_T:
			col = SEMICOLN_T;
			break;
		case THEN_T:
			col = SEMICOLN_T;
			break;
		case COMMA_T:
			col = SEMICOLN_T;
			break;
		case LESSLESS_T:
			col = SEMICOLN_T;
			break;
		case ELSE_T:
			col = SEMICOLN_T;
			break;
		case END_T:
			col = SEMICOLN_T;
			break;
			/***********************/
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

int parserClass::numLinesProcessed()
{
	return theScanner.numOfLineCounted();
}

void parserClass::parseProgram()
{
	token = theScanner.getToken();
	parse_program(); // parse whole program
	//parse_block(); // can only start with block for now
	//parse_stmt(); //can only start with statments for now
}

/***************************************************beginning of statments parse ********************************************/
void parserClass::parse_stmt()
{
	if (token.getTokenType() == WHILE_T)
	{
		token = theScanner.getToken();
		parse_expr();
		if (token.getTokenType() == DO_T)
		{
			token = theScanner.getToken();
			parse_stmt();
		}
		else
		{
			errorexit("keyword [do]", token.getLexeme());
		}
	}
	else if (token.getTokenType() == IF_T)
	{
		token = theScanner.getToken();
		parse_expr();
		if (token.getTokenType() == THEN_T)
		{
			token = theScanner.getToken();
			parse_stmt();
			parse_flIf();
		}
		else
		{
			errorexit("keyword [then]", token.getLexeme());
		}
	}
	else if (token.getTokenType() == IDEN_T)
	{
		token = theScanner.getToken();
		parse_flId();
	}
	else if (token.getTokenType() == COUT_T)
	{
		token = theScanner.getToken();
		parse_flCout();
	}
	else if (token.getTokenType() == CIN_T)
	{
		token = theScanner.getToken();
		parse_flCin();
	}
	else if (token.getTokenType() == BEGIN_T)
	{
		token = theScanner.getToken();
		parse_flBegin();
	}
	else
	{
		errorexit("one of those keywords: [while, if, cout, cin, begin] / an identifier", token.getLexeme()); 
	}
}

void parserClass::parse_flIf()
{
	if (token.getTokenType() == ELSE_T)
	{
		token = theScanner.getToken();
		parse_stmt();
	}
	else if (token.getTokenType() == SEMICOLN_T || token.getTokenType() == END_T) // the only things can follow fl-if if it's empty is ; or keyword end
		return;
	else
		errorexit("a semicolon / one of those keywords [else, end]", token.getLexeme());
}

void parserClass::parse_flId()
{
	if (token.getTokenType() == ASSIGN_T)
	{
		token = theScanner.getToken();
		parse_expr();
	}
	else if (token.getTokenType() == SWAP_T)
	{
		token = theScanner.getToken();
		if (token.getTokenType() == IDEN_T)
		{
			token = theScanner.getToken();
			return;
		}
		else
			errorexit("an identifier", token.getLexeme());
	}
	else if (token.getTokenType() == OPENPARENT_T)
	{
		token = theScanner.getToken();
		parse_expr();
		parse_flexpr();
		if (token.getTokenType() == CLOSEPARENT_T)
		{
			token = theScanner.getToken();
			return;
		}
		else
			errorexit("a close parenthese", token.getLexeme());
	}
	else if (token.getTokenType() == SEMICOLN_T || token.getTokenType() == END_T) // the only things can follow fl-id if it's empty is ; or keyword end
		return;
	else
		errorexit("a '=' / a '><' / an open parentheses / a semicolon / a keyword [end]", token.getLexeme());
}

void parserClass::parse_flCout()
{
	if (token.getTokenType() == LESSLESS_T)
	{
		token = theScanner.getToken();
		if (token.getTokenType() == STRINGCONST_T)
		{
			token = theScanner.getToken();
			if (token.getTokenType() == LESSLESS_T)
			{
				parse_flCout();
			}
			else
				return;
		}
		else if (token.getTokenType() == ENDL_T)
		{
			token = theScanner.getToken();
			if (token.getTokenType() == LESSLESS_T)
			{
				parse_flCout();
			}
			else
				return;
		}
		else // when it is expr
		{
			parse_expr();
			if (token.getTokenType() == LESSLESS_T)
			{
				parse_flCout();
			}
			else
				return;
		}
	}
	else
		errorexit("a '<<' after keyword [cout]", token.getLexeme());
}

void parserClass::parse_flCin()
{
	if (token.getTokenType() == GREATGREAT_T)
	{
		token = theScanner.getToken();
		if (token.getTokenType() == IDEN_T)
		{
			token = theScanner.getToken();
			if (token.getTokenType() == GREATGREAT_T)
			{
				parse_flCin();
			}
			else if (token.getTokenType() == SEMICOLN_T || token.getTokenType() == END_T)
				return; 
			else
				errorexit("a '>>' / a semicolon / a keyword [end]", token.getLexeme());
		}
		else
			errorexit("an identifier after '>>'", token.getLexeme());
	}
	else if (token.getTokenType() == SEMICOLN_T || token.getTokenType() == END_T) // the only things can follow flCin if it's empty is ; or keyword end
		return;
	else
		errorexit("a '>>' / a semicolon / a keyword [end]", token.getLexeme());
}

void parserClass::parse_flBegin()
{
	if (token.getTokenType() == END_T)
	{
		token = theScanner.getToken();
		return;
	}
	else if (token.getTokenType() == WHILE_T || token.getTokenType() == IF_T || token.getTokenType() == IDEN_T || token.getTokenType() == COUT_T || token.getTokenType() == CIN_T || token.getTokenType() == BEGIN_T)
	{
		parse_stmt();
		parse_stmtail();
		if (token.getTokenType() == END_T)
		{
			token = theScanner.getToken();
			return;
		}
		else
			errorexit("a semicolon to seperate multiple statements / a keyword [end] after last statement", token.getLexeme());
	}
	else
		errorexit("one of those keyword [end, while, if, cout, cin, begin] / an identifier", token.getLexeme());
}

void parserClass::parse_stmtail()
{
	if (token.getTokenType() == SEMICOLN_T)
	{
		token = theScanner.getToken();
		parse_stmt();
		parse_stmtail();
	}
	else if (token.getTokenType() == END_T)  // the only thing follows stmttail if it's empty is keyword end
		return;
	else 
		errorexit("a semicolon to seperate multiple statements / a keyword [end] after last statement", token.getLexeme());
}

void parserClass::parse_flexpr()
{
	if (token.getTokenType() == COMMA_T)
	{
		token = theScanner.getToken();
		parse_expr();
		parse_flexpr();
	}
	else if (token.getTokenType() == CLOSEPARENT_T) // the only thing can follow flexpr is close parenthese if it's empty
		return;
	else
		errorexit("a comma / a close parentheses", token.getLexeme());
}
/***************************************************end of statments parse ********************************************/

/***************************************************beginning of functions parse ********************************************/
void parserClass::parse_block()
{
	parse_varDecs();
	parse_funDecs();
	parse_progbody();
}

void parserClass::parse_type()
{
	if (token.getTokenType() == INT_T)
	{
		token = theScanner.getToken();
		return;
	}
	else if (token.getTokenType() == BOOLEAN_T)
	{
		token = theScanner.getToken();
		return;
	}
	else
		errorexit("one of those types [int, boolean]", token.getLexeme());
}

void parserClass::parse_varDecs()
{
	if (token.getTokenType() == INT_T || token.getTokenType() == BOOLEAN_T)
	{
		parse_type();
		parse_identList();
		if (token.getTokenType() == SEMICOLN_T)
		{
			token = theScanner.getToken();
			if (token.getTokenType() == INT_T || token.getTokenType() == BOOLEAN_T)
				parse_varDecs();
			else if (token.getTokenType() == FUNCTION_T || token.getTokenType() == BEGIN_T) //something follow var-decs whent it's empty
				return;
			else
				errorexit("one of those types [int, boolean] / one of those keywords [function, begin]", token.getLexeme());
		}
		else
			errorexit("a semicolon after the last identifier", token.getLexeme());
	}
	else if (token.getTokenType() == FUNCTION_T || token.getTokenType() == BEGIN_T)
		return;
	else
		errorexit("one of those types [int, boolean] / one of those keywords [function, begin]", token.getLexeme());
}

void parserClass::parse_identList()
{
	if (token.getTokenType() == IDEN_T)
	{
		token = theScanner.getToken();
		parse_identail();
	}
	else
		errorexit("an identifier after comma", token.getLexeme());
}

void parserClass::parse_identail()
{
	if (token.getTokenType() == COMMA_T)
	{
		token = theScanner.getToken();
		parse_identList();
	}
	else if (token.getTokenType() == SEMICOLN_T)
		return;
	else
		errorexit("a comma between multiple identifiers / a semicolon after the last identifier", token.getLexeme());
}

void parserClass::parse_funDecs()
{
	if (token.getTokenType() == FUNCTION_T)
	{
		token = theScanner.getToken();
		if (token.getTokenType() == IDEN_T)
		{
			token = theScanner.getToken();
			parse_funcDecTail();
			if (token.getTokenType() == SEMICOLN_T)
			{
				token = theScanner.getToken();
				parse_block();
				if (token.getTokenType() == SEMICOLN_T)
				{
					token = theScanner.getToken();
					parse_funDecs();
				}
				else
					errorexit("a semicolon after the block", token.getLexeme());
			}
			else
				errorexit("a semicolon after the parameter list / a semicolon after the function identifier", token.getLexeme());
		}
		else
			errorexit("an identifier after keyword [function]", token.getLexeme());
	}
	else if (token.getTokenType() == BEGIN_T)
		return;
	else
		errorexit("one of those keywords [function, begin]", token.getLexeme());
}

void parserClass::parse_funcDecTail()
{
	if (token.getTokenType() == OPENPARENT_T)
	{
		token = theScanner.getToken();
		parse_paramList();
		if (token.getTokenType() == CLOSEPARENT_T)
		{
			token = theScanner.getToken();
			return;
		}
		else
			errorexit("a close parentheses", token.getLexeme()); 
	}
	else if (token.getTokenType() == SEMICOLN_T)
		return;
	else
		errorexit("an open parentheses after function identifier / a semicolon after function identifier", token.getLexeme());
}

void parserClass::parse_paramList()
{
	if (token.getTokenType() == INT_T || token.getTokenType() == BOOLEAN_T)
	{
		parse_type();
		parse_typeTail();
	}
	else
		errorexit("one of those types [int, boolean]", token.getLexeme());
}

void parserClass::parse_typeTail()
{
	if (token.getTokenType() == IDEN_T)
	{
		token = theScanner.getToken();
		parse_identail2();
	}
	else if (token.getTokenType() == AMPERSAND_T)
	{
		token = theScanner.getToken();
		if (token.getTokenType() == IDEN_T)
		{
			token = theScanner.getToken();
			parse_identail2();
		}
		else
			errorexit("an identifier after '&'", token.getLexeme());
	}
	else
		errorexit("an identifier / an '&'", token.getLexeme());
}

void parserClass::parse_identail2()
{
	if (token.getTokenType() == COMMA_T)
	{
		token = theScanner.getToken();
		if (token.getTokenType() == INT_T || token.getTokenType() == BOOLEAN_T)
		{
			parse_paramList();
		}
		else
			errorexit("one of those types [int, boolean]", token.getLexeme());
	}
	else if (token.getTokenType() == CLOSEPARENT_T)
		return;
	else
		errorexit("a comma between multiple parameters / a close parentheses at the end of parameter list", token.getLexeme());
}

void parserClass::parse_progbody()
{
	if (token.getTokenType() == BEGIN_T)
	{
		token = theScanner.getToken();
		parse_flBegin();
	}
	else
		errorexit("a keyword [begin]", token.getLexeme());
}
/***************************************************end of functions parse ********************************************/

/***************************************************beginning of program parse ********************************************/
void parserClass::parse_program()
{
	if (token.getTokenType() == PROGRAM_T)
	{
		token = theScanner.getToken();
		if (token.getTokenType() == IDEN_T)
		{
			token = theScanner.getToken();
			if (token.getTokenType() == SEMICOLN_T)
			{
				token = theScanner.getToken();
				parse_block();
				if (token.getTokenType() == PERIOD_T)
					return; //end of whole program
				else
					errorexit("a period after the block", token.getLexeme());
			}
			else
				errorexit("a semicolon after program identifier", token.getLexeme());
		}
		errorexit("an identifier after keyword [program]", token.getLexeme());
	}
	errorexit("keyword [program]", token.getLexeme());
}
/***************************************************end of program parse ********************************************/

void parserClass::errorexit(string errormsg1, string errormsg2)
{
	cout << "Error: Expecting " << errormsg1 << " but found " << "[" <<errormsg2 << "]" << " at or near" << endl;
	theScanner.printCurrentLine();
	cin.get();
	exit(1);
}