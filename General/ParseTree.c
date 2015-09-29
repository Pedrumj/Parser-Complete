#include <Windows.h>
#include "ParseTree.h"
#include <stdio.h>
#include "Class_Generic.h"

struct internals {
	size_t size;
};



struct ParseTreeNode *GetLastSib(struct ParseTreeNode * __Node){
	struct ParseTreeNode *_ptrOutput = __Node;
	while (_ptrOutput->Next !=NULL){
		_ptrOutput = _ptrOutput->Next;
	}
	return _ptrOutput;
}

struct ParseTreeNode *GetPrevious(struct ParseTreeNode*__Node){
	struct ParseTreeNode *_ptrOutput = __Node->Parent->FirstChild;
	while (_ptrOutput->Next !=__Node){
		_ptrOutput = _ptrOutput->Next;
	}
	return _ptrOutput;

}


struct ParseTreeNode * Add_Ch(struct ParseTree *__ParseTree, struct ParseTreeNode *__Node, void *__newValue){
	struct ParseTreeNode *_ptrTemp;
	struct ParseTreeNode *_ptrNew;
	struct internals *_ptrInternals = (struct internals *)__ParseTree->internals;

	_ptrNew = (struct ParseTreeNode*)malloc(sizeof(struct ParseTreeNode));
		
	_ptrNew->Value = (void*)malloc(sizeof(_ptrInternals->size));
	_ptrNew->FirstChild =NULL;
	_ptrNew->Next = NULL;
	memcpy(_ptrNew->Value, __newValue,_ptrInternals->size);

	if (__Node->FirstChild == NULL){
		__Node->FirstChild = _ptrNew;
		_ptrNew->Parent = __Node;
	}
	else{
		_ptrTemp = GetLastSib(__Node->FirstChild);
		_ptrTemp->Next = _ptrNew;
		_ptrNew->Parent =_ptrTemp->Parent;
	}
	return _ptrNew;
}

struct ParseTreeNode *AddFirst(struct ParseTree *__ParseTree, struct ParseTreeNode *__Node, void *__newValue){
	
	if (__Node->Parent == NULL){
		return __ParseTree->Add_Ch(__ParseTree, __Node, __newValue);
	}
	else{
		return NULL;
	}
}

struct ParseTreeNode * Add_PSh(struct ParseTree *__ParseTree,struct ParseTreeNode *__Node, void *__newValue){

	struct ParseTreeNode *_ptrTemp;
	struct ParseTreeNode *_ptrNew;
	struct internals *_ptrInternals = (struct internals *)__ParseTree->internals;

	_ptrNew = (struct ParseTreeNode*)malloc(sizeof(struct ParseTreeNode));
		
	_ptrNew->Value = (void*)malloc(sizeof(_ptrInternals->size));
	_ptrNew->FirstChild =__Node;
	_ptrNew->Next = NULL;
	memcpy(_ptrNew->Value, __newValue,_ptrInternals->size);
	_ptrNew->Parent = __Node->Parent;
	if (__Node->Parent->FirstChild == __Node){
		__Node->Parent->FirstChild =_ptrNew;
		_ptrNew->FirstChild = __Node;
		__Node->Parent = _ptrNew;
		_ptrTemp = __Node;
		while (_ptrTemp->Next !=NULL){
			_ptrTemp->Parent = __Node->Parent;
			_ptrTemp=_ptrTemp->Next;
		}
	}
	else{
		GetPrevious(__Node)->Next =_ptrNew;
		_ptrNew->FirstChild = __Node;
		__Node->Parent = _ptrNew;
	}

		return _ptrNew;
}

struct ParseTreeNode * Add_Root(struct ParseTree *__ParseTree,struct ParseTreeNode *__Node, void *__newValue){

	struct ParseTreeNode *_ptrTemp;
	struct ParseTreeNode *_ptrNew;
	struct internals *_ptrInternals = (struct internals *)__ParseTree->internals;

	_ptrNew = (struct ParseTreeNode*)malloc(sizeof(struct ParseTreeNode));
		
	_ptrNew->Value = (void*)malloc(sizeof(_ptrInternals->size));
	_ptrNew->FirstChild =__Node;
	_ptrNew->Next = NULL;
	_ptrNew->Parent = __Node->Parent;
	memcpy(_ptrNew->Value, __newValue,_ptrInternals->size);
	
	_ptrNew->FirstChild = __Node->Parent->FirstChild;
	__Node->Parent->FirstChild = _ptrNew;
	_ptrTemp =_ptrNew->FirstChild;
	while (_ptrTemp->Next !=NULL){
		_ptrTemp->Parent = _ptrNew;
	}
		return _ptrNew;

}

struct ParseTreeNode * Add_Next(struct ParseTree *__ParseTree,struct ParseTreeNode *__Node, void *__newValue){

	struct ParseTreeNode *_ptrTemp;
	struct ParseTreeNode *_ptrNew;
	struct internals *_ptrInternals = (struct internals *)__ParseTree->internals;

	
	_ptrTemp =AddFirst(__ParseTree, __Node, __newValue);
	if (_ptrTemp!=NULL){
		return _ptrTemp;
	}
	_ptrNew = (struct ParseTreeNode*)malloc(sizeof(struct ParseTreeNode));
		
	_ptrNew->Value = (void*)malloc(sizeof(_ptrInternals->size));
	_ptrNew->FirstChild =NULL;
	_ptrNew->Next = NULL;
	_ptrNew->Parent = __Node->Parent;
	memcpy(_ptrNew->Value, __newValue,_ptrInternals->size);
	__Node->Next = _ptrNew;
	return _ptrNew;

}


void PrintTreeAux(struct ParseTreeNode *__ptrHeader, int __Recurse, void (*Print)(void *__ptrValue)){
	struct ParseTreeNode *_ptrTemp;
	char *_strSlash = (char *)malloc(sizeof(char)*100);
	int i;
	_ptrTemp = __ptrHeader;
	for (i =0; i < __Recurse; i ++){
		_strSlash[i] ='\\';
	}
	_strSlash[__Recurse] = '\0';
	printf(_strSlash);
	Print(_ptrTemp->Value);
	//printf("%d\n",*(int *)_ptrTemp->Value);
	if (_ptrTemp->FirstChild!=NULL){
		__Recurse ++;
		PrintTreeAux(_ptrTemp->FirstChild, __Recurse, Print);
		__Recurse--;
	}
	if (_ptrTemp->Next!=NULL){
		
		PrintTreeAux(_ptrTemp->Next, __Recurse, Print);
	}
	
}


//prints the tree
static void PrintTree(struct ParseTree *__ptrHeader, void (*Print)(void *__ptrValue)){
	printf("Printing tree\n");
	PrintTreeAux(__ptrHeader->Head, 0, Print);
}

static struct ParseTree *init(struct ParseTree *__ParseTree, size_t __size){
	int _Value;
	struct internals *_ptrInternals  = (struct internals *)__ParseTree->internals;
	struct ParseTreeNode *_ptrNode= (struct ParseTreeNode *)malloc(sizeof(struct ParseTreeNode));
	_ptrNode->Value = (void *)malloc(sizeof(int));
	
	_Value=0;
	
	_ptrNode->Parent = NULL;
	_ptrNode->FirstChild = NULL;
	_ptrNode->Next = NULL;
	memcpy(_ptrNode->Value, &_Value, __size); 
	__ParseTree->Head = _ptrNode;
	_ptrInternals->size =__size;

	return __ParseTree;
}

int ChildCount(struct ParseTreeNode *__ptrNode){
	struct ParseTreeNode *_ptrChildNode;
	int _countProd;

	if (__ptrNode->FirstChild == NULL){
		return 0;
	}

	_ptrChildNode = __ptrNode->FirstChild;
	if (_ptrChildNode->Next ==NULL){
		return 1;
	}

	_countProd = 1;
	while (_ptrChildNode->Next !=NULL){
		_ptrChildNode = _ptrChildNode->Next;
		_countProd++;
	}

	return _countProd;

}

 static void * c_ctor(){
	struct  ParseTree *_ptrOutput = (struct ParseTree *)malloc(sizeof(struct ParseTree));
	struct internals *_ptrInternals = (struct internals *)malloc(sizeof(struct internals));

	_ptrOutput->internals = _ptrInternals;
	_ptrOutput->Add_Ch = Add_Ch;
	_ptrOutput->Add_PSh = Add_PSh;
	_ptrOutput->Add_Root = Add_Root;
	_ptrOutput->init = init;
	_ptrOutput->Add_Next = Add_Next;
	_ptrOutput->GetPrevious = GetPrevious;
	_ptrOutput->PrintTree = PrintTree;
	_ptrOutput->ChildCount = ChildCount;
	return _ptrOutput;
}



struct  Class_Generic _PARSETREE = {c_ctor};

const void *PARSETREE = &_PARSETREE;