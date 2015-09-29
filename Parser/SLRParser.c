#include <Windows.h>
#include <stdio.h>
#include "ParseTable.h"
#include "SLRParser.h"
#include "../Lexical Analyzer/LexAnalyzer.h"

#include "../Lexical Analyzer/LexDefs.h"





struct internals{
	struct ParseTable *ParseTable;
};


int Par_Red(struct ParseTree *__ParseTree, struct ParseTreeNode *__ptrNode,  struct SLRParser *__ptrParser, struct Stack *__ptrStack, int __state, int __reduceNTer, 
			int __reducePopCount){
	struct internals *_ptrInternals  = (struct internals *)__ptrParser->internals;
	int _nextState;

	__state = *(int *)__ptrStack->Read(__ptrStack);
	_ptrInternals->ParseTable->NextGoto(_ptrInternals->ParseTable, __state, &_nextState,__reduceNTer);
	if (_nextState!=-1){
		__ptrStack->Push(__ptrStack, &_nextState);
		return 1;
	}
	else{
		return -1;
	}
}



struct ParseTree * Parse(struct SLRParser *__ptrParser, int *__tokens){


	struct Stack *_ptrStack = (struct Stack *)Create_Object(STACK);
	struct ParseTree *_ptrOutput = (struct ParseTree*)Create_Object( PARSETREE);
	struct ParseTreeNode *_ptrHead;

	struct internals *_ptrInternals = (struct internals *)__ptrParser->internals;
	int _state;
	int _nextState;
	int _popCount;
	int _tokenIndex =0;
	int _reduceNTer;
	int i;
	enum EnumActions _nextAction;
	int df;

	

	_ptrOutput = _ptrOutput->init(_ptrOutput, sizeof(int));
	_ptrStack = _ptrStack->init(_ptrStack, sizeof(int));
	_state =0;
	_ptrStack->Push(_ptrStack, &_state);

	_ptrHead= _ptrOutput->Head;

	while (1){
			
		_state = *(int *)_ptrStack->Read(_ptrStack);
		if (_tokenIndex ==7){
		_tokenIndex=7;
		}
		_ptrInternals->ParseTable->NextAction(_ptrInternals->ParseTable,__tokens[_tokenIndex], _state, &_nextState, &_nextAction, 
			&_reduceNTer, &_popCount);
		if (_nextAction == SHIFT){
			if (__tokens[_tokenIndex]==6){
				df=1;
			}
			_ptrStack->Push(_ptrStack, &_nextState);
			_ptrHead=_ptrOutput->Add_Next(_ptrOutput, _ptrHead, &__tokens[_tokenIndex]);
			_tokenIndex++;
		}
		else if (_nextAction == REDUCE){
			if (_popCount >0){
				_ptrStack->Pop(_ptrStack);
			}
			for ( i =0;i <_popCount-1;i++){
				_ptrStack->Pop(_ptrStack);
				_ptrHead=_ptrOutput->GetPrevious(_ptrHead);

			}
			_ptrHead=_ptrOutput->Add_PSh(_ptrOutput, _ptrHead, &_reduceNTer);
			if (Par_Red(_ptrOutput,_ptrHead, __ptrParser, _ptrStack, _state,  _reduceNTer, _popCount) ==-1){
				return NULL;
			}
		}
		else if (_nextAction == ACCEPT){
			return _ptrOutput;
		}
		else{
			return NULL;
		}
	}
	return NULL;
}


static struct  SLRParser * init(struct SLRParser *__ptrInput, int **__Grammar, int *__Rows, int __countRows, 
	int __countNonterminals, int __countTerminals){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	_ptrInternals->ParseTable = (struct ParseTable *)Create_Object(PARSETABLE);
	_ptrInternals->ParseTable = _ptrInternals->ParseTable->init(_ptrInternals->ParseTable, __Grammar, __Rows,
		__countRows, __countNonterminals, __countTerminals);
	return __ptrInput;
}


 static void * c_ctor(){
	struct  SLRParser *_ptrOutput = (struct SLRParser *)malloc(sizeof(struct SLRParser));
	struct internals *_ptrInternals  = (struct internals *)malloc(sizeof(struct internals));
	struct ParseTable *_ptrParseTable= (struct ParseTable *)malloc(sizeof(struct ParseTable));

	_ptrInternals->ParseTable = _ptrParseTable;
	_ptrOutput->internals  = _ptrInternals;
	_ptrOutput->init = init;
	_ptrOutput->Parse = Parse;
	return _ptrOutput;
}

struct  Class_Generic _SLRPARSER = {c_ctor};

const void *SLRPARSER = &_SLRPARSER;
