#include "Class_Generic.h"
#include "LinkedList.h"
#include <Windows.h>

void Add( void * __ptrValue, struct LinkedList *__ptrList){

	struct LinkedListNode *_ptrHead = __ptrList->Head;
	struct LinkedListNode *_ptrNew = (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode));
	_ptrNew->Value =NULL;
	_ptrNew->Next = NULL;


	while (_ptrHead->Next !=NULL){
		_ptrHead = _ptrHead->Next;	
	
	}
	_ptrHead->Value =malloc(__ptrList->Size);
	memcpy(_ptrHead->Value, __ptrValue,__ptrList->Size);
	_ptrHead->Next = _ptrNew;

}
void AddRef( void * __ptrValue, struct LinkedList *__ptrList){

	struct LinkedListNode *_ptrHead = __ptrList->Head;
	struct LinkedListNode *_ptrNew = (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode));
	_ptrNew->Value =NULL;
	_ptrNew->Next = NULL;


	while (_ptrHead->Next !=NULL){
		_ptrHead = _ptrHead->Next;	
	
	}
	 _ptrHead->Value=__ptrValue;
	_ptrHead->Next = _ptrNew;

}
static struct LinkedList * init(struct LinkedList *_ptrInput, int __Size){

	_ptrInput->Size = __Size;

	return _ptrInput;
}

int AddUnique(void * __Value, struct LinkedList * const __ptrList, int (*Compare)(void *__ptr1, void *__ptr2)){

	struct LinkedListNode *_ptrHead = __ptrList->Head;
	while (_ptrHead->Next !=NULL){
		if (Compare(_ptrHead->Value, __Value)==1){
			return 0;
		}
		_ptrHead=_ptrHead->Next;
	}

	__ptrList->Add(__Value, __ptrList);
	return 1;
}

static void Print(struct LinkedList *__ptrList, void (*Print)(void *__ptrValue)){
	struct LinkedListNode *_ptrHead;
	
	_ptrHead = __ptrList->Head;
	while (_ptrHead->Next!=NULL){
		Print(_ptrHead->Value);
		_ptrHead = _ptrHead->Next;
	}
}

int Count(struct LinkedList *__ptrList){
	int _output;
	struct LinkedListNode *_ptrNode;

	_ptrNode = __ptrList->Head;
	_output = 0;
	while(_ptrNode->Next !=NULL){
		_ptrNode = _ptrNode->Next;
		_output ++;
	}
	return _output;
}

 static void * c_ctor(){
	struct LinkedList *_ptrOutput = (struct LinkedList *)malloc(sizeof(struct LinkedList));
	struct LinkedListNode *_ptrNode = (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode));
	_ptrNode->Value =NULL;
	_ptrNode->Next = NULL;
	_ptrOutput->Head = _ptrNode;
	_ptrOutput->init = init;
	_ptrOutput->Add = Add;
	_ptrOutput->Print = Print;
	_ptrOutput->AddUnique =  AddUnique;
	_ptrOutput->AddRef= AddRef;
	_ptrOutput->Count= Count;
	return _ptrOutput;
}

struct  Class_Generic _LINKEDLIST = {c_ctor};

const void *LINKEDLIST = &_LINKEDLIST;


