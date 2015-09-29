#include <Windows.h>
#include "Grammar.h"
#include "Automaton.h"
#include "ParseTable.h"
#include "Closure.h"
#include <stdio.h>
#include "Follow.h"


 void PrintMatrix(int **__ptrInput, int __countRows, int __countColumns);
 void GenerateActionTable(struct ParseTable *__ptrInput);
 int GetClosureState(struct Closure* __ptrOutgoing, struct LinkedListNode *__ptrClosures);
 int IsTerminal(int __Token, int __NonTerminalCount, int __TerminalCount);
 int **InitiateMatrix( int __rowCount, int __colCount);
 int GetStateCount(struct LinkedListNode *__ptrClosures);


static struct internals{
	struct Grammar *Grammar;
	struct  Automaton *Automaton;
	int **Action;
	int **Reduce;
	int **Accept;
	int **Goto;
	int RowCount;
	int NonTerminalCount;
	int TerminalCount;
	int StateCount;
};



 void AddReduce(struct ParseTable *__ptrInput, int __intState, int __intProduction){
	struct internals *_ptrInternals  = (struct internals *)__ptrInput->internals;
	int _intNonterminal;
	struct LinkedListNode *_ptrFollowA; 
	int _intFolValue;
	int _rIndex;
	int _cIndex;

	_intNonterminal = _ptrInternals->Grammar->Rows[__intProduction];
	_ptrFollowA = _ptrInternals->Grammar->Follow->arrFollow[_intNonterminal]->Head;

	while (_ptrFollowA->Next !=NULL){
		_intFolValue= *(int *)_ptrFollowA->Value;
		if (_ptrInternals->Grammar->IsEndMarker(_intFolValue, _ptrInternals->Grammar) ==1){
			_cIndex=_intFolValue-_ptrInternals->NonTerminalCount-1;
		}
		else{
			_cIndex=_intFolValue-_ptrInternals->NonTerminalCount;
		}

		_rIndex=__intState;
		if (_rIndex ==39){
			_rIndex =39;
		}
		if (_ptrInternals->Reduce[_rIndex][_cIndex] !=-1){
				_ptrInternals->Grammar->Follow->Print(_ptrInternals->Grammar->Follow);
				_ptrInternals->Grammar->First->Print(_ptrInternals->Grammar->First);
			printf("REDUCE/REDUCE ERROR\n");
			exit(0);
		}
		else
		{
			_ptrInternals->Reduce[_rIndex][_cIndex]=__intProduction;
		}
		

		_ptrFollowA = _ptrFollowA->Next;
	}
}
int IsDotEnd(struct ItemInClosure *__ptrClItem, struct Grammar *__ptrGrammar){
	if (__ptrGrammar->GetItemAfterDot(__ptrGrammar, __ptrClItem->intProduction, __ptrClItem->intDot)==-1){
		return 1;
	}
	else{
		return 0;
	}
}


//add reduces associated with the input closure
void RedCl(struct Closure *__ptrClosure, struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClItems;
	struct ItemInClosure *_ptrClItem;
	_ptrClItems = __ptrClosure->Items->Head;
	//for each item in closure
	while (_ptrClItems->Next !=NULL){
		_ptrClItem = (struct ItemInClosure *)_ptrClItems->Value;
		//if the dot is at the end of the item
		if(IsDotEnd(_ptrClItem, _ptrInternals->Grammar) ==1){
			//if item is not the first production. whic would be accept in this case not reduce
			if (_ptrClItem->intProduction !=0){
				AddReduce(__ptrInput, __ptrClosure->State, _ptrClItem->intProduction);
			}
		}
		_ptrClItems = _ptrClItems->Next;
	}
}

//generate reduce table
void GenerateReduceTable(struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = _ptrInternals->Automaton->Closures->Head;
	struct Closure *_ptrClosure;

	_ptrInternals->Reduce = InitiateMatrix(_ptrInternals->StateCount, _ptrInternals->TerminalCount + 1);
	//for each closure in automaton
	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		RedCl(_ptrClosure, __ptrInput);
		_ptrClosures = _ptrClosures->Next;
	}

}


void GenerateAcceptTable(struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = _ptrInternals->Automaton->Closures->Head;
	struct Closure *_ptrClosure;
	struct LinkedListNode *_ptrLinks;
	struct Link *_ptrLink;
	int _rIndex;
	int _cIndex;


	_ptrInternals->Accept = InitiateMatrix(_ptrInternals->StateCount, _ptrInternals->TerminalCount + 1);

	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		_ptrLinks = _ptrClosure->Links->Head;
		while (_ptrLinks->Next !=NULL){
			_ptrLink = (struct Link *)_ptrLinks->Value;
			if (_ptrLink->IsAccept ==1){
				_rIndex=_ptrClosure->State;
				_cIndex=_ptrInternals->Grammar->End_Marker-_ptrInternals->NonTerminalCount-1;
				_ptrInternals->Accept[_rIndex][_cIndex]=1;
				if (_rIndex ==82){
					_rIndex = 82;
				}
			}			
			_ptrLinks = _ptrLinks->Next;
		}
		_ptrClosures = _ptrClosures->Next;

	}
}


void GenerateGotoTable(struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = _ptrInternals->Automaton->Closures->Head;
	struct Closure *_ptrClosure;
	struct LinkedListNode *_ptrLinks;
	struct Link *_ptrLink;
	int _state =0;
	_ptrInternals->Goto = InitiateMatrix(_ptrInternals->StateCount, _ptrInternals->NonTerminalCount -1);
	
	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		_ptrLinks = _ptrClosure->Links->Head;
		while (_ptrLinks->Next !=NULL){
			_ptrLink = (struct Link *)_ptrLinks->Value;
			if (_ptrInternals->Grammar->IsNonTerminal(_ptrInternals->Grammar, _ptrLink->Token)==1){

				_ptrInternals->Goto[_ptrClosure->State][_ptrLink->Token-1]=_ptrLink->NextClosure->State;
				if (_ptrClosure->State ==82){
					_ptrClosure->State = 82;
				}
			}
			_ptrLinks = _ptrLinks->Next;
		}
		_ptrClosures = _ptrClosures->Next;
	
	}
}

static void CheckSR_Err(struct ParseTable *__ptrInput){
	struct internals *_ptrInternals; 
	int i;
	int j;

	_ptrInternals = (struct internals *)__ptrInput->internals;
	for (i = 0; i<_ptrInternals->StateCount; i++){
		for (j = 0; j<_ptrInternals->TerminalCount +1;j++){
			if (_ptrInternals->Action[i][j] >= 0 && _ptrInternals->Reduce[i][j] >= 0){
				printf("SHIFT REDUCE ERROR\n");
				exit(0);
			}
		}
	}
}

static struct ParseTable * init(struct ParseTable *__ptrInput, int **__Grammar, int *__Rows, int __countRows, 
	int __countNonterminals, int __countTerminals){
	struct Grammar * _ptrGrammar = (struct Grammar *)Create_Object(GRAMMAR);
	struct Automaton *_ptrAutomaton = (struct Automaton *)Create_Object(AUTOMATON);
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	_ptrGrammar = _ptrGrammar->init(_ptrGrammar, __Grammar, __Rows, __countRows, __countNonterminals, __countTerminals);


	_ptrAutomaton = _ptrAutomaton->init(_ptrAutomaton, _ptrGrammar);
	
	_ptrInternals->Automaton = _ptrAutomaton;
	//_ptrInternals->Automaton->Print(_ptrInternals->Automaton);
	_ptrInternals->Grammar = _ptrGrammar;
	_ptrInternals->RowCount = __countRows;
	_ptrInternals->NonTerminalCount = __countNonterminals;
	_ptrInternals->TerminalCount = __countTerminals;
	_ptrInternals->StateCount = GetStateCount(_ptrInternals->Automaton->Closures->Head);
	GenerateActionTable(__ptrInput);
	GenerateReduceTable(__ptrInput);
	GenerateAcceptTable(__ptrInput);
	GenerateGotoTable(__ptrInput);
	CheckSR_Err(__ptrInput);


//	_ptrAutomaton->Print(_ptrAutomaton);
	
	return __ptrInput;
	}



void GenerateActionTable(struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = _ptrInternals->Automaton->Closures->Head;
	struct Closure *_ptrClosure;
	struct LinkedListNode *_ptrLinks;
	struct Link *_ptrLink;
	int _state =0;
		int _rIndex;
	int _cIndex;

	_ptrInternals->Action = InitiateMatrix(GetStateCount(_ptrClosures), _ptrInternals->TerminalCount + 1);

	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		_ptrLinks = _ptrClosure->Links->Head;
		while (_ptrLinks->Next !=NULL){
			_ptrLink = (struct Link *)_ptrLinks->Value;
			if (IsTerminal(_ptrLink->Token, _ptrInternals->NonTerminalCount, _ptrInternals->TerminalCount)==1){
				_rIndex =_ptrClosure->State;
				_cIndex =_ptrLink->Token-_ptrInternals->NonTerminalCount;
				if (_ptrInternals->Action[_rIndex][_cIndex]!=_ptrLink->NextClosure->State &&
					_ptrInternals->Action[_rIndex][_cIndex]!=-1){
					printf("SHIFT/SHIFT ERROR\n");
					
					exit(0);
					
				}
				else{
				if (_rIndex ==39){
						_rIndex =39;
					}
					_ptrInternals->Action[_rIndex][_cIndex]=_ptrLink->NextClosure->State;
				}
				
					
			}
			_ptrLinks = _ptrLinks->Next;
		}
		_ptrClosures = _ptrClosures->Next;
		_state ++;
	}
}
int GetStateCount(struct LinkedListNode *__ptrClosures){
		int _state;
	_state =0;
	while (__ptrClosures->Next!=NULL){

		_state ++;
		__ptrClosures = __ptrClosures->Next;
	}
	return _state;
}

int IsTerminal(int __Token, int __NonTerminalCount, int __TerminalCount){
	if (__Token >= __NonTerminalCount && __Token <__TerminalCount+__NonTerminalCount){
		return 1;
	}
	else{
		return 0;
	}
}
int **InitiateMatrix( int __rowCount, int __colCount){
	int i;
	int j;
	int **_ptrOutput= (int **)malloc(sizeof(int *)*__rowCount);
	for ( i= 0; i< __rowCount; i++){
		_ptrOutput[i]= (int *)malloc(sizeof(int)*__colCount);
		for ( j =0; j< __colCount;j++){
			_ptrOutput[i][j] = -1;
		}
	}

	return _ptrOutput;
}

 void PrintMatrix(int **__ptrInput, int __countRows, int __countColumns){
	
	  int i;
	  int j;
	    printf("Printing Matrix\n");
	 for ( i = 0; i<__countRows;i++){
		 for ( j = 0; j < __countColumns;j++){
			printf("%4d", __ptrInput[i][j]);
		 }
		 printf("\n");
	 }
 }

static void NextGoto(struct ParseTable *__ptrParseTable, int __state, int *__nextState, int __reduceNTer){
	 struct internals *_ptrInternals = (struct internals *)__ptrParseTable->internals;
	*__nextState = _ptrInternals->Goto[__state][__reduceNTer-1];
 
 }

  void NextAc_Ter(struct ParseTable *__ptrParseTable, int __nextToken, int __state, int *__nextState, enum EnumActions *__actionType, int *__reduceNTer){
	
 }

 void NextAction(struct ParseTable *__ptrParseTable, int __nextToken, int __state, int *__nextState, enum EnumActions *__actionType, 
	 int *__reduceNTer, int *__reducePoC){
	struct internals *_ptrInternals = (struct internals *)__ptrParseTable->internals;
	int _colIndex;

	if (_ptrInternals->Grammar->IsEndMarker(__nextToken, _ptrInternals->Grammar)==1){
		_colIndex = __nextToken-1-_ptrInternals->NonTerminalCount;
	}
	else{
		_colIndex = __nextToken-_ptrInternals->NonTerminalCount;
	}
	if (_ptrInternals->Action[__state][_colIndex] != -1){
		*__actionType = SHIFT;
		*__nextState = _ptrInternals->Action[__state][_colIndex];
	}
	else if(_ptrInternals->Reduce[__state][_colIndex] != -1){
		*__actionType = REDUCE;
		*__reduceNTer = _ptrInternals->Grammar->Rows[_ptrInternals->Reduce[__state][_colIndex]];
		*__reducePoC = _ptrInternals->Grammar->ProTokCount(_ptrInternals->Grammar, _ptrInternals->Reduce[__state][_colIndex]);
	}
	else if(_ptrInternals->Accept[__state][_colIndex] != -1){
		*__actionType = ACCEPT;
	}
	else{
		*__actionType = ERROR_A;
	}
	
 }

  static void * c_ctor(){
	struct ParseTable *_ptrOutput = (struct ParseTable *)malloc(sizeof(struct ParseTable));
	struct internals *_ptrInternals  = (struct internals *)malloc(sizeof(struct internals));
	struct Automaton *_ptrAutomaton = (struct Automaton *)malloc(sizeof(struct Automaton));
	_ptrInternals->Automaton = _ptrAutomaton;
	_ptrOutput->internals = _ptrInternals;
	_ptrOutput->init = init;
	_ptrOutput->NextAction = NextAction;
	_ptrOutput->NextGoto = NextGoto;

	return _ptrOutput;
}
struct  Class_Generic _PARSETABLE = {c_ctor};

const void *PARSETABLE = &_PARSETABLE;
