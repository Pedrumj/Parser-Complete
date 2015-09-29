#include "LexAnalyzer.h"
#include <Windows.h>
#include "..\General\Class_Generic.h"
#include "..\General\LinkedList.h"
#include "RegDefs.h"
#include "..\General\Create_object.h"
#include <stdio.h>
#include "LexDefs.h"
#include "NFA.h"
#include "..\General\String.h"



struct internals{
	 struct NFA_Class *NFAs;
};

//Retunrs handle to the file with the source code
static FILE *GetRuleF(){
	FILE *_fp;
	//file path in char *
	char *_strPath2=(char *)malloc(sizeof(char)*BUFFER);
	//directory path in wchar_t
	WCHAR * _strPath =(WCHAR *)malloc(sizeof(WCHAR)*BUFFER);
	//the lenght of the directory string
	int _pathLen;
	//temp variable
	size_t _intR;
	//current directory
	_pathLen=GetCurrentDirectory(BUFFER, _strPath);
	_strPath[_pathLen]='\0';

	//converting current directory string to char *
	wcstombs_s(&_intR, _strPath2, (size_t)BUFFER, _strPath,  (size_t)BUFFER);
	//rule file
	strcat_s(_strPath2,BUFFER, "\\Input\\Input.txt");
	fopen_s(&_fp, _strPath2,"r");
	return _fp;
}


//adds the lexems in the input character string, to the input list of lexems
//__ptrNFA: Contains all the NFAs associated with the RegExs
//__ptrLexems: List of lexems. New values will be added to this list
//__strLine: A line of input from the text file with the source code
void GetLexems(struct NFA_Class *__ptrNFA, struct LinkedList *__ptrLexems, char *__strLine){
	int *_index;
	int _lenStr;
	int _lexIndex;
	int _indexBef;
	struct Lexem *_ptrLexem;
	_index =(int *)malloc(sizeof(int));
	(*_index) =0;
	
	_lenStr = strlen(__strLine);
	//keeps going until the end of the line
	while (*_index < _lenStr){
		_indexBef= *_index;
		//returns the ID associate with the lexem. The list can be found in LexDefs.h
		_lexIndex = __ptrNFA->GetLex(__ptrNFA, __strLine, _index);
		//if -1, then none of the lexems are a match. Therefore its just a SINGLE character
		if (_lexIndex ==-1){
			(*_index) +=1;
		}
		_ptrLexem = (struct Lexem*)malloc(sizeof(struct Lexem));
		_ptrLexem->Value = (char *)malloc(sizeof(char)*BUFFER);
		cpy_mid(_ptrLexem->Value, __strLine, _indexBef, (*_index)-1);
		if (_lexIndex != -1){
			_ptrLexem->Type  = lexDef_Token[_lexIndex];
			_ptrLexem->SubType  = lexDef_ST[_lexIndex];
		}
		else{
			_ptrLexem->Type  =SINGLE;
		
		}
		__ptrLexems->Add(_ptrLexem, __ptrLexems);

	}
}

void PrintLexems(struct LinkedList *__ptrLexems){
	struct  Lexem *_ptrLexem;
	struct LinkedListNode *_ptrNode;
	
	printf("\n\nPringing Lexems\n");
	_ptrNode = __ptrLexems->Head;
	while(_ptrNode->Next !=NULL){
		_ptrLexem = (struct Lexem *)_ptrNode->Value;
		switch(_ptrLexem->Type){
		case 0: 
			printf("%-10s", "WS");
			break;
		case 1: 
			printf("%-10s","IF");
			break;
		case 2: 
			printf("%-10s","THEN");
			break;
		case 3: 
			printf("%-10s","ELSE");
			break;
		case 4: 
			printf("%-10s","ID");
			break;
		case 5: 
			printf("%-10s","NUMBER");
			break;
		case 6: 
			printf("%-10s","RELOP");
			break;
		case 7: 
			printf("%-10s","SINGLE");
			break;
		}	
		printf("%-10s\n",_ptrLexem->Value);
		_ptrNode = _ptrNode->Next;
	}
}

//returns a linked list of lexems based on the source code in the input file
struct LinkedList * Analyze(struct LexAnalyzer *__ptrInput){
	FILE *_fp = GetRuleF();
	char *_read;
	struct internals *_ptrInternals;
	struct LinkedList *_ptrLexems;

	_ptrLexems = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrLexems = _ptrLexems->init(_ptrLexems, sizeof(struct Lexem));

	_read = (char *)malloc(sizeof(char)*BUFFER);
	_ptrInternals = (struct internals *)__ptrInput->internals;
	while (fgets(_read, BUFFER, _fp)){
		printf( _read);		
		GetLexems(_ptrInternals->NFAs, _ptrLexems, _read);
	}
	printf("\n");


	return _ptrLexems;
}
static struct LexAnalyzer* init(struct LexAnalyzer *__ptrInput){
	struct internals *_ptrInternals;
	struct NFA_Class *_ptrNFA;
	struct RegDefs *_ptrRegDefs;
	struct LexDefs *_ptrLexDefs;

	_ptrInternals = (struct internals *)__ptrInput->internals;
	_ptrRegDefs = (struct RegDefs  *)Create_Object(REGDEFS);
	_ptrRegDefs = _ptrRegDefs->init(_ptrRegDefs); 


	_ptrLexDefs = (struct LexDefs  *)Create_Object(LEXDEFS);
	_ptrLexDefs = _ptrLexDefs->init(_ptrLexDefs, _ptrRegDefs);


	_ptrNFA = (struct NFA_Class *)Create_Object(NFA_CLASS);
	_ptrNFA = _ptrNFA->init(_ptrNFA, _ptrLexDefs->LexTrees);
	_ptrInternals->NFAs = _ptrNFA;
	return __ptrInput;

}

static void * c_ctor(){
	struct  LexAnalyzer *_ptrOutput = (struct LexAnalyzer *)malloc(sizeof(struct LexAnalyzer));
	struct internals *_ptrInternals;

	_ptrInternals = (struct internals *)malloc(sizeof(struct internals));
	_ptrOutput->internals = _ptrInternals;
	_ptrOutput->init = init;
	_ptrOutput->Analyze = Analyze;
	return _ptrOutput;
}

struct  Class_Generic _LEXANALYZER = {c_ctor};

const void *LEXANALYZER = &_LEXANALYZER;