#ifndef LEXTREE_H
#define LEXTREE_H
#include "..\General\LinkedList.h"
#include "..\Parser\SLRParser.h"


extern const void * LEXTREE;

struct LexTree{
	void *internals;
	struct LexTree * (*init)(struct LexTree *__ptrInput, char *__strLexDef);
	void (*Print)(struct LexTree *__ptrInput);
	//Values are of the LexTreeNode Type
	struct ParseTree *ParseTree;
};

//CONSTAR is a concate with a start at the end. The star is for the second "S"
enum LexNodeTypes{
	CONCATE, 
	LITERAL, 
	STAR, 
	OR, 
	INIT, 
	EXTRA, 
	CONSTAR
};


struct LexTreeNode{
	char *Value;
	enum LexNodeTypes NodeType;
};




#endif LEXTREE_H