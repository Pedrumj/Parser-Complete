#include "Closure.h"
#include "Grammar.h"
#include <Windows.h>
#include <stdio.h>

 struct internals{
	const struct Grammar *  Grammar;

};

 void GenerateClosure(struct Closure *_ptrInput);

 int AddItemToClosure(struct Closure *_ptrInput, int __intProduction, int __intDot, int __IsKernel){
	struct ItemInClosure *_ptrNewItem = (struct ItemInClosure *)malloc(sizeof(struct ItemInClosure));
	//struct internals *_ptrInternals =(struct internals *)_ptrInput->internals;
	struct LinkedListNode *_ptrItemsHead = _ptrInput->Items->Head;
	struct ItemInClosure *_ptrValue;
	int _flagFound = 0;
	while (_ptrItemsHead->Next !=NULL){
		_ptrValue = (struct ItemInClosure *)_ptrItemsHead->Value;
	
		if (_ptrValue->intProduction==__intProduction && _ptrValue->intDot==__intDot){
			return 0;
		}
		else{
			_ptrItemsHead = _ptrItemsHead->Next;
		}
	}
	_ptrNewItem->intProduction = __intProduction;
	_ptrNewItem->intDot = __intDot;
	_ptrNewItem->IsKernel =1;
	_ptrInput->Items->Add(_ptrNewItem, _ptrInput->Items);
	//GenerateClosure(_ptrInput);
	return 1;
}



static struct Closure * init(struct Closure *_ptrInput, const struct Grammar * const __ptrGrammar, int __state){
	struct internals *_ptrInternals = (struct internals *)_ptrInput->internals;
	_ptrInternals->Grammar = __ptrGrammar;
	_ptrInput->Items = (struct LinkedList*)Create_Object(LINKEDLIST);
	_ptrInput->Items = _ptrInput->Items->init(_ptrInput->Items, sizeof(struct ItemInClosure));
	_ptrInput->Links = (struct LinkedList*)Create_Object(LINKEDLIST);
	_ptrInput->Links = _ptrInput->Links->init(_ptrInput->Links, sizeof(struct Link));
	_ptrInput->State = __state;

	return _ptrInput;
}


//add additional items to the closure associated with the input productions
int GenClos_I_Prods(struct Closure *_ptrInput, struct LinkedList *_ptrProductions){
	int _intChanged;
	struct internals *_ptrInternals =(struct internals *)_ptrInput->internals;
	struct LinkedListNode *_ptrHeadRows;

	_intChanged =0;
	
	_ptrHeadRows = _ptrProductions->Head;
	while (_ptrHeadRows->Next !=NULL){
		_intChanged+=AddItemToClosure(_ptrInput, *(int *)_ptrHeadRows->Value, 0, 0);
		_intChanged +=1;
		_ptrHeadRows = _ptrHeadRows->Next;	
	}
	
	return _intChanged;
}


//add additional items to the closure associated with the input item.
int GenClos_I(struct Closure *_ptrInput, struct ItemInClosure * __ptrItem){
	int _intChanged;
	int _intProduction;
	int _intDot;
	struct LinkedList *_ptrRows;
	struct internals *_ptrInternals =(struct internals *)_ptrInput->internals;
	struct LinkedListNode *_ptrHeadRows;
	int _ItemAfterDot;



	
	_intChanged =0;
	
	_intProduction = __ptrItem->intProduction;
	_intDot = __ptrItem->intDot;
	_ItemAfterDot = _ptrInternals->Grammar->GetItemAfterDot(_ptrInternals->Grammar, _intProduction, _intDot);
	if (_ptrInternals->Grammar->IsNonTerminal(_ptrInternals->Grammar, _ItemAfterDot)==1){
		_ptrRows = _ptrInternals->Grammar->GetRows(_ptrInternals->Grammar, _ItemAfterDot);
		_ptrHeadRows = _ptrRows->Head;
		while (_ptrHeadRows->Next !=NULL){
			_intChanged+=AddItemToClosure(_ptrInput, *(int *)_ptrHeadRows->Value, 0, 0);
			_intChanged +=1;
			_ptrHeadRows = _ptrHeadRows->Next;	
		}
	}


	
	return _intChanged;
}

int FindLink(struct LinkedListNode *__ptrLinks, struct Link *__ptrNewLink){
	struct Link *_ptrValue;
	while (__ptrLinks->Next !=NULL){
		_ptrValue = (struct Link *)__ptrLinks->Value;
		if (_ptrValue->IsAccept ==1 && __ptrNewLink->IsAccept ==1){
			return 1;
		}
		else if (_ptrValue->NewItem !=NULL){
			if (_ptrValue->NewItem->intDot == __ptrNewLink->NewItem->intDot && 
				_ptrValue->NewItem->intProduction == __ptrNewLink->NewItem->intProduction){
				return 1;
			}
		}
		__ptrLinks =__ptrLinks->Next;
	}
	return 0;
}


void OutgoingLinks(struct Closure *_ptrInput){
	struct internals *_ptrInternals =(struct internals *)_ptrInput->internals;
	struct LinkedListNode *_ptrItemsHead = _ptrInput->Items->Head;
	struct Grammar *_ptrGrammar = (struct Grammar *)_ptrInternals->Grammar;
	struct ItemInClosure *_ptrItemInClosure;
	struct Link *_ptrNewLink;
	struct ItemInClosure *_ptrNewItem;
	int _intToken;
	while (_ptrItemsHead->Next !=NULL){
		_ptrItemInClosure = (struct ItemInClosure *)_ptrItemsHead->Value;
		_intToken = _ptrGrammar->GetItemAfterDot(_ptrGrammar, _ptrItemInClosure->intProduction, _ptrItemInClosure->intDot);
		//dot was not at the end of the production
		if (_intToken!=-1){
			_ptrNewLink =(struct Link *)malloc(sizeof(struct Link *));
			_ptrNewItem = (struct ItemInClosure *)malloc(sizeof(struct ItemInClosure *));
			_ptrNewLink->IsAccept = 0;
			_ptrNewLink->Token = _intToken;
			_ptrNewItem->intDot = _ptrItemInClosure->intDot+1;
			_ptrNewItem->intProduction =_ptrItemInClosure->intProduction;
			_ptrNewLink->NewItem = _ptrNewItem;
			_ptrNewLink->NextClosure = NULL;
			if (FindLink(_ptrInput->Links->Head, _ptrNewLink)==0){
				_ptrInput->Links->Add(_ptrNewLink, _ptrInput->Links);
			}
			
		}
		else if (_intToken ==-1 && _ptrItemInClosure->intProduction==0){
	
			_ptrNewLink =(struct Link *)malloc(sizeof(struct Link *));
			_ptrNewLink->IsAccept = 1;
			_ptrNewLink->Token = -1;
			_ptrNewLink->NewItem = NULL;
			if (FindLink(_ptrInput->Links->Head, _ptrNewLink)==0){
				_ptrInput->Links->Add(_ptrNewLink, _ptrInput->Links);
			}
		}
		_ptrItemsHead= _ptrItemsHead->Next;	


	}
}


//loop through all productions in the input closure, and add addition items
static int GenClos_E(struct Closure *_ptrInput){
	int _intChanged;
	int _intProduction;
	int _intDot;
	struct LinkedList *_ptrRows;
	struct internals *_ptrInternals =(struct internals *)_ptrInput->internals;
	struct LinkedListNode *_ptrHeadRows;
	int _ItemAfterDot;
	struct LinkedListNode *_ptrHeadClosureItems; 
	struct ItemInClosure *_ptrItemInClosure;


	
	_intChanged =0;
	
	_ptrHeadClosureItems = _ptrInput->Items->Head;
	while (_ptrHeadClosureItems->Next !=NULL){
		_ptrItemInClosure = (struct ItemInClosure *)_ptrHeadClosureItems->Value;
		_intProduction = _ptrItemInClosure->intProduction;
		_intDot = _ptrItemInClosure->intDot;
		_ItemAfterDot = _ptrInternals->Grammar->GetItemAfterDot(_ptrInternals->Grammar, _intProduction, _intDot);
		if (_ptrInternals->Grammar->IsNonTerminal(_ptrInternals->Grammar, _ItemAfterDot)==1){
			_ptrRows = _ptrInternals->Grammar->GetRows(_ptrInternals->Grammar, _ItemAfterDot);
			_ptrHeadRows = _ptrRows->Head;
			while (_ptrHeadRows->Next !=NULL){
				_intChanged+=AddItemToClosure(_ptrInput, *(int *)_ptrHeadRows->Value, 0, 0);
			
				_ptrHeadRows = _ptrHeadRows->Next;	
			}
		}
		_ptrHeadClosureItems =_ptrHeadClosureItems->Next;
	}



	OutgoingLinks(_ptrInput);
//		_ptrInput->Print(_ptrInput);
	return _intChanged;
}



//generate all production for the input closure until nothing else can be added
void GenerateClosure(struct Closure *_ptrInput){
	int _intChanged;


	struct internals *_ptrInternals =(struct internals *)_ptrInput->internals;

	
	_intChanged =1;
	while (_intChanged !=0){
		_intChanged =GenClos_E(_ptrInput);
	}

	OutgoingLinks(_ptrInput);
	

}





static void Print(struct Closure *_ptrInput){
	//struct internals *_ptrInternals = (struct internals *)_ptrInput->internals;
	struct LinkedListNode *_ptrHeader = _ptrInput->Items->Head;
	struct ItemInClosure *_ptrItem;
	struct Link *_ptrLink;
	struct LinkedListNode *_ptrLinkHeader = _ptrInput->Links->Head;;
	printf("Printing Closure Items\n");
	printf("Closure state %d: \n", _ptrInput->State);
	printf ("%-15s", "Production");
	printf ("%-15s", "Dot");
		printf("\n");
	while (_ptrHeader->Next !=NULL){
		_ptrItem = (struct ItemInClosure *)_ptrHeader->Value;
		printf("%-15d", _ptrItem->intProduction);
		printf("%-15d",  _ptrItem->intDot);
			printf("\n");
		_ptrHeader = _ptrHeader->Next;	
	}
	printf("\n");
	

	printf("Printing Closure Links\n");
	printf ("%-15s", "Outgoing");
	printf ("%-15s", "ToAccept");
	printf("\n");
	while (_ptrLinkHeader->Next !=NULL){
		_ptrLink = (struct Link *)_ptrLinkHeader->Value;
		printf("%-15d", _ptrLink->Token);
		printf("%-15d",  _ptrLink->IsAccept);
			printf("\n");
		_ptrLinkHeader = _ptrLinkHeader->Next;	
	}

}

int MatchKernelItem(struct Closure *__ptrInput, struct ItemInClosure *__ptrItem2Find){
	//struct internals *_ptrInternals =(struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrItems = (struct LinkedListNode *)__ptrInput->Items->Head;
	struct ItemInClosure *_ptrItem;
	while (_ptrItems->Next !=NULL){
		_ptrItem  = (struct ItemInClosure *)_ptrItems->Value;
		if (_ptrItem->IsKernel ==1 && _ptrItem->intDot==__ptrItem2Find->intDot &&
			_ptrItem->intProduction ==__ptrItem2Find->intProduction){
			return 1;
		}
		else{
			_ptrItems = _ptrItems->Next;
		}
	}
	return 0;
}
struct LinkedList * ItemsDotEnd(struct Closure *__ptrInput){
	struct LinkedList *_ptrOutput = (struct LinkedList *)Create_Object(LINKEDLIST);
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct ItemInClosure *_ptrValue;

	int _itemAfterDot;


	_ptrOutput = _ptrOutput->init(_ptrOutput, sizeof(struct ItemInClosure));
	while (__ptrInput->Items->Head->Next!=NULL){
		_ptrValue=  (struct ItemInClosure *)__ptrInput->Items->Head->Value;
		_itemAfterDot = _ptrInternals->Grammar->GetItemAfterDot(_ptrInternals->Grammar, _ptrValue->intProduction, 
			_ptrValue->intDot);
		if (_itemAfterDot ==-1){
			_ptrOutput->Add(_ptrValue, _ptrOutput);
		}
	}
	return _ptrOutput;
}
 static void * c_ctor(){
	struct Closure *_ptrOutput = (struct Closure *)malloc(sizeof(struct Closure));
	_ptrOutput->internals = (struct internals *)malloc(sizeof(struct internals));
	_ptrOutput->init = init;
	_ptrOutput->Print = Print;
	_ptrOutput->MatchKernelItem = MatchKernelItem;
	_ptrOutput->AddItemToClosure = AddItemToClosure;
	_ptrOutput->ItemsDotEnd = ItemsDotEnd;
		_ptrOutput->GenerateClosure = GenerateClosure;
	return _ptrOutput;
}

struct  Class_Generic _CLOSURE = {c_ctor};

const void *CLOSURE = &_CLOSURE;