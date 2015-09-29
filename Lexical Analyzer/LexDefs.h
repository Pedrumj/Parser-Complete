#ifndef LEXDEFS_H
#define LEXDEFS_H

#include "..\General\LinkedList.h"
#include "RegDefs.h"
#include "LexTree.h"
extern const void * LEXDEFS;
#define LEXDEF_COUNT 13

//This class contains a list of LexTrees
struct LexDefs{
	void *internals;
	struct LexDefs * (*init)(struct LexDefs *__ptrInput, struct RegDefs *__ptrRegDefs);
	void (*Print)(struct LexDefs *__ptrInput);
	struct LinkedList *LexTrees;

};



enum Tokens{
 WS,
 IF, 
 THEN, 
 ELSE, 
 ID,
 NUMBER,
 RELOP, 
 SINGLE, 
 WHILE
};

enum Sub_Types{
	LT, 
	LE, 
	EQ, 
	NE, 
	GT, 
	GE, 
	NONE
};

//The type assocaited with each lex def
static enum Tokens lexDef_Token[LEXDEF_COUNT] = {WS, 
										  IF, 
										  WHILE, 
										  THEN, 
										  ELSE, 
										  ID, 
										  NUMBER,
										  RELOP, 
										  RELOP,
										  RELOP,
										  RELOP,
										  RELOP,
										  RELOP 
										  

};

//the subtype associated with each lex def
static enum Sub_Types lexDef_ST[LEXDEF_COUNT] = {NONE, 
										  NONE, 
										  NONE, 
										  NONE, 
										  NONE, 
										  NONE,
										  NONE, 
										  LT, 
										  LE,
										  EQ,
										  NE,
										  GT,
										  GE
										  

};

//The regex name associated with each lex def. Can be found in RegDefs.c
static char * lexDef_RegName[LEXDEF_COUNT] = {"ws", 
									   "IF", 
									   "WHILE", 
									   "THEN", 
									   "ELSE", 
									   "id", 
									   "number",
									   "LT", 
									   "LE", 
									   "EQ", 
									   "NE", 
									   "GT", 
									   "GE"
									   
};


#endif LEXDEFS_H