#include <Windows.h>
#include "Grammar.h"
#include <stdio.h>
#include "First.h"


struct internals{
	int countRows;
	int countNonterminals;
	int countTerminals;
	struct Grammar *Grammar;
};



static void Print(struct First *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrPrFirst_Node;
	int i;
	printf("Printing First");
	printf("\n");
	for ( i = 0; i< _ptrInternals->countRows;i++){
		_ptrPrFirst_Node = __ptrInput->arrFirst[i]->Head;
		while (_ptrPrFirst_Node->Next !=NULL){
			 printf("%-4d", *(int *)_ptrPrFirst_Node->Value);
			 _ptrPrFirst_Node = _ptrPrFirst_Node->Next;
		}
		printf("\n");
	}
}


int CompareFirst(void *__ptr1, void *__ptr2){
	int _val1 = *(int *)__ptr1;
	int _val2 = *(int *)__ptr2;
	if (_val1 == _val2){
		return 1;
	}
	else{
		return 0;
	}
}


//Checks if input is terminal and adds to First
int Add_Terminal(int __intProduction, struct internals *__ptrInternals, struct LinkedList *__ptrFirst){
	if (__intProduction >= __ptrInternals->countNonterminals){
		return __ptrFirst->AddUnique(&__intProduction,__ptrFirst, CompareFirst);
	}
	return 0;
}


//Adds epslion if the end of the producion has been reached
int AddPrFirst_Eps(int __flagEps, struct LinkedList * __ptrPrFirst, struct First *__ptrInput){
	int _flagChanged =0;
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	int _eps = _ptrInternals->countNonterminals + _ptrInternals->countTerminals;
	if (__flagEps ==1){
		_flagChanged +=__ptrPrFirst->AddUnique(&_eps, __ptrPrFirst, CompareFirst);
	}
	return _flagChanged;
}

//Adds endmarker if this is the first production
int AddPrFirst_EM(int __intProduction, struct LinkedList * __ptrPrFirst, struct First *__ptrInput){
	int _flagChanged =0;
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	int _endMarker = _ptrInternals->countNonterminals + _ptrInternals->countTerminals +1;
	if (__intProduction ==0){
		_flagChanged +=__ptrPrFirst->AddUnique(&_endMarker, __ptrPrFirst, CompareFirst);
	}
	return _flagChanged;
}

//Adds terminal to the FIRST set of the input production
int AddPrFirst_ItTer(int __ptrItem, struct LinkedList * __ptrPrFirst, struct First *__ptrInput, int * __flagEpsilon, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	*__flagEpsilon = __ptrGrammar->IsEpsilon(__ptrItem, __ptrGrammar);
	if (__ptrGrammar->IsTerminal(__ptrGrammar, __ptrItem) ==1){
		return	__ptrPrFirst->AddUnique(&__ptrItem,__ptrPrFirst, CompareFirst);
	}
	return 0;
}

//while processing the tokens in the production a nonterminal was found. This function adds the FIRST values for this
//nonterminal associated with the input production to the main production.
int AddPrFirst_ItNTer_Single(int __ptrProduction, struct LinkedList * __ptrPrFirst, struct First *__ptrInput, int *__flagEpsilon, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
 	struct LinkedListNode *_ptrPrFirst_NTer_Node = __ptrInput->arrFirst[__ptrProduction]->Head;
	int _flagChanged = 0;
	while(_ptrPrFirst_NTer_Node->Next !=NULL){
		*__flagEpsilon+=__ptrGrammar->IsEpsilon(*(int *)_ptrPrFirst_NTer_Node->Value, __ptrGrammar);
		_flagChanged=__ptrPrFirst->AddUnique(_ptrPrFirst_NTer_Node->Value, __ptrPrFirst, CompareFirst);
		_ptrPrFirst_NTer_Node=_ptrPrFirst_NTer_Node->Next;
	}
	return _flagChanged;
}

//while processing the tokens in the production a nonterminal was found. This function adds all the FIRST values for this
//nonterminal to the production.
int AddPrFirst_ItNTer(int __ptrNonterminal, struct LinkedList * __ptrPrFirst, struct First *__ptrInput, int *__flagEpsilon, struct Grammar *__ptrGrammar){
	int _flagChanged =0;
	int i;
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	for (i =0; i<_ptrInternals->countRows;i++){
		if (__ptrGrammar->Rows[i] ==__ptrNonterminal){
			_flagChanged+=AddPrFirst_ItNTer_Single(i, __ptrPrFirst, __ptrInput, __flagEpsilon, __ptrGrammar);
		}
	}
	return _flagChanged;
}


//Adds the tokens to the FIRST array for the input production
int AdPrFirst(int __intProduction,  struct First *__ptrInput, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrProductionItems;
	int _flagEpsInFirst;
	int _PrItem;
	int _flagChanged =0;
	int _flagTerminal;

	_flagEpsInFirst = 1;
	_ptrProductionItems =__ptrGrammar->GetProduction(__ptrGrammar, __intProduction)->Head;
	while (_flagEpsInFirst !=0 && _ptrProductionItems->Next !=NULL){
		_flagEpsInFirst = 0;
		_flagTerminal =0;
		_PrItem = *(int *)_ptrProductionItems->Value;
		
		if (__ptrGrammar->IsNonTerminal(__ptrGrammar, _PrItem)==1){
			//token is nonterminal
			_flagTerminal=AddPrFirst_ItNTer(_PrItem, __ptrInput->arrFirst[__intProduction], __ptrInput, &_flagEpsInFirst, __ptrGrammar);
		}
		else{
			//token is termina 
			_flagTerminal=AddPrFirst_ItTer(_PrItem, __ptrInput->arrFirst[__intProduction], __ptrInput, &_flagEpsInFirst, __ptrGrammar);
		}
		_flagChanged +=_flagTerminal;
		_ptrProductionItems=_ptrProductionItems->Next;
	}
		
	_flagChanged += AddPrFirst_EM(__intProduction, __ptrInput->arrFirst[__intProduction], __ptrInput);
	_flagChanged += AddPrFirst_Eps(_flagEpsInFirst, __ptrInput->arrFirst[__intProduction], __ptrInput);
	return _flagChanged;
}


//Adds the FIRST tokens to the FIRST array for each production.
static int AddFirst(struct First *__ptrInput, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	int i;
	int _flagChanged =0;
	for ( i =0; i < _ptrInternals->countRows;i++){
		_flagChanged+=AdPrFirst(i, __ptrInput, __ptrGrammar);
	}
	return _flagChanged;
}


//allocates memory for the FIRST array
static void InitFirstArray(struct First *__ptrInput, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	int i;
	__ptrInput->arrFirst = (struct LinkedList **)malloc(sizeof(struct LinkedList *)*_ptrInternals->countRows);
	for ( i =0;i < _ptrInternals->countRows;i++){
		__ptrInput->arrFirst[i] = (struct LinkedList *)Create_Object(LINKEDLIST);
		__ptrInput->arrFirst[i] = __ptrInput->arrFirst[i]->init(__ptrInput->arrFirst[i], sizeof(int)); 
	} 
}



//Generates the FIRST array
void GenerateFirst(struct First *__ptrInput, struct Grammar *__ptrGrammar){
		struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
		InitFirstArray(__ptrInput, __ptrGrammar);
		while (AddFirst(__ptrInput, __ptrGrammar)!=0){
		}

}

void Add_NTs_First(struct LinkedList *__ptrInput, int _production, struct First *__ptrFirst){
	struct LinkedListNode *_ptrFirstHead;

	_ptrFirstHead = __ptrFirst->arrFirst[_production]->Head;
	while (_ptrFirstHead->Next !=NULL){
		__ptrInput->AddUnique(_ptrFirstHead->Value, __ptrInput, CompareFirst);
		_ptrFirstHead = _ptrFirstHead->Next;
	}
}
struct LinkedList *NT_First(int __nonTerminal, struct First *__ptrFirst){
	struct LinkedList *_ptrOutput;
	struct LinkedList *_ptrRows;
	struct internals *_ptrInternals;
	struct Grammar *_ptrGrammar;
	struct LinkedListNode *_ptrRowsHead;
	int _production;

	_ptrOutput = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrOutput = _ptrOutput->init(_ptrOutput, sizeof(int));
	_ptrInternals  = (struct internals *)__ptrFirst->internals;
	_ptrGrammar = _ptrInternals->Grammar;
	_ptrRows = _ptrGrammar->GetRows(_ptrGrammar, __nonTerminal);


	_ptrRowsHead = _ptrRows->Head;
	while (_ptrRowsHead->Next!=NULL){
		_production  = *(int *)_ptrRowsHead->Value;
		Add_NTs_First(_ptrOutput, _production, __ptrFirst);
		_ptrRowsHead = _ptrRowsHead->Next;
	}
	return _ptrOutput;
	
}


static struct First * init(struct First *__ptrFirst, struct Grammar *__ptrGrammar, int __countRows, int __countNonterminals, 
	int __countTerminals){
	
	struct internals *_ptrInternals ;
	
	_ptrInternals =	(struct internals *)__ptrFirst->internals;

	_ptrInternals->countNonterminals = __countNonterminals;
	_ptrInternals->countTerminals = __countTerminals;
	_ptrInternals->countRows = __countRows;
	_ptrInternals->Grammar = __ptrGrammar;
	GenerateFirst(__ptrFirst, __ptrGrammar);
	
	return __ptrFirst;
}







static void *c_ctor(){
	struct First *_ptrFirst = (struct First *)malloc(sizeof(struct First));
	_ptrFirst->internals = (struct internals *)malloc(sizeof(struct internals *));
	//_ptrFirst->GetFirst = GetFirst;
	_ptrFirst->init = init;
	_ptrFirst->Print =  Print;
	_ptrFirst->NT_First = NT_First;
	return _ptrFirst;
}
struct Class_Generic _FIRST = {c_ctor};
const void *FIRST = &_FIRST;
