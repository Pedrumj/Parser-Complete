#include "RegDefs.h"
#include "LexDefs.h"
#include "..\General\Class_Generic.h"
#include <stdio.h>
#include "String.h"
#include "..\General\LinkedList.h"
#include "..\General\Create_object.h"

#define BUFFER 10000


struct internals{
	//each expression is the name of the regex associated with a tree
	struct LinkedList *Token_Str;
	//each expression is associated with one tree 
	struct LinkedList *Expression;	
};


//returns a linked list of RegEx names. Each name is associated with one of th trees
static struct LinkedList * Set_TokenStr(){
	struct LinkedList *_ptrNew  = (struct LinkedList *)Create_Object(LINKEDLIST);
	int i;

	_ptrNew = _ptrNew->init(_ptrNew, sizeof(char)*BUFFER);
	for (i =0; i< LEXDEF_COUNT;i++){
		_ptrNew->Add(lexDef_RegName[i], _ptrNew);
	}
	return _ptrNew;
}

//returns a linked list of RegExpressions. Each expressoin is associated with one of the trees
static struct LinkedList * Set_Expressions(struct RegDefs *__ptrInput){
	struct LinkedList *_ptrNew  = (struct LinkedList *)Create_Object(LINKEDLIST);
	int i;

	_ptrNew = _ptrNew->init(_ptrNew, sizeof(char)*BUFFER);
	for (i =0; i< LEXDEF_COUNT;i++){
		_ptrNew->Add(__ptrInput->GetExpr(lexDef_RegName[i], __ptrInput), _ptrNew);
	}
	return _ptrNew;
}


static void Print(struct LexDefs *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrName_Header;
	struct LinkedListNode *_ptrExpr_Header;

	_ptrName_Header = _ptrInternals->Token_Str->Head;
	_ptrExpr_Header = _ptrInternals->Expression->Head;
	printf("Printing LexDefs :\n");
	while (_ptrName_Header->Next !=NULL){
		printf("%-10s%-10s", (char *)_ptrName_Header->Value, (char *)_ptrExpr_Header->Value);
		_ptrName_Header =_ptrName_Header->Next;
		_ptrExpr_Header =_ptrExpr_Header->Next;
		printf("\n");
	}
	printf("\n");
}

//Generates the LexTrees
void init_LexTrees(struct LexDefs *__ptrInput){
	struct internals *_ptrInternals;
	struct LinkedListNode *_ptrExpression_Head;
	struct LexTree *_ptrNew;
	
	_ptrInternals= (struct internals *)__ptrInput->internals;
	_ptrExpression_Head = _ptrInternals->Expression->Head;	
	while (_ptrExpression_Head->Next!=NULL){
		_ptrNew = (struct LexTree *)Create_Object(LEXTREE);
		_ptrNew = _ptrNew->init(_ptrNew, (char *)_ptrExpression_Head->Value);
		__ptrInput->LexTrees->Add(_ptrNew, __ptrInput->LexTrees);
		_ptrExpression_Head = _ptrExpression_Head->Next;
	}
}

static struct LexDefs* init(struct LexDefs *__ptrInput, struct RegDefs *__ptrRegDefs){
	struct internals *_ptrInternals = (struct internals *)malloc(sizeof(struct internals));

	__ptrInput->internals = _ptrInternals;
	
	_ptrInternals->Token_Str = Set_TokenStr();
	_ptrInternals->Expression = Set_Expressions(__ptrRegDefs);
	__ptrInput->LexTrees  = (struct LinkedList *)Create_Object(LINKEDLIST);

	__ptrInput->LexTrees  = __ptrInput->LexTrees->init(__ptrInput->LexTrees, sizeof(struct LexTree));
	init_LexTrees(__ptrInput);
	return __ptrInput;
}

static void * c_ctor(){
	struct  LexDefs *_ptrOutput = (struct LexDefs *)malloc(sizeof(struct LexDefs));
	_ptrOutput->init = init;
	return _ptrOutput;
}

struct  Class_Generic _LEXDEFS = {c_ctor};

const void *LEXDEFS = &_LEXDEFS;