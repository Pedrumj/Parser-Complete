#include "Stack.h"
#include "Class_Generic.h"
#include <stdio.h>


struct Node{
	struct Node *Previous;
	void *Value;
};

struct internals{
	struct Node *Head;
	size_t size;
};

void Push(struct Stack *__ptrStack, void *__newValue){
	struct Node *_ptrNew = (struct Node *)malloc(sizeof(struct Node));
	struct internals *_ptrInternals = (struct internals *)__ptrStack->internals;

	_ptrNew->Value= (void *)malloc(_ptrInternals->size);
	memcpy(_ptrNew->Value, __newValue, _ptrInternals->size);
	_ptrNew->Previous = _ptrInternals->Head;
	_ptrInternals->Head  =_ptrNew;
}

void *Pop(struct Stack *__ptrStack){
	struct internals *_ptrInternals = (struct internals *)__ptrStack->internals;
	struct Node *_ptrTemp;// = (struct StackNode *)malloc(sizeof(struct StackNode));
	void *_output;
	if (_ptrInternals->Head->Previous !=NULL){
		_ptrTemp = _ptrInternals->Head;
		_ptrInternals->Head = _ptrInternals->Head->Previous;
		_output= _ptrTemp->Value;
		_ptrTemp->Previous = NULL;
		_ptrTemp = NULL;
		return _output;
	}
	else{
		return NULL;
	}
	

}

void *Read(struct Stack *__ptrStack){
	struct internals *_ptrInternals = (struct internals *)__ptrStack->internals;
	struct Node *_ptrTemp;// = (struct StackNode *)malloc(sizeof(struct StackNode));
	void *_output;
	if (_ptrInternals->Head->Previous !=NULL){
		_ptrTemp = _ptrInternals->Head;
		//_ptrInternals->Head = _ptrInternals->Head->Previous;
		_output= _ptrTemp->Value;
		//_ptrTemp->Previous = NULL;
		//_ptrTemp = NULL;
		return _output;
	}
	else{
		return NULL;
	}
	

}

static struct Stack *init(struct Stack *__ptrInput, size_t __size){
	struct  Node *_ptrTemp = (struct Node *)malloc(sizeof (struct Node));
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
		int Temp = _ptrInternals->size;
	_ptrTemp->Previous = NULL;
	_ptrInternals->Head = _ptrTemp;
	_ptrInternals->size = __size;

	return __ptrInput;
}


 static void * c_ctor(){
	struct  Stack *_ptrOutput = (struct Stack *)malloc(sizeof(struct Stack));

	struct internals *_ptrInternals  = (struct internals *)malloc(sizeof(struct internals));
	_ptrOutput->internals = _ptrInternals;
	_ptrOutput->Pop = Pop;
	_ptrOutput->Push = Push;
	_ptrOutput->Read = Read;
	_ptrOutput->init = init;
	

	return _ptrOutput;
}

struct  Class_Generic _STACK = {c_ctor};
//
const void *STACK = &_STACK;
