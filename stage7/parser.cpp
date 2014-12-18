//Congshu Wang revised on Nov 25, 2014

#include "stdafx.h"
#include "parser.h"

parserClass::parserClass()
{
	outfile.open("out.pal", ios::out);
}

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

	printInstruction("movw", NULL, "sp", NULL, "r1");
	pStackClass theStack;
	startToken.setToken(";", SEMICOLN_T, NONE);
	theStack.push(startToken, NULL);

	while (1)
	{
		if (((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == SEMICOLN_T)) || ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == DO_T))
			|| ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == THEN_T)) || ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == COMMA_T))
			|| ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == LESSLESS_T)) || ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == ELSE_T))
			|| ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == CLOSEPARENT_T)) || ((theStack.returnTopMostTerminal() == SEMICOLN_T) && (token.getTokenType() == END_T)))
		{
			printInstruction("movw", theStack.top->symbol, "", NULL, "@r1");
			printInstruction("movw", NULL, "r1", NULL, "sp");
			printInstruction("addw", NULL, intToPali(4), NULL, "sp");
			//Store the type to be returned before deleting the temp variables in the table
			returnedSymbolType = theStack.top->symbol->symbolType;
			theScanner.theSymbolTable.top->deleteTempSymbol();
			return;
		}
		else
		{
			topTerm = theStack.returnTopMostTerminal();
			if (checkPrecedence(topTerm, token.getTokenType()) == LT || checkPrecedence(topTerm, token.getTokenType()) == EQ)
			{
				theStack.push(token, NULL);
				token = theScanner.getToken();
				//theStack.helper();
			}
			else if (checkPrecedence(topTerm, token.getTokenType()) == GT)
			{
				int counter = 0;
				int tempArrayType[3];
				int tempArraySubType[3];
				string tempArrayLexeme[3];
				SymbolNode* tempArraySymbol[3];
				do
				{
					tempArrayType[counter] = theStack.pop();//call pop to return the last popped token type, subtype, lexeme and a point to symbol in the symbol table
					tempArraySubType[counter] = theStack.returnLastPoppedTokenSubType();//put the assigned subtype to subtype array
					tempArrayLexeme[counter] = theStack.returnLastPoppedTokenLexeme();//put the assigned lexeme to lexeme array
					tempArraySymbol[counter] = theStack.returnLastPoppedTokenSymbol();//put the assigned pointer of symbol to an array of symbol pointers
					++counter;
				} while (noTerminalsHaveBeenPopped(tempArrayType, counter) || theStack.isTerminalOnTop() == false || checkPrecedence(theStack.returnTopMostTerminal(), theStack.returnLastTerminalPopped()) != LT);
				if (isValidRHS(tempArrayType, tempArraySubType, tempArrayLexeme, tempArraySymbol, counter))
				{
					theStack.pushEOnStack(symbolPushOnStack);
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

bool parserClass::isValidRHS(int tempArrayType[], int tempArraySubType[], string tempArrayLexeme[], SymbolNode* tempArraySymbol[], int counter)
{
	if (counter == 1)
	{
		SymbolNode *temp = NULL;
		switch (tempArrayType[0])
		{
		case IDEN_T:
			symbolPushOnStack = theScanner.theSymbolTable.tableLookUp(tempArrayLexeme[0]);
			if (symbolPushOnStack == NULL)
				undeclaredErrorExit(tempArrayLexeme[0]);
			if (theScanner.returnExprDebugStatus() == true)
				cout << "E -> id" << endl;
			return true;
			break;
		case INTCONST_T:
			if (theScanner.returnExprDebugStatus() == true)
				cout << "E -> integer-constant" << endl;
			temp = addTempVar(INT_TYPE);
			symbolPushOnStack = theScanner.theSymbolTable.tableLookUp(temp->symbolLexeme);
			if (symbolPushOnStack == NULL)
				undeclaredErrorExit(temp->symbolLexeme);
			printInstruction("movw", NULL, strToPali(tempArrayLexeme[0]), temp, "");
			return true;
			break;
		case TRUE_T:
			if (theScanner.returnExprDebugStatus() == true)
				cout << "E -> true" << endl;
			temp = addTempVar(BOOLEAN_TYPE);
			symbolPushOnStack = theScanner.theSymbolTable.tableLookUp(temp->symbolLexeme);
			if (symbolPushOnStack == NULL)
				undeclaredErrorExit(temp->symbolLexeme);
			printInstruction("movw", NULL, intToPali(1), temp, "");
			return true;
			break;
		case FALSE_T:
			if (theScanner.returnExprDebugStatus() == true)
				cout << "E -> false" << endl;
			temp = addTempVar(BOOLEAN_TYPE);
			symbolPushOnStack = theScanner.theSymbolTable.tableLookUp(temp->symbolLexeme);
			if (symbolPushOnStack == NULL)
				undeclaredErrorExit(temp->symbolLexeme);
			printInstruction("movw", NULL, intToPali(0), temp, "");
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
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != INT_TYPE)
					wrongTypeErrorExit("Addtion", "Integer");
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliArithmetic("addw", e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == MULTIPLICATION_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E * E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != INT_TYPE)
					wrongTypeErrorExit("Multiplication", "Integer");
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliArithmetic("mulw", e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == SUBSTRACTION_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E - E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != INT_TYPE)
					wrongTypeErrorExit("Substraction", "Integer");
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliArithmetic("subw", e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == DIVISION_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E div E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != INT_TYPE)
					wrongTypeErrorExit("Division", "Integer");
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliArithmetic("divw", e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == MODULO_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E mod E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != INT_TYPE)
					wrongTypeErrorExit("Modulo", "Integer");
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliMod(e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == OR_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E or E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != BOOLEAN_TYPE)
					wrongTypeErrorExit("Or", "Boolean");
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliOr(e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == AND_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E and E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != BOOLEAN_TYPE)
					wrongTypeErrorExit("And", "Boolean");
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliAnd(e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == EQUAL_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E == E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliBranch("beq", e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == LESSTHAN_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E < E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliBranch("blss", e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == LESSEQ_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E <= E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliBranch("bleq", e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == GREATTHAN_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E > E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliBranch("bgtr", e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == GREATEQ_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E >= E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliBranch("bgeq", e1, e2);
				return true;
			}
			else if ((tempArraySubType[1] == NOTEQ_ST) && tempArrayType[2] == E_T)
			{
				if (theScanner.returnExprDebugStatus() == true)
					cout << "E -> E <> E" << endl;
				SymbolNode *e1 = tempArraySymbol[2];
				SymbolNode *e2 = tempArraySymbol[0];
				if (e1 == NULL || e2 == NULL)
					return false;
				if (e1->symbolType != e2->symbolType)
					notSameTypeErrorExit(e1->symbolLexeme, e2->symbolLexeme);
				symbolPushOnStack = paliBranch("bneq", e1, e2);
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
				SymbolNode *e1 = tempArraySymbol[1];
				if (e1 == NULL)
					return false;
				symbolPushOnStack = e1;
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
		string whileTop = getNextTempVar();
		string whileBottom = getNextTempVar();
		printBranch(whileTop);
		token = theScanner.getToken();
		parse_expr();
		if (returnedSymbolType != BOOLEAN_TYPE)
		{
			cout << "Error: The while loop condition must return a boolean type." << endl;
			theScanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		//Compare 0 with with the result of the condition statement of while loop.
		printInstruction("cmpw", NULL, intToPali(0), NULL, "@r1");
		//Clear the temp variable.
		printInstruction("movw", NULL, "r1", NULL, "sp");//subw #4 sp
		//Print after branch
		printInstruction("beq", NULL, whileBottom);
		if (token.getTokenType() == DO_T)
		{
			token = theScanner.getToken();
			parse_stmt();
			//jump statement
			printInstruction("jmp", NULL, whileTop);
			//print after label
			printBranch(whileBottom);//end of while
		}
		else
		{
			errorexit("keyword [do]", token.getLexeme());
		}
	}
	else if (token.getTokenType() == IF_T)
	{
		elseLable = getNextTempVar();
		afterIfLabel = getNextTempVar();
		token = theScanner.getToken();
		parse_expr();
		if (returnedSymbolType != BOOLEAN_TYPE)
		{
			cout << "Error: The if statement condition must return a boolean type." << endl;
			theScanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		if (token.getTokenType() == THEN_T)
		{
			token = theScanner.getToken();

			printInstruction("cmpw", NULL, intToPali(0), NULL, "@r1");
			//Clear the temp variable.
			printInstruction("movw", NULL, "r1", NULL, "sp");//subw #4 sp
			//Print skip branch
			printInstruction("beq", NULL, elseLable);
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
		idLookUpResult = theScanner.theSymbolTable.tableLookUp(token.getLexeme());
		if (idLookUpResult == NULL)
			undeclaredErrorExit(token.getLexeme());
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
		printInstruction("jmp", NULL, afterIfLabel);
		printBranch(elseLable);
		printInstruction("movw", NULL, "r0", NULL, "r0");
		parse_stmt();
		printBranch(afterIfLabel);
		printInstruction("movw", NULL, "r0", NULL, "r0");
	}
	else if (token.getTokenType() == SEMICOLN_T || token.getTokenType() == END_T) // the only things can follow fl-if if it's empty is ; or keyword end
	{
		printBranch(elseLable);
		printInstruction("movw", NULL, "r0", NULL, "r0");
		return;
	}
	else
		errorexit("a semicolon / one of those keywords [else, end]", token.getLexeme());
}

void parserClass::parse_flId()
{
	if (token.getTokenType() == ASSIGN_T)
	{
		token = theScanner.getToken();
		parse_expr();
		if (idLookUpResult->symbolType != returnedSymbolType)
		{
			cout << "Error: Can't do assignment with two different types." << endl;
			theScanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		printInstruction("movw", NULL, "@r1", idLookUpResult, ""); // store the answer in temp to the token on the left of =
		printInstruction("movw", NULL, "r1", NULL, "sp");
	}
	else if (token.getTokenType() == SWAP_T)
	{
		token = theScanner.getToken();
		if (token.getTokenType() == IDEN_T)
		{
			SymbolNode *result = theScanner.theSymbolTable.tableLookUp(token.getLexeme());
			if (result == NULL)
				undeclaredErrorExit(token.getLexeme());
			else
			{
				if (idLookUpResult->symbolType != result->symbolType)
				{
					cout << "Error: Can't do swap with two different types." << endl;
					theScanner.printCurrentLine();
					cin.get();
					exit(1);
				}
				//move the first parameter to the top of the stack
				printInstruction("movw", idLookUpResult, "", NULL, "@sp");
				//increase stack pointer
				printInstruction("addw", NULL, intToPali(4), NULL, "sp");
				//move the second parameter to the first parameter
				printInstruction("movw", result, "", idLookUpResult, "");
				//move the first parameter back
				printInstruction("movw", NULL, "-4@sp", result, "");
				//decrease sp to remove the temp
				printInstruction("subw", NULL, intToPali(4), NULL, "sp");
			}
			token = theScanner.getToken();
			return;
		}
		else
			errorexit("an identifier", token.getLexeme());
	}
	else if (token.getTokenType() == OPENPARENT_T)
	{
		//funcParamsCount = 1;
		token = theScanner.getToken();

		/*---------------------*/
		SymbolNode* param = idLookUpResult->parameterTop;
		if (param->symbolKind == REF_PARAM)
		{
			SymbolNode *funcParamLookUpResult = theScanner.theSymbolTable.tableLookUp(token.getLexeme());
			if (funcParamLookUpResult != NULL)
			{
				token = theScanner.getToken();
				//Move the address of the parameter on to the top of the stack
				printInstruction("mova", funcParamLookUpResult, "", NULL, "@sp");
				//Move SP up
				printInstruction("addw", NULL, intToPali(4), NULL, "sp");
			}
			else
			{
				cout << "Error: Can't find the identifier in this type." << endl;
				theScanner.printCurrentLine();
				cin.get();
				exit(1);
			}
		}
		else // when symbol kind is value param
		{
			parse_expr();
			if (returnedSymbolType != param->symbolType)
			{
				if (param->symbolType == INT_TYPE)
					 requiredType = "Integer Type";
				else if (param->symbolType == BOOLEAN_TYPE)
					 requiredType = "Boolean Type";
				else
					 requiredType = "Other Type";

				if (returnedSymbolType == INT_TYPE)
					 returnedType = "Integer Type";
				else if (returnedSymbolType == BOOLEAN_TYPE)
					 returnedType = "Boolean Type";
				else
					 returnedType = "Other Type";

				cout << "Error: Incorrect matching of parameter types. " << requiredType << " is required but found " << returnedType << endl;
				theScanner.printCurrentLine();
				cin.get();
				exit(1);
			}
		}
		parse_flexpr(param->next);//parse other params
		if (token.getTokenType() == CLOSEPARENT_T)
		{
			//when funcParamsCount == numOfFuncParams
			printInstruction("call", NULL, intToPali(idLookUpResult->numOfParams), NULL, idLookUpResult->symbolLabel);
			printInstruction("subw", NULL, intToPali((idLookUpResult->numOfParams) * 4), NULL, "sp");
			token = theScanner.getToken();
			return;
		}
		else
			errorexit("a close parenthese", token.getLexeme());
	}
	else if (token.getTokenType() == SEMICOLN_T || token.getTokenType() == END_T)
	{
		if (idLookUpResult->symbolKind == FUNC_ID) // new added
		{
			// the only things can follow fl-id if it's empty is ; or keyword end
			if (idLookUpResult->numOfParams != 0)
			{
				//if the function has parameters required but found nothing
				cout << "Error: Incorrect number of parameter(s).  Expecting " << idLookUpResult->numOfParams << " parameter(s) but found none." << endl;
				theScanner.printCurrentLine();
				cin.get();
				exit(1);
			}
			//call a function with no params
			printInstruction("call", NULL, intToPali(0), NULL, idLookUpResult->symbolLabel);
			return;
		}
	}
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
			string temp = token.getLexeme();
			for (int i = 0; i < temp.length(); i++)
			{
				if (temp[i] == ' ')
				{
					outfile << "outb #32" << endl;
				}
				else
					outfile << "outb ^" << temp[i] << endl;
			}
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
			printInstruction("outb", NULL, intToPali(10));
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
			//First print the result of the expression
			outfile << "outw @r1" << endl;
			//Then remove the temp from the top of the stack
			outfile << "movw r1 sp" << endl;
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
			SymbolNode *result = theScanner.theSymbolTable.tableLookUp(token.getLexeme());
			if (result == NULL)
				undeclaredErrorExit(token.getLexeme());
			if (result->symbolType != INT_TYPE)
				wrongTypeErrorExit("Cin", "Integer");
			int theOffset = (result->offset) * 4;
			outfile << "inw +" << theOffset << "@r0" << endl;
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
	{
		outfile << "inb $junk" << endl;
		return;
	}
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

void parserClass::parse_flexpr(SymbolNode *param)
{
	if (token.getTokenType() == COMMA_T)
	{
		if (param == NULL) // number of params in function call is larger than it should be
		{
			cout << "Error: Incorrect number of parameter(s)." << endl;
			theScanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		token = theScanner.getToken();
		if (param->symbolKind == REF_PARAM)
		{
			SymbolNode *funcParamLookUpResult = theScanner.theSymbolTable.tableLookUp(token.getLexeme());
			if (funcParamLookUpResult != NULL)
			{
				token = theScanner.getToken();
				//Move the address of the parameter on to the top of the stack
				printInstruction("mova", funcParamLookUpResult, "", NULL, "@sp");
				//Move SP up
				printInstruction("addw", NULL, intToPali(4), NULL, "sp");
			}
			else
			{
				cout << "Error: Can't find the identifier in this type." << endl;
				theScanner.printCurrentLine();
				cin.get();
				exit(1);
			}
		}
		else // when symbol kind is value param
		{
			parse_expr();
			if (returnedSymbolType != param->symbolType)
			{
				if (param->symbolType == INT_TYPE)
					requiredType = "Integer Type";
				else if (param->symbolType == BOOLEAN_TYPE)
					requiredType = "Boolean Type";
				else
					requiredType = "Other Type";

				if (returnedSymbolType == INT_TYPE)
					returnedType = "Integer Type";
				else if (returnedSymbolType == BOOLEAN_TYPE)
					returnedType = "Boolean Type";
				else
					returnedType = "Other Type";

				cout << "Error: Incorrect matching of parameter types. " << requiredType << " is required but found " << returnedType << endl;
				theScanner.printCurrentLine();
				cin.get();
				exit(1);
			}
		}
		parse_flexpr(param->next);
	}
	else if (token.getTokenType() == CLOSEPARENT_T) // the only thing can follow flexpr is close parenthese if it's empty
	{
		if (param != NULL)//when actural params fewer than required
		{
			cout << "Error: Incorrect number of parameter(s)." << endl;
			theScanner.printCurrentLine();
			cin.get();
			exit(1);
		}
		return;
	}
	else
		errorexit("a comma / a close parentheses", token.getLexeme());
}
/***************************************************end of statments parse ********************************************/

/***************************************************beginning of functions parse ********************************************/
void parserClass::parse_block(int paramCounter)
{
	parse_varDecs();
	int globalVarSpaces = (theScanner.theSymbolTable.top->nextOffset) * 4;
	//move paramters from ap to above local params
	for (int i = 0; i < paramCounter; i++)
	{
		string apAddress = "-" + to_string(i * 4) + "@ap";
		string spAddress = "+" + to_string((paramCounter - i - 1) * 4) + "@sp";
		printInstruction("movw", NULL, apAddress, NULL, spAddress);
	}
	printInstruction("addw", NULL, intToPali(globalVarSpaces), NULL, "sp");
	//jump to the corresponding begin block
	string blockName = getNextTempVar();
	printInstruction("jmp", NULL, blockName);
	parse_funDecs();
	printBranch(blockName);
	parse_progbody();
}

void parserClass::parse_type()
{
	if (token.getTokenType() == INT_T)
	{
		symbol_type = INT_TYPE;
		token = theScanner.getToken();
		return;
	}
	else if (token.getTokenType() == BOOLEAN_T)
	{
		symbol_type = BOOLEAN_TYPE;
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
		if (!theScanner.theSymbolTable.tableAddEntry(new SymbolNode(token.getLexeme(), VAR_ID, symbol_type)))
		{
			symbol_type = NULL;
			dupErrorExit(token.getLexeme());
		}
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
			SymbolNode *funcSymbol = new SymbolNode(token.getLexeme(), FUNC_ID, FUNC_ID_TYPE);
			if (!theScanner.theSymbolTable.tableAddEntry(funcSymbol))
				dupErrorExit(token.getLexeme());
			string theSymbolLabel = getNextTempVar();
			funcSymbol->symbolLabel = theSymbolLabel;
			//start function call 
			printBranch(theSymbolLabel);
			theScanner.theSymbolTable.tableAddLevel(token.getLexeme());
			token = theScanner.getToken();
			parse_funcDecTail();
			if (token.getTokenType() == SEMICOLN_T)
			{
				token = theScanner.getToken();
				parse_block(theScanner.theSymbolTable.top->nextOffset);
				theScanner.theSymbolTable.tableDelLevel();
				if (token.getTokenType() == SEMICOLN_T)
				{
					printInstruction("movw", NULL, "fp", NULL, "sp");
					printInstruction("ret", NULL, "");
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
		if (!theScanner.theSymbolTable.addFuncParam(new SymbolNode(token.getLexeme(), VALUE_PARAM, symbol_type)))
		{
			symbol_type = NULL;
			dupErrorExit(token.getLexeme());
		}
		if (!theScanner.theSymbolTable.tableAddEntry(new SymbolNode(token.getLexeme(), VALUE_PARAM, symbol_type)))
		{
			symbol_type = NULL;
			dupErrorExit(token.getLexeme());
		}
		token = theScanner.getToken();
		parse_identail2();
	}
	else if (token.getTokenType() == AMPERSAND_T)
	{
		token = theScanner.getToken();
		if (token.getTokenType() == IDEN_T)
		{
			if (!theScanner.theSymbolTable.addFuncParam(new SymbolNode(token.getLexeme(), REF_PARAM, symbol_type)))
			{
				symbol_type = NULL;
				dupErrorExit(token.getLexeme());
			}
			if (!theScanner.theSymbolTable.tableAddEntry(new SymbolNode(token.getLexeme(), REF_PARAM, symbol_type)))
			{
				symbol_type = NULL;
				dupErrorExit(token.getLexeme());
			}
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
			theScanner.theSymbolTable.tableAddLevel("main");

			token = theScanner.getToken();
			if (token.getTokenType() == SEMICOLN_T)
			{
				outfile << "$junk #1" << endl;
				outfile << "$main movw sp r0" << endl;
				token = theScanner.getToken();
				parse_block(0);
				theScanner.theSymbolTable.tableDelLevel();
				if (token.getTokenType() == PERIOD_T)
				{
					outfile << "inb $junk" << endl;
					outfile << "end" << endl;
					outfile.close();
					return; //end of whole program
				}
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

void parserClass::dupErrorExit(string errormsg1)
{
	cout << "Error: Duplicate identifier " << "[" << errormsg1 << "]." << endl;
	theScanner.printCurrentLine();
	cin.get();
	exit(1);
}

void parserClass::undeclaredErrorExit(string errormsg1)
{
	cout << "Error: Identifier " << "[" << errormsg1 << "]" << " is undeclared." << endl;
	theScanner.printCurrentLine();
	cin.get();
	exit(1);
}

void parserClass::notSameTypeErrorExit(string errormsg1, string errormsg2)
{
	cout << "Error: " << "[" << errormsg1 << "] and " << "[" << errormsg2 << "] are not same type." << endl;
	theScanner.printCurrentLine();
	cin.get();
	exit(1);
}

void parserClass::wrongTypeErrorExit(string errormsg1, string errormsg2)
{
	cout << "Error: " << errormsg1 << " has to start with " << errormsg2 << "." << endl;
	theScanner.printCurrentLine();
	cin.get();
	exit(1);
}

string parserClass::getNextTempVar()
{
	stringstream ss;
	static int nextCt = 0;
	ss << "$" << nextCt++;
	return ss.str();
}

void parserClass::printInstruction(string instruction, SymbolNode *firstParamPointer, string firstParam, SymbolNode *secondParamPointer, string secondParam)
{
	outfile << instruction << " " << printInstructionHelper(firstParamPointer, firstParam) << " " << printInstructionHelper(secondParamPointer, secondParam) << endl;
}

void parserClass::printInstruction(string instruction, SymbolNode *firstParamPointer, string firstParam)
{
	outfile << instruction << " " << printInstructionHelper(firstParamPointer, firstParam) << endl;
}

void parserClass::printBranch(string branchName)
{
	outfile << branchName << " ";
}

string parserClass::printInstructionHelper(SymbolNode *paramPointer, string param)
{
	if (paramPointer == NULL)
		return param;
	string address = "";
	if (paramPointer->nestingLevel == 0)
		address = "@r0";
	else
		address = "@fp";
	if (paramPointer->symbolKind == REF_PARAM)
		address = "@" + address;
	string result = "+" + to_string((paramPointer->offset)*4) + address;
	return result;
}

string parserClass::intToPali(int n)
{
	return strToPali(to_string(n));
}

string parserClass::strToPali(string s)
{
	return "#" + s;
}

SymbolNode* parserClass::addTempVar(int symbolType)
{
	SymbolNode* node = new SymbolNode(getNextTempVar(), VAR_ID, symbolType);
	if (theScanner.theSymbolTable.tableAddEntry(node))
	{
		//Move SP up to give space for the new temp variable
		printInstruction("addw", NULL, intToPali(4), NULL, "sp");
		return node;
	}
	return NULL;
}

SymbolNode* parserClass::paliArithmetic(string operation, SymbolNode *node1, SymbolNode *node2)
{
	SymbolNode *temp = addTempVar(INT_TYPE);
	//First move the first parameter to temp location
	printInstruction("movw", node1, "", temp, "");
	//Then do the operation on the temp
	printInstruction(operation, node2, "", temp, "");
	return temp;
}

void parserClass::checkVariable(SymbolNode *node)
{
	checkVariable(node->symbolType);
}

void parserClass::checkVariable(int type)
{
	if (type != INT_TYPE && type != BOOLEAN_TYPE)
	{
		cout << "Error: Expecting an integer type variable or a boolean type variable." << endl;
		theScanner.printCurrentLine();
		cin.get();
		exit(1);
	}
}

SymbolNode* parserClass::paliMod(SymbolNode *node1, SymbolNode *node2)
{
	SymbolNode *temp1 = paliArithmetic("divw", node1, node2);
	SymbolNode *temp2 = paliArithmetic("mulw", node2, temp1);
	SymbolNode *temp3 = paliArithmetic("subw", node1, temp2);
	return temp3;
}

SymbolNode* parserClass::paliAnd(SymbolNode *node1, SymbolNode *node2)
{
	SymbolNode *result = paliArithmetic("mulw", node1, node2);
	result->symbolType = BOOLEAN_TYPE;
	return result;
}

SymbolNode* parserClass::paliOr(SymbolNode *node1, SymbolNode *node2)
{
	SymbolNode *andResult = paliAnd(node1, node2);
	SymbolNode *addResult = paliArithmetic("addw", node1, node2);
	SymbolNode *result = paliArithmetic("subw", addResult, andResult);
	result->symbolType = BOOLEAN_TYPE;
	return result;
}

SymbolNode* parserClass::paliBranch(string operation, SymbolNode *node1, SymbolNode *node2)
{
	//First compare the two parameters
	printInstruction("cmpw", node1, "", node2, "");
	//Add a temp variable as the result
	SymbolNode *temp = addTempVar(BOOLEAN_TYPE);
	//First set the temp to be true
	printInstruction("movw", NULL, intToPali(1), temp, "");
	string branchName = getNextTempVar();
	//Based on the comparision, either skip to the label, or 
	//carry out the next instruction, which sets the temp to false.
	printInstruction(operation, NULL, branchName);
	printInstruction("movw", NULL, intToPali(0), temp, "");
	printBranch(branchName);
	return temp;
}

