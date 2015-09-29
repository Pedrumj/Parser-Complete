#include "Automaton.h"
#include "Grammar.h"
#include "Closure.h"
#include <Windows.h>

struct internals{
	struct Grammar *Grammar;
};



//Searches all closures in the automaton for one with a kernel matching the outgoing link. Returns null if 
//a match is not found
struct Closure *FindClosure(struct Automaton*__ptrInput, struct Link  *__ptrNewItem, 
	struct LinkedListNode *__ptrClosureLinks){
	struct LinkedListNode *_ptrClosures = (struct LinkedListNode *)__ptrInput->Closures->Head;
	struct Closure *_ptrClosure;
	struct Link *_ptrValue;

	//check current closure for any outoing links that have already matched a closure. This is 2 pervent creating
	//2 closures with the same state. If something is found in this loop the closure will have more than one kernel
	//item
	while (__ptrClosureLinks->Next !=NULL){
		_ptrValue = (struct Link *)__ptrClosureLinks->Value;
		if (_ptrValue->Token == __ptrNewItem->Token && _ptrValue->NextClosure !=NULL){
			return _ptrValue->NextClosure;
		}
		__ptrClosureLinks = __ptrClosureLinks->Next;	
	}

	//check other closures. 
	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		if (_ptrClosure->MatchKernelItem(_ptrClosure, __ptrNewItem->NewItem)==1){
			return _ptrClosure;
		}
		_ptrClosures= _ptrClosures->Next;
	}
	return NULL;
}

	
	

//Generate outgoing closure for Link
int GenClos_L(struct Automaton *__ptrInput, struct Link *__ptrLink, struct Closure *__ptrClosure, int *__state){
	struct internals * _ptrInternals = (struct internals *)__ptrInput->internals;
	int _flagChanged;
	struct Closure *_ptrNextClosure;
	struct Closure *_ptrNewClosure;
	_flagChanged = 0;

  
	if (__ptrLink->IsAccept ==0){
		_ptrNextClosure = FindClosure(__ptrInput, __ptrLink, __ptrClosure->Links->Head);
		if (_ptrNextClosure!=NULL){
			__ptrLink->NextClosure = _ptrNextClosure;
			_flagChanged += _ptrNextClosure->AddItemToClosure(_ptrNextClosure, __ptrLink->NewItem->intProduction,
				__ptrLink->NewItem->intDot, 1);
		}
		else{
			_ptrNewClosure= (struct Closure *)Create_Object(CLOSURE);
			_ptrNewClosure = _ptrNewClosure->init(_ptrNewClosure, _ptrInternals->Grammar, *__state);
			(*__state) ++;
			_flagChanged += _ptrNewClosure->AddItemToClosure(_ptrNewClosure, __ptrLink->NewItem->intProduction,
				__ptrLink->NewItem->intDot, 1);
			__ptrInput->Closures->Add(_ptrNewClosure, __ptrInput->Closures);
			__ptrLink->NextClosure = _ptrNewClosure;
		}
	}
				
//	__ptrInput->Print(__ptrInput);
	

return _flagChanged;
}


//Generates closures once for each closure in the automaton
int GenClos_E(struct Automaton *__ptrInput){
	struct internals * _ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures;
	int _flagChanged;
	struct Closure *_ptrClosure;
	struct Link *_ptrLink;
	struct LinkedListNode *_ptrLinkNode;
	int _state =1;
	_flagChanged = 1;

	
 	_flagChanged = 0;
	_ptrClosures = (struct LinkedListNode*)__ptrInput->Closures->Head;
	//repeat for each closure
	while (_ptrClosures->Next!=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		_ptrClosure->GenerateClosure(_ptrClosure);
		_ptrLinkNode = _ptrClosure->Links->Head;
		while (_ptrLinkNode->Next!=NULL){
			_ptrLink = (struct Link *)_ptrLinkNode->Value;
			 _flagChanged += GenClos_L(__ptrInput, _ptrLink, _ptrClosure, &_state);
			//_ptrClosure->Print(_ptrClosure);
			_ptrLinkNode = _ptrLinkNode->Next;
		}
		_ptrClosures = _ptrClosures->Next;
	}
return _flagChanged;
}


//Generates closures for the automaton. 
void GenerateClosures(struct Automaton *__ptrInput){
	struct internals * _ptrInternals = (struct internals *)__ptrInput->internals;
	int _flagChanged;
	struct Closure *_ptrClosure;

	_flagChanged = 1;

	//add first closure
    _ptrClosure = (struct Closure*)Create_Object(CLOSURE);
	_ptrClosure  = _ptrClosure->init(_ptrClosure, _ptrInternals->Grammar, 0);
	_ptrClosure->AddItemToClosure(_ptrClosure, 0, 0, 1);
	__ptrInput->Closures->Add(_ptrClosure, __ptrInput->Closures);
	//_ptrClosure->Print(_ptrClosure);
	//keep adding while changes are being made
	while (_flagChanged !=0){
 		_flagChanged =  GenClos_E(__ptrInput);
	//	__ptrInput->Print(__ptrInput);
	}
}

struct Automaton *init(struct Automaton *__ptrInput, struct Grammar* __ptrGrammar ){
	struct internals * _ptrInternals = (struct internals *)__ptrInput->internals;

	_ptrInternals->Grammar = __ptrGrammar;

	GenerateClosures(__ptrInput);
	return __ptrInput;
}

void Print(struct Automaton *__ptrInput){
	//struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = (struct LinkedListNode *)__ptrInput->Closures->Head;
	struct Closure *_ptrClosure;
	while (_ptrClosures->Next !=NULL){
		_ptrClosure=(struct Closure*)_ptrClosures->Value;
		_ptrClosure->Print(_ptrClosure);
		_ptrClosures = _ptrClosures->Next;
	}
}

 static void * c_ctor(){
	struct Automaton *_ptrOutput = (struct Automaton *)malloc(sizeof(struct Automaton));
	struct internals *_ptrInternals  = (struct internals *)malloc(sizeof(struct internals));
	struct LinkedList *_ptrClosures = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrClosures = _ptrClosures->init(_ptrClosures, sizeof(struct Closure));
	_ptrOutput->Closures = _ptrClosures;
	_ptrOutput->internals = _ptrInternals;
	_ptrOutput->init = init;
	_ptrOutput->Print =Print;
	return _ptrOutput;
}

struct  Class_Generic _AUTOMATON = {c_ctor};

const void *AUTOMATON = &_AUTOMATON;
