#include <Windows.h>
#include "Grammar.h"
#include <stdio.h>
#include "Follow.h"
#include "First.h"


struct internals{
	//struct Grammar *Grammar;
	int countRows;
	int countNonterminals;
	int countTerminals;
};


int CompareFollow(void *__ptr1, void *__ptr2){
	int _val1 = *(int *)__ptr1;
	int _val2 = *(int *)__ptr2;
	if (_val1 == _val2){
		return 1;
	}
	else{
		return 0;
	}
}

static void Print(struct Follow *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrPrFollow_Node;
	int i;
	printf("Printing Follow");
	printf("\n");
	for ( i = 0; i< _ptrInternals->countNonterminals;i++){
		_ptrPrFollow_Node = __ptrInput->arrFollow[i]->Head;
		while (_ptrPrFollow_Node->Next !=NULL){
			 printf("%-4d", *(int *)_ptrPrFollow_Node->Value);
			 _ptrPrFollow_Node = _ptrPrFollow_Node->Next;
		}
		printf("\n");
	}
}


//Adds the items in the FOLLOW set of the production __intProduction, to the FOLLOW set of the nonterminal __intNonterminal
int AdFol_PrFollow(int __intNonterminal, int __intProduction, struct Follow *__ptrFollow, struct Grammar *__ptrGrammar){
	int _intNonTerminal_Pr = __ptrGrammar->Rows[__intProduction];
	struct LinkedListNode *_ptrPr_First = __ptrFollow->arrFollow[_intNonTerminal_Pr]->Head;	
	int _flagChanged =0;
	while (_ptrPr_First->Next !=NULL){
		_flagChanged += __ptrFollow->arrFollow[__intNonterminal]->AddUnique(_ptrPr_First->Value, __ptrFollow->arrFollow[__intNonterminal], 
			CompareFollow);
		_ptrPr_First =_ptrPr_First->Next;
	}
	return _flagChanged;
}


//Adds the items in the FOLLOW set of the __intNext nonterminal to the follow set of the __intNonterminal'th nontemrinal
int AdFirst_ItNx_NTer(int __intNonterminal, int __intNextNonTerminal, struct Follow *__ptrFollow, struct Grammar *__ptrGrammar, 
					int *__flagEpsilong){
	struct First *__ptrFirst;
	struct LinkedListNode *_ptrFirstNode;
	int _flagChanged =0;
	int _flagEpsilon = 0;
	int _intFollowValue;
	__ptrFirst = __ptrGrammar->First;
	 _ptrFirstNode= __ptrFirst->NT_First(__intNextNonTerminal, __ptrFirst)->Head;
	while (_ptrFirstNode->Next !=NULL){
		_intFollowValue = *(int *)_ptrFirstNode->Value;
		_flagEpsilon += __ptrGrammar->IsEpsilon(_intFollowValue, __ptrGrammar);
		_flagChanged += __ptrFollow->arrFollow[__intNonterminal]->AddUnique(&_intFollowValue,__ptrFollow->arrFollow[__intNonterminal], 
			CompareFollow);
		_ptrFirstNode = _ptrFirstNode->Next;
	}
	(*__flagEpsilong) = _flagEpsilon;
	return _flagChanged;			
	
}


//Adds the items in the First set of the __intNext item to the follow set of the __intNonterminal'th nontemrinal
int AdFirst_ItNx(int __intNonterminal, int __intNext, struct Follow *__ptrFollow, struct Grammar *__ptrGrammar, 
			   int __intProduction){
	int _flagChanged =0;
	int _flagEpsilon;
	if (__ptrGrammar->IsTerminal(__ptrGrammar, __intNext) ==1){
		_flagChanged+= __ptrFollow->arrFollow[__intNonterminal]->AddUnique(&__intNext,
			__ptrFollow->arrFollow[__intNonterminal], CompareFollow);
	}
	else{
		_flagChanged +=AdFirst_ItNx_NTer(__intNonterminal, __intNext, __ptrFollow, __ptrGrammar, &_flagEpsilon);
		if (_flagEpsilon !=0){
			_flagChanged+= AdFol_PrFollow(__intNonterminal, __intProduction, __ptrFollow, __ptrGrammar);
		}
				
	}
	return _flagChanged;
}

//Loops through the tokens in the __intProduction'th production until the nontemrinal __intNonterminal is found. it then 
//Adds all the items in the FOLLOW set of the following item to the FOLLOW set of the __intNonterminal'th nonterminal.
int AdFol_Pr(int __intProduction, int __intNonterminal, struct Follow *__ptrFollow, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternal = (struct internals *)__ptrFollow->internals;
	struct LinkedListNode *_ptrPr_Node = __ptrGrammar->GetProduction(__ptrGrammar, __intProduction)->Head;
	int _intPrIt_Value;
	int _intPrIt_Value_Next;
	int _flagChanged =0;
	while (_ptrPr_Node->Next !=NULL){
		_intPrIt_Value = *(int *)_ptrPr_Node->Value;
		if (_intPrIt_Value == __intNonterminal){
			if (_ptrPr_Node->Next->Next!=NULL){
				
				_intPrIt_Value_Next =*(int *) _ptrPr_Node->Next->Value;
				_flagChanged += AdFirst_ItNx(__intNonterminal, _intPrIt_Value_Next, __ptrFollow, __ptrGrammar, 
					__intProduction);
			}
			else{
				_flagChanged+=AdFol_PrFollow(__intNonterminal, __intProduction, __ptrFollow, __ptrGrammar);
			}
		}
		_ptrPr_Node= _ptrPr_Node->Next;
	}
	return _flagChanged;

}


//Adds the endmarker to the first nonterminal
int AddEm_S(struct Follow *__ptrInput, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	int _flagChanged =0;
	int _intEm =_ptrInternals->countNonterminals +_ptrInternals->countTerminals +1;
	_flagChanged +=__ptrInput->arrFollow[0]->AddUnique(&_intEm, __ptrInput->arrFollow[0], CompareFollow);
	return _flagChanged;
}

//Adds the follow valuues to the sets for each token in each production
int AddFollow(struct Follow *__ptrInput, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	int i;
	int _flagChanged =0;
	int j;
	for ( i =0; i < _ptrInternals->countNonterminals;i++){
		for ( j =0; j <_ptrInternals->countRows;j++){
			_flagChanged+=AdFol_Pr(j, i,__ptrInput,  __ptrGrammar);
		}
	}

	return _flagChanged;
}


//Allocates memory for the FOLLOW array
void InitFirstArray(struct Follow *__ptrInput, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	int i;
	__ptrInput->arrFollow = (struct LinkedList **)malloc(sizeof(struct LinkedList *)*_ptrInternals->countNonterminals);
	for ( i =0;i < _ptrInternals->countNonterminals;i++){
		__ptrInput->arrFollow[i] = (struct LinkedList *)Create_Object(LINKEDLIST);
		__ptrInput->arrFollow[i] = __ptrInput->arrFollow[i]->init(__ptrInput->arrFollow[i], sizeof(int)); 
	} 
}



//Generates the FOLLOW sets for the nontemrinals
void GenerateFollow(struct Follow *__ptrInput, struct Grammar *__ptrGrammar){
		struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
		InitFirstArray(__ptrInput, __ptrGrammar);
		AddEm_S(__ptrInput,__ptrGrammar);
		while (AddFollow(__ptrInput, __ptrGrammar)!=0){

		}

}

static struct Follow * init(struct Follow *__ptrFollow, struct Grammar *__ptrGrammar, int __countRows, int __countNonterminals, 
	int __countTerminals){
	struct internals *_ptrInternals ;
	
	_ptrInternals =	(struct internals *)__ptrFollow->internals;
	_ptrInternals->countNonterminals = __countNonterminals;
	_ptrInternals->countTerminals = __countTerminals;
	_ptrInternals->countRows = __countRows;
	
	GenerateFollow(__ptrFollow, __ptrGrammar);

	return __ptrFollow;
}


static void *c_ctor(){
	struct Follow *_ptrFollow = (struct Follow *)malloc(sizeof(struct Follow));
	_ptrFollow->internals = (struct internals *)malloc(sizeof(struct internals *));
	_ptrFollow->init = init;
	_ptrFollow->Print = Print;
	return _ptrFollow;
}
struct Class_Generic _FOLLOW = {c_ctor};
const void *FOLLOW = &_FOLLOW;
