//Congshu Wang revised on Sep 11, 2014
//This file contains some general constant definitions that will
//be useful in several parts of the compiler.  Feel free to add
//additional definitions here.

#ifndef _general
#define _general

const int MAXCELLSPERSTRING = 82;  //80 characters as well as \n plus \0.

//Define constants for tokens
const int ASSIGN_T = 0;
const int RELOP_T = 1;
const int SWAP_T = 2;  //><
const int GREATGREAT_T = 3; //>>
const int LESSLESS_T = 4;  //<<
const int ADDOPS_T = 5;
const int MULOPS_T = 6;
const int OPENPARENT_T = 7; //(
const int CLOSEPARENT_T = 8; //)
const int SEMICOLN_T = 9; //;
const int COMMA_T = 10; //,
const int PERIOD_T = 11;  //.
const int AMPERSAND_T = 12;  //&
const int PROGRAM_T = 13;  
const int FUNCTION_T = 14;
const int BEGIN_T = 15;
const int END_T = 16;
const int IF_T = 17;
const int THEN_T = 18;
const int ELSE_T = 19;
const int WHILE_T = 20;
const int DO_T = 21;
const int COUT_T = 22;
const int CIN_T = 23;
const int ENDL_T = 24;
const int INT_T = 25;
const int BOOLEAN_T = 26;
const int TRUE_T = 27;
const int FALSE_T = 28;
const int IDEN_T = 29;
const int STRINGCONST_T = 30;
const int INTCONST_T = 31;

const int NONE = 100;
const int NOTEQ_ST = 101; //<>
const int EQUAL_ST = 102; //==
const int LESSTHAN_ST = 103; //<
const int LESSEQ_ST = 104; //<=
const int GREATTHAN_ST = 105; //>
const int GREATEQ_ST = 106; //>=
const int ADDITION_ST = 107; //+
const int SUBSTRACTION_ST = 108; //-
const int OR_ST = 109; // or
const int AND_ST = 110; //and
const int MULTIPLICATION_ST = 111; //*
const int DIVISION_ST = 112; //div
const int MODULO_ST = 113; //mod

const int EOF_T = 200;


#endif