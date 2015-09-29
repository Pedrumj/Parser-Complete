#include "RegDefs.h"
#include "..\General\Class_Generic.h"
#include <stdio.h>
#include "..\General\String.h"
#include "..\General\LinkedList.h"
#include "..\General\Create_object.h"

#define BUFFER 10000
#define REGDEF_COUNT 16

struct internals{
	struct LinkedList *Name;
	struct LinkedList *Expression;
};



char * regDef_Names[REGDEF_COUNT] = {"delim", 
	                      "ws", 
						  "letter", 
						  "digit",
						  "id", 
						  "number",
						  "IF",
						  "THEN", 
						  "ELSE", 
						  "LT", 
						  "LE", 
						  "EQ",
						  "NE",
						  "GT",
						  "GE", 
						  "WHILE"

						};
char * regDef_Exprs[REGDEF_COUNT] = {"[ \t\n]\0", 
	                      "{delim}+\0", 
						  "[A-Za-z]\0", 
						  "[0-9]\0",
						  "{letter}({letter}|{digit})*\0", 
						  "{digit}+(\\.{digit}+)?(E[\\+\\-]?{digit}+)?\0",
						  "if\0", 
						  "then\0",
						  "else\0",
						  "<\0", 
						  "<=\0", 
						  "=\0", 
						  "<>\0", 
						  ">\0", 
						  ">=\0", 
						  "while"
						};





//State, Expland, Regular Definition. Will ignore the RD
static void St_Exp_RD(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*2);
	
	_str2Cpy[0] = __strInput[*__index];
	_str2Cpy[1] = '\0';
	strcat_s(__strOutput, BUFFER,_str2Cpy );
	if (__strInput[*__index] =='}'){
		(*__index) ++;
	}
	else{
		(*__index) ++;
		St_Exp_RD(__strInput, __strOutput, __index);
	}
}


//State, Expand Multiple options copy escape. Copies the escape characters as is in multiple options
static void St_Exp_MOpts_CpyEsc(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*4);
	_str2Cpy[0] = __strInput[*__index];
	_str2Cpy[1] = __strInput[*__index+1];
	_str2Cpy[2] = '|';
	_str2Cpy[3] = '\0';
	strcat_s(__strOutput, BUFFER,_str2Cpy );
	(*__index) += 2;
}


//state expand, multiple options , copies characters in multiple options as is
static void St_Exp_MOpts_CpyChr(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*3);
	_str2Cpy[0] = __strInput[*__index];
	_str2Cpy[1] = '|';
	_str2Cpy[2] = '\0';
	strcat_s(__strOutput, BUFFER,_str2Cpy );
	(*__index) += 1;
}


//state, expland multiple options, generates range of characters in multiple options
static void St_Exp_MOpts_Rng(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*3);
	char _char1;
	char _char2;
	int i;
	_char1 = __strInput[*__index];
	_char2 = __strInput[(*__index)+2];
	for (i =_char1; i<=_char2; i++){
		_str2Cpy[0] = i;
		_str2Cpy[1] = '|';
		_str2Cpy[2] = '\0';
		strcat_s(__strOutput, BUFFER,_str2Cpy );
	}
	
	(*__index) += 3;
}

//state expland multiple options
static void St_Exp_MOpts(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*2);
	
	
	if (__strInput[*__index] ==']'){
		(*__index) ++;

		__strOutput = rm_right(__strOutput, 1);
		_str2Cpy[0] = ')';
		_str2Cpy[1] = '\0';
		strcat_s(__strOutput, BUFFER,_str2Cpy );
	}
	else if(__strInput[*__index] =='['){
		(*__index) ++;
		_str2Cpy[0] = '(';
		_str2Cpy[1] = '\0';
		strcat_s(__strOutput, BUFFER,_str2Cpy );
		St_Exp_MOpts(__strInput, __strOutput, __index);
	}
	else if(__strInput[*__index] =='\\'){
		St_Exp_MOpts_CpyEsc(__strInput, __strOutput, __index);
		St_Exp_MOpts(__strInput, __strOutput, __index);
	}
	else if(__strInput[(*__index)+1] =='-' && __strInput[(*__index)+2] !=']'){
		St_Exp_MOpts_Rng(__strInput, __strOutput, __index);
		St_Exp_MOpts(__strInput, __strOutput, __index);
	}
	else{
		St_Exp_MOpts_CpyChr(__strInput, __strOutput, __index);
		St_Exp_MOpts(__strInput, __strOutput, __index);
	}
}


//the default state in explansion. Paranthesis are added to the both sides of the character
static void St_Exp_Def(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*4);


	
	_str2Cpy[0] = '(';
	_str2Cpy[1] = __strInput[*__index];
	_str2Cpy[2] =')';
	_str2Cpy[3] = '\0';
	strcat_s(__strOutput, BUFFER,_str2Cpy );
		
	(*__index) ++;

}


//when a literal is fouund in the expansion that paranthesis must not be added
static void St_Exp_Lit(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*2);


	
	
	_str2Cpy[0] = __strInput[*__index];
	_str2Cpy[1] = '\0';
	strcat_s(__strOutput, BUFFER,_str2Cpy );
		
	(*__index) ++;

}


//Returns the start index of the closing paranthesis
//__strInput: The input string
//__index: The last index to start seraching
//_closing: The closing character to search for
//__statckCount: Set to zero
static int St_Exp_Plus_SI(char *__strInput,  int __index, const char _closing, int __stackCount){
	switch (__strInput[__index]){
		case '}' :
			if (_closing=='}'){
				return St_Exp_Plus_SI(__strInput,__index-1,_closing , __stackCount+1);
			
			}
			else{
				return St_Exp_Plus_SI(__strInput,__index-1,_closing , __stackCount);
				
			}
			break;
		case ')':
			if (_closing==')'){
				return St_Exp_Plus_SI(__strInput,__index-1,_closing , __stackCount+1);
				
			}
			else{
				return St_Exp_Plus_SI(__strInput,__index-1,_closing , __stackCount);
				
			}
			break;
		case '{' :
			if (_closing=='}'){
				if (__stackCount ==1){
					return __index;

				}
				else{
					return St_Exp_Plus_SI(__strInput,__index-1,_closing , __stackCount-1);
				}
			}
			else{
				return St_Exp_Plus_SI(__strInput,__index-1,_closing , __stackCount);
			}
			break;
		case '(':
			if (_closing==')'){
				if (__stackCount ==1){
					return __index;
				}
				else{
					return St_Exp_Plus_SI(__strInput,__index-1,_closing , __stackCount-1);
				}
			}
			else{
				return St_Exp_Plus_SI(__strInput,__index-1,_closing , __stackCount);
			}
			break;
	default:
		return St_Exp_Plus_SI(__strInput,__index-1,_closing , __stackCount);
		break;

	}

}

//When a plus is found the conten is copied twice and a * is added to the end
static void St_Exp_Plus(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*2);
	int _sIndex;
	char *_srtDupl;
	switch (__strInput[(*__index)-1]){
		case ')':
			_sIndex=St_Exp_Plus_SI(__strInput, (*__index)-1, ')', 0);
			break;
		case '}' :
			_sIndex=St_Exp_Plus_SI(__strInput, (*__index)-1, '}', 0);
			break;
	}
	_srtDupl = (char *)malloc(sizeof(char)*BUFFER);
	_srtDupl = cpy_mid(_srtDupl, __strInput, _sIndex, (*__index) -1);
	strcat_s(__strOutput, BUFFER,_srtDupl );
	_str2Cpy[0] = '*';
	_str2Cpy[1] = '\0';
	strcat_s(__strOutput, BUFFER,_str2Cpy );
	(*__index) ++;

}

//When a question mark is found the content is ORed with NULL
static void St_Exp_Ques(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*BUFFER);
	int _sIndex;
	int _eIndex;
	char *_srtDupl;

	_eIndex = strlen(__strOutput)-1;
	switch (__strOutput[_eIndex]){
		case ')':
			_sIndex=St_Exp_Plus_SI(__strOutput, _eIndex, ')', 0);
			break;
		case '}' :
			_sIndex=St_Exp_Plus_SI(__strOutput, _eIndex, '}', 0);
			break;
	}
	
	_srtDupl = (char *)malloc(sizeof(char)*BUFFER);
	_srtDupl = cpy_mid(_srtDupl, __strOutput, _sIndex, _eIndex);
	rm_right(__strOutput, _eIndex - _sIndex +1);
	_str2Cpy[0] = '(';
	_str2Cpy[1] = '\\';
	_str2Cpy[2] = 'e';
	_str2Cpy[3] = '|';
	_str2Cpy[4] = '\0';
	strcat_s(__strOutput, BUFFER,_str2Cpy );
	strcat_s(__strOutput, BUFFER,_srtDupl );
	_str2Cpy[0] = ')';
	_str2Cpy[1] = '\0';
	strcat_s(__strOutput, BUFFER,_str2Cpy );

	(*__index) ++;

}


//When a slash is found paranthesis are added to both sides
static void St_Exp_Slash(char *__strInput,char *__strOutput,  int *__index){
	char *_str2Cpy = (char *)malloc(sizeof(char)*5);

	_str2Cpy[0] = '(';
	_str2Cpy[1] = __strInput[*__index];
	_str2Cpy[2] = __strInput[(*__index)+1];
	_str2Cpy[3] =')';
	_str2Cpy[4] = '\0';
	strcat_s(__strOutput, BUFFER,_str2Cpy );
		
	(*__index) +=2;
}


//expand the regular definition to standard format
static char * Expand (char *__strInput){
	char *_strOutput = (char *)malloc(sizeof(char)*BUFFER);
	int _index;

	_index =0;

	_strOutput[0] ='\0';
	while (__strInput[_index]!='\0'){
		switch (__strInput[_index]){
			case '{' :
				St_Exp_RD(__strInput, _strOutput, &_index);
				break;
			case '[' :
				St_Exp_MOpts(__strInput, _strOutput, &_index);
				break;
			case '(':
			case ')':
			case '|':
			case '*':
				St_Exp_Lit(__strInput, _strOutput, &_index);
				break;
			case '?':
				St_Exp_Ques(__strInput, _strOutput, &_index);
				break;
			case '+':
				St_Exp_Plus(__strInput, _strOutput, &_index);
				break;
			case '\\':
				St_Exp_Slash(__strInput, _strOutput, &_index);
				break;
			default: 
				St_Exp_Def(__strInput, _strOutput, &_index);
		}
		
	}
	return _strOutput;
}


//replace all occurances of __str2Find with in __strInput with __str2Replace
static char * Exp_Curly_Replace(char *__strInput, char *__str2Find, char *__str2Replace){
	char *_ptrFind;
	_ptrFind=strstr(__strInput, __str2Find);
	while (_ptrFind){
		__strInput= str_Replace(__strInput, __str2Find, __str2Replace);
		_ptrFind=strstr(__strInput, __str2Find);
	}

	return __strInput;
}

//puts curly braces around the input character strings
static char *WrapCurlies(char *__strInput){
	char *_str2Cpy = (char *)malloc(sizeof(char)*BUFFER);
	char *_strOutput = (char *)malloc(sizeof(char)*BUFFER);
	_strOutput[0] = '\0';
	_str2Cpy[0] = '{';
	_str2Cpy[1] = '\0';
	strcat_s(_strOutput, BUFFER,_str2Cpy );
	strcat_s(_strOutput, BUFFER,__strInput );
	_str2Cpy[0] = '}';
	_str2Cpy[1] = '\0';
	strcat_s(_strOutput, BUFFER,_str2Cpy );
	return _strOutput;
}


//Expland occurances of other regular definition in the input regular defnition
static char * Exp_Curly(char *__strInput, struct LinkedList *__ptrNames, struct LinkedList *__ptrExprs){
	struct LinkedListNode *_ptrName_Header;
	struct LinkedListNode *_ptrExpr_Header;


	char *_str2Find;
	 char *_strName;
	 char *_strExpr;

	
	

	_ptrName_Header =__ptrNames->Head;
	_ptrExpr_Header =__ptrExprs->Head;
	
	while (_ptrExpr_Header->Next!=NULL){
		_strName=(char *)_ptrName_Header->Value;
		_strExpr=(char *)_ptrExpr_Header->Value;
		_str2Find =WrapCurlies(_strName);
		__strInput=Exp_Curly_Replace(__strInput, _str2Find, _strExpr);
		free(_str2Find);
		_ptrName_Header = _ptrName_Header->Next;
		_ptrExpr_Header = _ptrExpr_Header->Next;
	}
	return __strInput;

	
}


static struct LinkedList * Set_Name(){
	struct LinkedList *_ptrNew  = (struct LinkedList *)Create_Object(LINKEDLIST);
	int i;

	_ptrNew = _ptrNew->init(_ptrNew, sizeof(char)*BUFFER);
	for (i =0; i< REGDEF_COUNT;i++){
		_ptrNew->Add(regDef_Names[i], _ptrNew);
	}

	return _ptrNew;
}


static char * Adj_Star(char *__strInput){
	int _sIndex;
	int _matchIndex; 
	
	_sIndex = 0;
	while (__strInput[_sIndex] != '\0'){
		if (__strInput[_sIndex] !='*'){
			_sIndex ++;
		}
		else{
			_matchIndex=St_Exp_Plus_SI(__strInput, _sIndex-1, __strInput[_sIndex-1], 0);
			__strInput[_matchIndex] = '{';
			__strInput[_sIndex-1]='}';
			_sIndex ++;
		}
	}
	return __strInput;
}
struct LinkedList * Set_Expressions(struct RegDefs *__ptrInput){
	struct LinkedList *_ptrNew  = (struct LinkedList *)Create_Object(LINKEDLIST);
	int i;
	int j;
	char * _strExpand;
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;

	_ptrNew = _ptrNew->init(_ptrNew, sizeof(char)*BUFFER);

	for (i =0; i< REGDEF_COUNT;i++){
		if (i==15){
		j=1;
		}
		_strExpand =  Expand(regDef_Exprs[i]);
		_strExpand=Exp_Curly(_strExpand, _ptrInternals->Name, _ptrNew);
	//	_strExpand= Adj_Star(_strExpand);
		_ptrNew->Add(_strExpand, _ptrNew);
		
	}

	return _ptrNew;
}


static void Print(struct RegDefs *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrName_Header;
	struct LinkedListNode *_ptrExpr_Header;

	_ptrName_Header = _ptrInternals->Name->Head;
	_ptrExpr_Header = _ptrInternals->Expression->Head;

	printf("Printing RegDefs :\n");
	while (_ptrName_Header->Next !=NULL){
		printf("%-10s%-10s", (char *)_ptrName_Header->Value, (char *)_ptrExpr_Header->Value);
		_ptrName_Header =_ptrName_Header->Next;
		_ptrExpr_Header =_ptrExpr_Header->Next;
		printf("\n");
	}
	
	printf("\n");
	
	
}


static char * GetExpr(char *__strName, struct RegDefs *__ptrRegDefs){
	struct LinkedListNode *_ptrName_Head;
	struct LinkedListNode *_ptrExpr_Head;
	struct internals *_ptrInternals;
	char *_strName;

	_ptrInternals = (struct internals *)__ptrRegDefs->internals;
	_ptrName_Head = _ptrInternals->Name->Head;
	_ptrExpr_Head = _ptrInternals->Expression->Head;

	while (_ptrName_Head->Next !=NULL){
		_strName= (char *)_ptrName_Head->Value;
		if (strcmp(__strName, _strName)==0){
			return (char *) _ptrExpr_Head->Value;
		}

		_ptrName_Head  = _ptrName_Head->Next;
		_ptrExpr_Head = _ptrExpr_Head->Next;
	}

	return NULL;
}
static struct RegDefs* init(struct RegDefs *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)malloc(sizeof(struct internals));



	__ptrInput->internals = _ptrInternals;
	_ptrInternals->Name = Set_Name();
	_ptrInternals->Expression = Set_Expressions(__ptrInput);

	__ptrInput->GetExpr = GetExpr;
	__ptrInput->Print = Print;
	return __ptrInput;
}

static void * c_ctor(){
	struct  RegDefs *_ptrOutput = (struct RegDefs *)malloc(sizeof(struct RegDefs));
	_ptrOutput->init = init;
	return _ptrOutput;
}

struct  Class_Generic _REGDEFS = {c_ctor};

const void *REGDEFS = &_REGDEFS;