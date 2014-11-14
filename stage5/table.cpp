//Congshu Wang created on Nov 4, 2014

#include "stdafx.h"
#include "table.h"

SymbolNode::SymbolNode(string lexeme, int kind, int type)
{
	symbolLexeme = lexeme;
	symbolKind = kind;
	symbolType = type;
	parameterTop = NULL;
	next = NULL;
	numOfParams = 0;
}

SymbolNode::~SymbolNode()
{
	SymbolNode* p = parameterTop;
	while (p != NULL)
	{
		SymbolNode* temp = p;
		p = p->next;
		delete temp;
	}
}

bool SymbolNode::addParam(SymbolNode *parameter)
{
	if (symbolKind != FUNC_ID)
		throw exception("Adding parameter to a non-function symbol.");
	if (parameter->symbolKind != REF_PARAM && parameter->symbolKind != VALUE_PARAM)
		throw exception("Parameter can only be reference parameter or value parameter.");
	parameter->nestingLevel = nestingLevel + 1;
	if (parameterTop == NULL)
	{
		parameterTop = parameter;
		parameter->offset = numOfParams++;
		return true;
	}
	//add the the parameter to the end of the list so when printing out parameters are in order
	SymbolNode *end = parameterTop;
	if (end->symbolLexeme == parameter->symbolLexeme)
		return false;
	while (end->next != NULL)
	{
		end = end->next;
		if (end->symbolLexeme == parameter->symbolLexeme)
			return false;
	}
	parameter->offset = numOfParams++;
	end->next = parameter;
	return true;
}

string SymbolNode::print()
{
	string s = printParam();
	s += ", nesting level ";
	s += to_string(nestingLevel);
	if (parameterTop != NULL)
	{
		s += "\n\tparam-list:";
		SymbolNode *p = parameterTop;
		while (p != NULL)
		{
			s += "\n\t" + p->printParam();
			p = p->next;
		}
	}
	return s;
}

string SymbolNode::printParam()
{
	string s = "lexeme=";
	s += symbolLexeme;
	s += ", kind=";
	switch (symbolKind)
	{
	case FUNC_ID:
		s += "func-id";
		break;
	case VAR_ID:
		s += "var-id";
		break;
	case REF_PARAM:
		s += "ref-param";
		break;
	case VALUE_PARAM:
		s += "value-param";
		break;
	default:
		break;
	}
	s += ", type=";
	switch (symbolType)
	{
	case FUNC_ID_TYPE:
		s += "func-id-type";
		break;
	case INT_TYPE:
		s += "int-type";
		break;
	case BOOLEAN_TYPE:
		s += "boolean-type";
		break;
	default:
		break;
	}
	s += ", offset=";
	s += to_string(offset);
	return s;
}

tableLevel::tableLevel(string scopeName, int theNestingLevel)
{
	name = scopeName;
	nestingLevel = theNestingLevel;
	nextOffset = 0;
	top = NULL;
	next = NULL;
}

tableLevel::~tableLevel()
{
	SymbolNode* p = top;
	while (p != NULL)
	{
		SymbolNode* temp = p;
		p = p->next;
		delete temp;
	}
}

SymbolNode* tableLevel::lookup(string lexeme)
{
	SymbolNode *p = top;
	if (p == NULL)
		return NULL;
	if (_stricmp(p->symbolLexeme.c_str(), lexeme.c_str()) == 0)
		return p;
	while (p->next != NULL)
	{
		if (_stricmp(p->next->symbolLexeme.c_str(), lexeme.c_str()) == 0)
		{
			SymbolNode *found = p->next;
			p->next = found->next;
			found->next = top;
			top = found;
			return found;
		}
		p = p->next;
	}
	return NULL;
}

bool tableLevel::addSymbol(SymbolNode *node)
{
	if (lookup(node->symbolLexeme) != NULL)
		return false;
	node->nestingLevel = nestingLevel;
	node->offset = nextOffset;
	if (node->symbolKind != FUNC_ID)
		nextOffset++;
	node->next = top;
	top = node;
	return true;
}

string tableLevel::print()
{
	string s = "Name = ";
	s += name;
	s += " Nesting level = ";
	s += to_string(nestingLevel);
	if (top != NULL)
	{
		SymbolNode *p = top;
		while (p != NULL)
		{
			s += "\n" + p->print();
			p = p->next;
		}
	}
	return s;
}

tableClass::tableClass()
{
	top = NULL;
	nextNestingLevel = 0;
}

tableClass::~tableClass()
{
	while (top != NULL)
		tableDelLevel();
}

void tableClass::tableAddLevel(string scope)
{
	string currentScope = "";
	if (top != NULL)
		currentScope = top->name + "." + scope;
	else
		currentScope = scope;
	tableLevel *level = new tableLevel(currentScope, nextNestingLevel++);
	level->next = top;
	top = level;
}	

void tableClass::tableDelLevel()
{
	if (top == NULL)
		throw exception("Can't delete anymore since top level is empty.");
	tableLevel *temp = top;
	top = top->next;
	delete temp;
	nextNestingLevel--;
}

bool tableClass::tableAddEntry(SymbolNode *node)
{
	if (top == NULL)
		throw exception("Can't add symbol since no level created yet.");
	return top->addSymbol(node);
}

SymbolNode* tableClass::tableLookUp(string lexeme)
{
	tableLevel *p = top;
	while (p != NULL)
	{
		SymbolNode *levelResult = p->lookup(lexeme);
		if (levelResult != NULL)
			return levelResult;
		p = p->next;
	}
	return NULL;
}

string tableClass::print()
{
	string s = "***********************************************************************";
	tableLevel *p = top;
	while (p != NULL)
	{
		s += "\n" + p->print() + "\n";
		p = p->next;
	}
	return s;
}

bool tableClass::addFuncParam(SymbolNode *parameter)
{
	if (top == NULL || top->next == NULL)
		throw exception("Can't add parameter since can't find the scope.");
	tableLevel *lastLevel = top->next;
	if (lastLevel->top == NULL)
		throw exception("Can't add parameter since can't find function identifier.");
	return lastLevel->top->addParam(parameter);
}