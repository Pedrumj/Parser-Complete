#include "NFA.h"
#include <Windows.h>
#include "..\General\Class_Generic.h"
#include "LexTree.h"
#include "..\General\LinkedList.h"
#include <stdio.h>
#include <Windows.h>
#include "..\General\String.h"

struct internals{
	struct LinkedList *NFAs;
};


void GenNFA_Aux(struct ParseTreeNode *__ptrNode, struct NFA_Node *__ptrNFA);

//creates a new empty nfa node
struct NFA_Node *NewNode(){
	struct NFA_Node *_ptrOutput;
	_ptrOutput = (struct NFA_Node *)malloc(sizeof(struct NFA_Node));
	_ptrOutput->Nodes = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrOutput->Links = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrOutput->Nodes = _ptrOutput->Nodes->init(_ptrOutput->Nodes, sizeof(struct NFA_Node));
	_ptrOutput->Links = _ptrOutput->Links->init(_ptrOutput->Links, sizeof(char));
	return _ptrOutput;

}


//creates a new empty nfa
struct NFA_Node *NewNFA(){
	struct NFA_Node *_ptrOutput;
	struct NFA_Node *_ptrFinal;
	struct NFA_Node *_ptrTemp;
	char _link;


	//malloc
	_ptrOutput =  NewNode();
	_ptrFinal = NewNode();

	//states
	_ptrOutput->State =-1;
	_ptrFinal->State = -1;


	
	_link = '\0';

	_ptrOutput->Nodes->AddRef(_ptrFinal,_ptrOutput->Nodes);
	_ptrOutput->Links->Add(&_link, _ptrOutput->Links);

	_ptrTemp =(struct NFA_Node *)_ptrOutput->Nodes->Head->Value;
	//Links to first and last
	_ptrOutput->First = _ptrOutput;
	_ptrOutput->Final = (struct NFA_Node *)_ptrOutput->Nodes->Head->Value;
	
	((struct NFA_Node *)_ptrOutput->Nodes->Head)->First = _ptrOutput;
	((struct NFA_Node *)_ptrOutput->Nodes->Head)->Final = (struct NFA_Node *)_ptrOutput->Nodes->Head;	


	return  _ptrOutput;
}

//generates nfa for literal node
void GenNFA_LIT(struct ParseTreeNode *__ptrNode, struct NFA_Node *__ptrNFA){
	struct LexTreeNode * _ptrLexNode;
	char *_link;

	_ptrLexNode = (struct LexTreeNode *)__ptrNode->Value;
	
	
	if (strcmp(_ptrLexNode->Value, "")==0){
		_link = (char *)malloc(sizeof(char));
		*_link = '\0';
		__ptrNFA->Links->Head->Value=_link;
	

	}
	else{
	__ptrNFA->Links->Head->Value=_ptrLexNode->Value;
	}
}

//generate nfa for or node
void GenNFA_OR(struct ParseTreeNode *__ptrNode, struct NFA_Node *__ptrNFA){
	struct NFA_Node *_ptr1;
	struct NFA_Node *_ptr2;
	char _link;


	_ptr1 = NewNFA();
	_ptr2 = NewNFA();

	GenNFA_Aux(__ptrNode->FirstChild, _ptr1);
	GenNFA_Aux(__ptrNode->FirstChild->Next, _ptr2);

	//add the second outgoing link
	_link = '\0';
	__ptrNFA->Links->Add(&_link, __ptrNFA->Links);
	
	//set the start
	__ptrNFA->Nodes->Head->Value = _ptr1;
	__ptrNFA->Nodes->AddRef(_ptr2, __ptrNFA->Nodes);

	//set the end
	_ptr1->Final->Links->Add(&_link, _ptr1->Final->Links);
	_ptr1->Final->Nodes->AddRef(__ptrNFA->Final, _ptr1->Final->Nodes);

	//set the end
	_ptr2->Final->Links->Add(&_link, _ptr2->Final->Links);
	_ptr2->Final->Nodes->AddRef(__ptrNFA->Final, _ptr2->Final->Nodes);

	//remove final state from the end
	_ptr1->Final->IsFinal = 0;
	_ptr2->Final->IsFinal = 0;


}

//generates NFA for concate node
void GenNFA_CONC(struct ParseTreeNode *__ptrNode, struct NFA_Node *__ptrNFA){
	struct NFA_Node *_ptr1;
	struct NFA_Node *_ptr2;
	char _link;

	_ptr1 = NewNFA();
	_ptr2 = NewNFA();


	GenNFA_Aux(__ptrNode->FirstChild, _ptr1);
	GenNFA_Aux(__ptrNode->FirstChild->Next, _ptr2);
	
	//set the start
	__ptrNFA->Nodes->Head->Value = _ptr1;

	//set the concate
	_link ='\0';
	_ptr1->Final->Links->Add(&_link, _ptr1->Final->Links);
	_ptr1->Final->Nodes->AddRef(_ptr2, _ptr1->Final->Nodes);
	
	//set the end
	_ptr2->Final->Links->Add(&_link, _ptr2->Final->Links);
	_ptr2->Final->Nodes->AddRef(__ptrNFA->Final, _ptr2->Final->Nodes);

	//remove final state from the end
	_ptr1->Final->IsFinal = 0;
	_ptr2->Final->IsFinal = 0;


}


//Generates NFA for star node
void GenNFA_STAR(struct ParseTreeNode *__ptrNode, struct NFA_Node *__ptrNFA){
	struct NFA_Node *_ptr1;

	char _link;


	_ptr1 = NewNFA();
	GenNFA_Aux(__ptrNode->FirstChild, _ptr1);
		
	//set the start
	__ptrNFA->Nodes->Head->Value = _ptr1;
	
	
	_link ='\0';

	//add the shortcut
	__ptrNFA->Links->Add(&_link, __ptrNFA->Links);
	__ptrNFA->Nodes->AddRef(__ptrNFA->Final, __ptrNFA->Nodes);

	//set the concate
	_ptr1->Final->Links->Add(&_link, _ptr1->Final->Links);
	_ptr1->Final->Nodes->AddRef(__ptrNFA->Final, _ptr1->Final->Nodes);
	
	//loop the middle
	_ptr1->Final->Links->Add(&_link, _ptr1->Final->Links);
	_ptr1->Final->Nodes->AddRef(_ptr1, _ptr1->Final->Nodes);
	_ptr1->Final->IsFinal =0;

}

//Auxially function for adjusting NFAs
void Adj_NFAs_Aux(struct NFA_Node *__ptrNFA, int *__State, struct NFA_Node *__ptrFirst, struct NFA_Node *__ptrFinal){
	struct NFA_Node *_ptrNFANode;
	struct LinkedListNode *_ptrNode;

	__ptrNFA->State = *__State;
	__ptrNFA->Final = __ptrFirst;
	__ptrNFA->Final	= __ptrFinal;
	__ptrNFA->IsFinal = 0;


	_ptrNode = __ptrNFA->Nodes->Head;
	while (_ptrNode->Next !=NULL){
		_ptrNFANode = (struct NFA_Node *)_ptrNode->Value;
		if (_ptrNFANode->State ==-1){
			(*__State) +=1;
			Adj_NFAs_Aux(_ptrNFANode, __State, __ptrFirst, __ptrFinal);
		}
		_ptrNode = _ptrNode->Next;
	}
}

//The method used for generating the NFA, creates incorrect ->Final and ->Final properties for mid states
//This function corrects this
void Adj_NFAs(struct NFA_Node *__ptrNFA){
	int *_State;
	
	_State = (int *)malloc(sizeof(int));
	*_State =0;
	Adj_NFAs_Aux(__ptrNFA, _State, __ptrNFA->First, __ptrNFA->Final);
	__ptrNFA->Final->IsFinal =1;
}

//Auxiallary function for generating an NFA
void GenNFA_Aux(struct ParseTreeNode *__ptrNode, struct NFA_Node *__ptrNFA){
	struct LexTreeNode *_ptrLexNode;

	_ptrLexNode = (struct LexTreeNode * )__ptrNode->Value;
	switch(_ptrLexNode->NodeType){
	case LITERAL:
		GenNFA_LIT(__ptrNode, __ptrNFA);
		break;
	case OR:
		GenNFA_OR(__ptrNode, __ptrNFA);
		break;
	case CONCATE:
		GenNFA_CONC(__ptrNode, __ptrNFA);
		break;
	case STAR:
		GenNFA_STAR(__ptrNode, __ptrNFA);
		break;
	}
}

//Generates the NFA associated with the input LexTree
struct NFA_Node *GenNFA(struct ParseTree *__ptrLexTree){
	struct NFA_Node *_ptrOutput;



	_ptrOutput = NewNFA();

	GenNFA_Aux(__ptrLexTree->Head->FirstChild, _ptrOutput);
	Adj_NFAs(_ptrOutput);
	return _ptrOutput;
}

int SeenState(struct LinkedList *__Seen, int __NewState){
	struct LinkedListNode *_ptrHead;
	_ptrHead = __Seen->Head;
	while(_ptrHead->Next !=NULL){
		if (*(int *)_ptrHead->Value == __NewState){
			return 1;
		}
		_ptrHead = _ptrHead->Next;	
	}

	return 0;
}

char *GetLinkString(char __Input){
	char *_strOutput;
	_strOutput = (char *)malloc(sizeof(char)*1000);
	if (__Input == '\0'){
		_strOutput = "EPS\0";
	}
	else if (__Input == ' '){
		_strOutput = "SPACE\0";
	}
	else if (__Input == '\t'){
		_strOutput = "TAB\0";
	}
	else if (__Input == '\n'){
		_strOutput = "NEW LINE\0";
	}
	else{
		_strOutput[0] = __Input;
		_strOutput[1] ='\0';
	}
	return _strOutput;
}
void PrintNFA_Aux(struct NFA_Node *__ptrNFA, struct LinkedList *__Seen){
	struct NFA_Node *_ptrNode;
	struct LinkedListNode *_ptrHead;
	struct LinkedListNode *_ptrHeadLinks;
	int *_newState;
	char *_strOLink;
	char *_strTemp;

	_newState = (int *)malloc(sizeof(int));
	//printf("Printing State %d\n", __ptrNFA->State);
	_ptrHead = __ptrNFA->Nodes->Head;	
	_ptrHeadLinks = __ptrNFA->Links->Head;
	while (_ptrHead->Next !=NULL){
		_ptrNode = (struct NFA_Node *)_ptrHead->Value;

		if (SeenState(__Seen, _ptrNode->State)==0){
			_strTemp=(char *)_ptrHeadLinks->Value;
			_strOLink = GetLinkString(*(char *)_ptrHeadLinks->Value);
			if (_ptrNode->IsFinal ==1){
				printf("Is Final\n");
			}
			printf("%-15s%-15d\n", _strOLink, _ptrNode->State);
			*_newState = _ptrNode->State;
			__Seen->Add(_newState, __Seen);
			PrintNFA_Aux(_ptrNode, __Seen);
		}
		else{
			printf("Seen %d\n", _ptrNode->State);
		}
		_ptrHead = _ptrHead->Next;
		_ptrHeadLinks = _ptrHeadLinks->Next;
	}
}

void PrintNFA (struct NFA_Node *__ptrNFA){


	struct LinkedList *_Seen;


	printf("Printing NFA\n\n\n");
	printf("%-15s%-15s\n","Out_Links", "Out_States");

	_Seen = (struct LinkedList *)Create_Object(LINKEDLIST);
	_Seen =_Seen->init(_Seen, sizeof(int));


	//printf("%-15s%-15d\n", _strOLink, __ptrNFA->State);
	PrintNFA_Aux(__ptrNFA,_Seen);
	//printf("State = %d", __ptrNFA->State);


}


//Generates NFAs based on the input LexTrees
struct LinkedList *GenNFAs(struct LinkedList *__ptrLexTrees){
	struct LinkedListNode *_ptrNode;
	struct LexTree *_ptrLexTree;
	struct LinkedList *_NFAs;
	struct NFA_Node *_ptrNFA_Node;

	_NFAs = (struct LinkedList *)Create_Object(LINKEDLIST);
	_NFAs = _NFAs->init(_NFAs, sizeof(struct NFA_Node));
	_ptrNode = __ptrLexTrees->Head;
	while (_ptrNode->Next !=NULL){
		_ptrLexTree = (struct LexTree *)_ptrNode->Value;
		_ptrNFA_Node = GenNFA(_ptrLexTree->ParseTree);
		_NFAs->Add(_ptrNFA_Node, _NFAs);
		_ptrNode = _ptrNode->Next;
	}

	return _NFAs;
}

//compares the 2 states to see if they are equal
int Compare(void *__ptr1, void *__ptr2){
	struct NFA_Node *_ptrNFA1;
	struct NFA_Node *_ptrNFA2;

	_ptrNFA1 = (struct NFA_Node *)__ptr1;
	_ptrNFA2 = (struct NFA_Node *)__ptr2;
	if (_ptrNFA1->State ==_ptrNFA2->State){
		return 1;
	}
	else{
		return 0;
	}


}

//Generates the closure for the input NFA
struct LinkedList *Closure_E(struct NFA_Node *__ptrNFA){
	struct LinkedListNode *_ptrNode;
	struct LinkedListNode *_ptrLink;
	struct LinkedList *_ptrOutput;
	char _link;

	_ptrOutput = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrOutput= _ptrOutput->init(_ptrOutput, sizeof(struct NFA_Node));
	
	
	_ptrNode = __ptrNFA->Nodes->Head;	
	_ptrLink = __ptrNFA->Links->Head;

	while (_ptrNode->Next !=NULL){
		_link = *(char *)_ptrLink->Value;
		if (_link=='\0'){
			_ptrOutput->AddUnique(_ptrNode->Value, _ptrOutput, Compare);
				
		}
		_ptrLink = _ptrLink->Next;
		_ptrNode = _ptrNode->Next;
	}


	
	return _ptrOutput;
}

//Adds all memebrs in _ptrSrc to _ptrDest
int AddToList(struct LinkedList *_ptrSrc, struct LinkedList *_ptrDest){
	struct LinkedListNode *_ptrNode;
	int _flagChanged;

	_flagChanged = 0;
	_ptrNode = _ptrSrc->Head;
	while (_ptrNode->Next !=NULL){
		_flagChanged+=_ptrDest->AddUnique(_ptrNode->Value, _ptrDest, Compare);
		_ptrNode = _ptrNode->Next;
	}

	return _flagChanged;
}


//Generates the closure for the input set
struct LinkedList *Closure_ESet(struct LinkedList *__ptrNFAs){
	struct LinkedListNode *_ptrNode;
	struct LinkedList *_ptrOutput;
	struct NFA_Node *_ptrNFA;
	struct LinkedList *_ptrTemp;
	int _flagChanged;

	_ptrOutput = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrOutput= _ptrOutput->init(_ptrOutput, sizeof(struct NFA_Node));
	AddToList(__ptrNFAs, _ptrOutput);

	_flagChanged =1;
	while (_flagChanged != 0){
		_flagChanged =0;
		_ptrNode = _ptrOutput->Head;	
	
		while (_ptrNode->Next !=NULL){
			_ptrNFA = (struct NFA_Node *)_ptrNode->Value;
			_flagChanged+=_ptrOutput->AddUnique(_ptrNFA, _ptrOutput, Compare);
		
			_ptrTemp =   Closure_E(_ptrNFA);
			_flagChanged+=AddToList(_ptrTemp, _ptrOutput);
			_ptrNode = _ptrNode->Next;
		}
	}
	

	return _ptrOutput;
}


//Applies Move(C) on the input NFA
struct LinkedList * Move_C(struct NFA_Node *_ptrNFA, char __newChar){
	struct LinkedList *_ptrOutput;
	struct LinkedListNode *_ptrNodes;
	struct LinkedListNode *_ptrLinks;
	char _link;

	_ptrOutput = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrOutput = _ptrOutput->init(_ptrOutput, sizeof(struct NFA_Node));

	_ptrNodes = _ptrNFA->Nodes->Head;
	_ptrLinks =_ptrNFA->Links->Head;
	while (_ptrNodes->Next !=NULL){
		_link = *(char *)_ptrLinks->Value;
		if (_link == __newChar){
			_ptrOutput->Add(_ptrNodes->Value, _ptrOutput);
		} 
		_ptrLinks = _ptrLinks->Next;
		_ptrNodes = _ptrNodes->Next;
	}

	return _ptrOutput;
}


//Applies Move(C, S) to the set "S" on the input character
struct LinkedList *Move_Set(struct LinkedList *__ptrNFAs, char __newChar){
	struct LinkedListNode *_ptrNode;
	struct LinkedList *_ptrOutput;
	struct NFA_Node *_ptrNFA;
	struct LinkedList *_ptrTemp;

	_ptrOutput = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrOutput= _ptrOutput->init(_ptrOutput, sizeof(struct NFA_Node));
	
	_ptrNode = __ptrNFAs->Head;	
	
	while (_ptrNode->Next !=NULL){

		_ptrNFA = (struct NFA_Node *)_ptrNode->Value;
		_ptrTemp =   Move_C(_ptrNFA, __newChar);
		AddToList(_ptrTemp, _ptrOutput);
		_ptrNode = _ptrNode->Next;
	}

	return _ptrOutput;
}

//checks if a Final state is in the list of NFAs
int IsFinal(struct LinkedList *_ptrNodes){
	struct LinkedListNode *_ptrNode;
	struct NFA_Node *_ptrNFA;

	_ptrNode = _ptrNodes->Head;
	while (_ptrNode ->Next !=NULL){
		_ptrNFA = (struct NFA_Node *)_ptrNode->Value;
		if (_ptrNFA->IsFinal ==1){
			return 1;
		}
		_ptrNode = _ptrNode->Next;
	}
	return 0;
}


//checks if the input string is assocaited with the input NFA or not. _Index is set to the end
//location of the matching lexem
int TryString(char *__strInput, int *_Index, struct NFA_Node *__ptrNFA, int *__IsEmpty){
	struct LinkedList *_ptrSet;
	struct LinkedList *_ptrNew;
	int *_indexTemp;

	_ptrSet = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrSet = _ptrSet->init(_ptrSet, sizeof(struct NFA_Node));
	_ptrSet->Add(__ptrNFA, _ptrSet);

	_indexTemp = (int *)malloc(sizeof(int));
	*_indexTemp = *_Index;
	while (_ptrSet->Head->Next!=NULL && __strInput[*_indexTemp]!='\0'){
		_ptrNew = Closure_ESet(_ptrSet);
		free( _ptrSet);
		_ptrSet = _ptrNew;

		_ptrNew = Move_Set(_ptrSet, __strInput[*_indexTemp] );
		free( _ptrSet);
		_ptrSet = _ptrNew;
		(*_indexTemp) +=1;
	}
	_ptrNew = Closure_ESet(_ptrSet);
	if( IsFinal(_ptrNew)==1){
		*_Index = *_indexTemp;
		return 1;
		
	}

	if (_ptrSet->Head->Next ==NULL){
		(*__IsEmpty) =1;
	}
	else{
		(*__IsEmpty) =0;
	}
	return 0;
}


//checks if the input NFA is the NFA associated with the input string. 
int TryLex(char *__strInput, int *_Index, struct NFA_Node *__ptrNFA){
	int _indexTemp;
	int _IsEmpty;
	int _strLen;
	int _eIndex;
	char *_strTemp;
	int _isValid;
	int _lastValid;
	int _indexDiff;


	_strLen =strlen(__strInput);
	_IsEmpty = 0;
	_indexTemp =0;
	_eIndex = *_Index;
	_lastValid =-1;
	_strTemp = (char *)malloc(sizeof(char));
	while(_IsEmpty ==0 && _strLen != _indexTemp){
		_indexTemp = *_Index;
		cpy_mid(_strTemp, __strInput, _indexTemp, _eIndex);
		_indexDiff =0;
		_isValid = TryString(_strTemp, &_indexDiff, __ptrNFA, &_IsEmpty);
		_indexTemp = _indexDiff + *_Index;
		if (_isValid != 0){
			_lastValid = _indexTemp -1;
		}
		_eIndex ++;

	}

	if (_lastValid !=-1){
		(*_Index) = _lastValid+1;
		return 1;
	}
	else{
		return 0;
	}

}



//returns the ID of the Lex associated with the input string. __index sepecifies where to start looking. 
//THe list of lexes can be found in LexDef.h

int GetLex(struct NFA_Class *__ptrInput, char *__strInput, int *__Index){
	struct LinkedListNode *_ptrNode;
	struct NFA_Node *_ptrNFA;
	struct internals *_ptrInternals;
	int _Lex;
	int _indexMax;
	int _indexTemp;
	int _lexMax;

	_ptrInternals = (struct internals *)__ptrInput->internals;
	_ptrNode = _ptrInternals->NFAs->Head;
	_Lex  =0;
	_indexMax =0;
	_lexMax =-1;
	while (_ptrNode->Next !=NULL){
		_ptrNFA = (struct NFA_Node *)_ptrNode->Value;
		_indexTemp=*__Index;

		if (TryLex(__strInput, &_indexTemp, _ptrNFA) ==1){
			if (_indexTemp > _indexMax ){
				_indexMax = _indexTemp;
				_lexMax =_Lex;
			}
		}
		
		_Lex ++;
		_ptrNode = _ptrNode->Next;
	}
	if (_lexMax != -1){
		(*__Index) = _indexMax;
		return _lexMax;
	}
	return -1;
}

static struct NFA_Class * init(struct NFA_Class *__ptrInput, struct LinkedList *__ptrLexTrees){
	struct internals *_ptrInternals;

	_ptrInternals = (struct internals *)__ptrInput->internals;
	_ptrInternals->NFAs=GenNFAs(__ptrLexTrees);
	

	return __ptrInput;
}

static void * c_ctor(){
	struct  NFA_Class *_ptrOutput = (struct NFA_Class *)malloc(sizeof(struct NFA_Class));
	struct internals *_ptrInternals;
	_ptrInternals = (struct internals *)malloc(sizeof(struct internals));
	_ptrOutput->internals = _ptrInternals;
	_ptrOutput->init = init;
	_ptrOutput->GetLex  = GetLex;
	return _ptrOutput;
}

struct  Class_Generic _NFA_CLASS = {c_ctor};

const void *NFA_CLASS = &_NFA_CLASS;