#include "..\Parser\SLRParser.h"
#include "LexDefs.h"
#include "..\General\Class_Generic.h"
#include <stdio.h>
#include "..\General\String.h"
#include "..\General\LinkedList.h"
#include "..\General\Create_object.h"

#define BUFFER 10000
#define ROW_COUNT 86
#define NONTERMINA_COUNT 4
#define TERMINAL_COUNT 82
#define COLUMN_COUNT 7

struct internals{
	struct SLRParser *SLRParser;
	char *Name;
};

/*
NONTERMINALS
[S'    0]
[S     1]
[F     2]
[G     3] //reserved

NONTERMINALS
*/

struct Terminal{
	char *String;
	int Token;
	int StringLen;
};

struct Production{
	int Items[COLUMN_COUNT];
	enum LexNodeTypes NodeType;
};

struct Terminal  Terminals[TERMINAL_COUNT] = {{"*", 4,  1}, //operator
											{"(", 5,  1}, 
											{")", 6,  1}, 
											{"|", 7,  1}, 
											{" ", 8,  1}, 
											{"\t", 9,  1}, 
											{"\n", 10,  1}, 
											{"\\*", 11,  2}, //literal * sign
											{"\\(", 12,  2}, 
											{"\\)", 13,  2}, 
											{"\\|", 14,  2}, 
											{"a", 15,  1}, 
											{"b", 16,  1}, 
											{"c", 17,  1}, 
											{"d", 18,  1}, 
											{"e", 19,  1}, 
											{"f", 20,  1}, 
											{"g", 21,  1}, 
											{"h", 22,  1}, 
											{"i", 23,  1}, 
											{"j", 24,  1}, 
											{"k", 25,  1}, 
											{"l", 26,  1}, 
											{"m", 27,  1}, 
											{"n", 28,  1}, 
											{"o", 29,  1}, 
											{"p", 30,  1}, 
											{"q", 31,  1}, 
											{"r", 32,  1}, 
											{"s", 33,  1}, 
											{"t", 34,  1}, 
											{"u", 35,  1}, 
											{"v", 36,  1}, 
											{"w", 37,  1}, 
											{"x", 38,  1}, 
											{"y", 39,  1}, 
											{"z", 40,  1}, 
											{"A", 41,  1}, 
											{"B", 42,  1}, 
											{"C", 43,  1}, 
											{"D", 44,  1}, 
											{"E", 45,  1}, 
											{"F", 46,  1}, 
											{"G", 47,  1}, 
											{"H", 48,  1}, 
											{"I", 49,  1}, 
											{"J", 50,  1}, 
											{"K", 51,  1}, 
											{"L", 52,  1}, 
											{"M", 53,  1}, 
											{"N", 54,  1}, 
											{"O", 55,  1}, 
											{"P", 56,  1}, 
											{"Q", 57,  1}, 
											{"R", 58,  1}, 
											{"S", 59,  1}, 
											{"T", 60,  1}, 
											{"U", 61,  1}, 
											{"V", 62,  1}, 
											{"W", 63,  1}, 
											{"X", 64,  1}, 
											{"Y", 65,  1}, 
											{"Z", 66,  1}, 
											{"0", 67,  1}, 
											{"1", 68,  1}, 
											{"2", 69,  1}, 
											{"3", 70,  1}, 
											{"4", 71,  1}, 
											{"5", 72,  1}, 
											{"6", 73,  1}, 
											{"7", 74,  1}, 
											{"8", 75,  1}, 
											{"9", 76,  1}, 
											{"\\e", 77,  2}, //epsilon
											{"\\+", 78,  2}, //literal + sign
											{"\\-", 79,  2}, //literal - sign
											{"\\.", 80,  2}, //literal . sign
											{"=", 81,  1}, 
											{">", 82,  1},
											{"<", 83,  1}, 
											{"{", 84,  1}, 
											{"}", 85,  1}
};

							//S'->S
struct Production MatGrammar[ROW_COUNT] = {{{1, -1},INIT} , 
							//S->(S)*
							{{5, 1, 6, 4,   -1},STAR},  
							//S->S|F
							{{  1, 7, 2 ,  -1}, OR},
							//S->(S)
							{{ 5, 1, 6,  -1}, EXTRA}, 
							//S->S(S)
							{{ 1, 5, 1, 6, -1}, CONCATE}, 
							//S->S(S)*
							{{ 1, 5, 1, 6, 4,  -1}, CONSTAR}, 
							//S->S|(S)
							{{1, 7,  5, 1, 6, -1}, OR}, 
							//S->F
							{{2, -1},EXTRA}, 
							//[     8]space
							{{8 ,-1},EXTRA}, 
							//[\t    9]
							{{9 ,-1},EXTRA}, 
							//[\n   10]
							{{10 ,-1},EXTRA}, 
							//[\\*  11] literal
							{{11 ,-1},EXTRA}, 
							//[\\(  12] literal
							{{12 ,-1},EXTRA}, 
							//[\\)  13] literal
							{{13 ,-1},EXTRA}, 
							//[\\|  14] literal
							{{14 ,-1},EXTRA}, 
							//F->'a', 'b', 'c', ...
							{{15, -1 },EXTRA}, 
							{{16, -1 },EXTRA}, 
							{{17, -1 },EXTRA}, 
							{{18, -1 },EXTRA}, 
							{{19, -1 },EXTRA}, 
							{{20, -1 },EXTRA}, 
							{{21, -1 },EXTRA}, 
							{{22, -1 },EXTRA}, 
							{{23, -1 },EXTRA}, 
							{{24, -1 },EXTRA}, 
							{{25, -1 },EXTRA}, 
							{{26, -1 },EXTRA}, 
							{{27, -1 },EXTRA}, 
							{{28, -1 },EXTRA}, 
							{{29, -1 },EXTRA}, 
							{{30, -1 },EXTRA}, 
							{{31, -1 },EXTRA}, 
							{{32, -1 },EXTRA}, 
							{{33, -1 },EXTRA}, 
							{{34, -1 },EXTRA}, 
							{{35, -1 },EXTRA}, 
							{{36, -1 },EXTRA}, 
							{{37, -1 },EXTRA}, 
							{{38, -1 },EXTRA}, 
							{{39, -1 },EXTRA}, 
							{{40, -1 },EXTRA}, 
							{{41 , -1},EXTRA}, 
							{{42 , -1},EXTRA}, 
							{{43 , -1},EXTRA}, 
							{{44 , -1},EXTRA}, 
							{{45 , -1},EXTRA}, 
							{{46 , -1},EXTRA}, 
							{{47 , -1},EXTRA}, 
							{{48 , -1},EXTRA}, 
							{{49 , -1},EXTRA}, 
							{{50 , -1},EXTRA}, 
							{{51 , -1},EXTRA}, 
							{{52 , -1},EXTRA}, 
							{{53 , -1},EXTRA}, 
							{{54 , -1},EXTRA}, 
							{{55 , -1},EXTRA}, 
							{{56 , -1},EXTRA}, 
							{{57 , -1},EXTRA}, 
							{{58 , -1},EXTRA}, 
							{{59 , -1},EXTRA}, 
							{{60 , -1},EXTRA}, 
							{{61 , -1},EXTRA}, 
							{{62 , -1},EXTRA}, 
							{{63 , -1},EXTRA}, 
							{{64 , -1},EXTRA}, 
							{{65 , -1},EXTRA}, 
							{{66 , -1},EXTRA}, 
							{{67 , -1},EXTRA}, 
							{{68 , -1},EXTRA}, 
							{{69 , -1},EXTRA}, 
							{{70 , -1},EXTRA}, 
							{{71 , -1},EXTRA}, 
							{{72 , -1},EXTRA}, 
							{{73 , -1},EXTRA}, 
							{{74 , -1},EXTRA}, 
							{{75 , -1},EXTRA}, 
							{{76 , -1},EXTRA}, 
							{{77, -1},EXTRA}, 
							{{78, -1},EXTRA}, 
							{{79, -1},EXTRA}, 
							{{80, -1},EXTRA}, 
							{{81, -1},EXTRA}, 
							{{82, -1},EXTRA}, 
							{{83, -1}, EXTRA},
							{{84, -1}, EXTRA},
							{{85, -1}, EXTRA}
};

int arrMap[ROW_COUNT] = {0, 
					1, 
				    1,
				    1,
				    1,
				    1, 
				    1,
				    1,
				   2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2,
				  2, 
				  2,
				  2,
				  2,
				  2,
				  2,
				   2,
				  2,
				  2,
				  
				  2
};

void SLR2LexTreeAux (struct ParseTreeNode *__ptrONode, struct ParseTreeNode *__ptrNNode, struct ParseTree *__ptrNTree);

void PrintTF(void *__ptrValue){
	printf("%d\n",*(int *)__ptrValue);
}

char *NodeType2String(enum LexNodeTypes __nodeType){
	switch (__nodeType){
	case 0:
		return "CONCATE";
	case 1:
		return "LITERAL";
	case 2:
		return "STAR";
	case 3:
		return "OR";
	case 4:
		return "INIT";
	case 5:
		return "EXTRA";
	}

	return NULL;
}
void PrintLex(void *__ptrValue){
	 struct LexTreeNode *_lexNode;

	_lexNode = (struct LexTreeNode *)__ptrValue;
	if (_lexNode->NodeType ==LITERAL){
		printf("%-5s%-5s\n",_lexNode->Value,NodeType2String(_lexNode->NodeType));
	}
	else{
		printf("%-5s\n",NodeType2String(_lexNode->NodeType));
	}
	
}

static void Print(struct LexTree *__ptrInput){
	__ptrInput->ParseTree->PrintTree(__ptrInput->ParseTree, PrintLex);

}


 //convers a 2d matrix to a 2d pointer
int **Struct2DyMat(struct Production __matInput[ROW_COUNT]){
	int **_matOutput = (int **)malloc(sizeof (int *)*ROW_COUNT);
	int i;
	
	for (i = 0; i< ROW_COUNT; i++){
		_matOutput[i] = (int *)malloc(sizeof(int)*COLUMN_COUNT);	
		memcpy(_matOutput[i],__matInput[i].Items, sizeof(int)*COLUMN_COUNT);
	}
	return _matOutput;
}

//Receives as input a string and an index, returns the token at that index .The tokens are defined at the top of this file
int GetToken(char *__strLexDef, int *__SIndex){
	int i;
	int j;
	int IsMatch;
	for (i =0; i<TERMINAL_COUNT;i++){
		IsMatch =1;
		for (j =0; j<Terminals[i].StringLen; j++){
			if (__strLexDef[*__SIndex+j]!=Terminals[i].String[j]){
				IsMatch =0;
				j = Terminals[i].StringLen;
			}
		}
		if (IsMatch==1){
			(*__SIndex)+=Terminals[i].StringLen;
			return Terminals[i].Token;
		}
	}

	return -1;
} 

//Receives as input a token and returns the associated string. The tokens are defined at the top of this file
char *GetStrFroTok(int __token){
	int i;
	char *_output;

	_output = (char *)malloc(sizeof(char)*1000);
	for (i =0; i<TERMINAL_COUNT;i++){
		if (Terminals[i].Token== __token){
			if (Terminals[i].StringLen ==1){
				return Terminals[i].String;
			}
			else if(i ==73){
				return "\0";
			
			}
			else if (Terminals[i].StringLen ==2){
				cpy_mid(_output, Terminals[i].String, 1, 2);
				return _output;
			}
			
		}
	}
	return NULL;
}


//Receives as input a text string, and returns an array of tokens. The tokens are defined at the top of this file
int *str2Tokens(char *__strLexDef, int *__count){
	int *_ptrOutput;
	int i;
	int _strIndex;
	int j;
	(*__count) =0;
	_ptrOutput = (int *)malloc(sizeof(int) *strLen(__strLexDef));
	i =0;
	_strIndex =0;
	while (__strLexDef[_strIndex]!='\0'){
		_ptrOutput[i] =  GetToken(__strLexDef, &_strIndex);
		if (_ptrOutput[i]==4){
			j =9;
		}
		(*__count)++;
		i++;
	}
	_ptrOutput[i] = NONTERMINA_COUNT+TERMINAL_COUNT+1;
	return _ptrOutput;
}

//Gets the production number associated with __ptrONode. Note that this is hardcoded. Changing the grammare would require
//changes in this function. However the Lexical anaylzyer grammar is not intended to be changed. 
static int GetProduction (struct ParseTreeNode *__ptrONode){
	struct ParseTreeNode *_ptrFirstChild;
	struct ParseTreeNode *_ptrChild;
	int i;
	int j;
	int _flagMatch;
	int _flagGo;
	_ptrFirstChild = __ptrONode->FirstChild;
	for (i = 0; i < ROW_COUNT; i++){
		_ptrChild = _ptrFirstChild;
		j =0;
		_flagMatch = 1;
		_flagGo =1;
		if (*(int *)_ptrChild->Value!= MatGrammar[i].Items[j]){
			_flagMatch =0;
			_flagGo =0;
		}
		if (_ptrChild->Next ==NULL){
			_flagGo = 0;
			if (MatGrammar[i].Items[j+1]!=-1){
				_flagMatch = 0;
			}
		}
		while (_flagGo ==1){
			j ++;
			_ptrChild = _ptrChild->Next;
			if (*(int *)_ptrChild->Value!= MatGrammar[i].Items[j]){
				_flagMatch =0;
				_flagGo =0;
			}
			if (_ptrChild->Next ==NULL){
				_flagGo = 0;
				if (MatGrammar[i].Items[j+1]!=-1){
					_flagMatch = 0;
				}
			}
		}
		if (_flagMatch ==1){
			return i;
		}
	}
	return -1;
}


//Adds an S node to __ptrNNode
struct ParseTreeNode * SetChild_S(struct ParseTreeNode *__ptrONode, struct ParseTreeNode *__ptrNNode, struct ParseTree *__ptrNTree){
	struct LexTreeNode *_ptrNew;
	int _production;
	int _nonTerminal;
	struct ParseTreeNode *_ptrNode;

	_nonTerminal = *(int *)__ptrONode->Value;
	_production = GetProduction(__ptrONode);
	_ptrNew = (struct LexTreeNode *)malloc(sizeof(struct LexTreeNode ));
	_ptrNew->NodeType = MatGrammar[_production].NodeType;
	if (_ptrNew->NodeType ==LITERAL){
		_ptrNew->NodeType = LITERAL;
	}
	if (__ptrNTree->ChildCount(__ptrNNode)==0){
		return __ptrNTree->Add_Ch(__ptrNTree, __ptrNNode, _ptrNew);
	}
	else{
		_ptrNode = __ptrNNode->FirstChild;
		while (_ptrNode->Next!=NULL){
			_ptrNode = _ptrNode->Next;
		}
		return __ptrNTree->Add_Next(__ptrNTree, _ptrNode, _ptrNew);
	}
	

		
	
}


//Adds an F node to __ptrNNode. This node contans a literal character
void SetChild_F(struct ParseTreeNode *__ptrONode, struct ParseTreeNode *__ptrNNode, struct ParseTree *__ptrNTree){

	struct LexTreeNode *_ptrLexTreeNode;
	int _FValue;

	struct ParseTreeNode *_ptrNode;
	struct LexTreeNode *_ptrLexTemp;

	_ptrLexTreeNode = (struct LexTreeNode *)malloc(sizeof(struct LexTreeNode));
	_FValue =*(int *)__ptrONode->FirstChild->Value;
	_ptrLexTreeNode->Value  = GetStrFroTok(_FValue);
	_ptrLexTreeNode->NodeType = LITERAL;
	if (__ptrNTree->ChildCount(__ptrNNode)==0){
		__ptrNTree->Add_Ch(__ptrNTree, __ptrNNode, _ptrLexTreeNode);
		_ptrLexTemp = (struct LexTreeNode *)__ptrNNode->Value;
	}
	else{
		_ptrNode = __ptrNNode->FirstChild;
		while (_ptrNode->Next!=NULL){
			_ptrNode = _ptrNode->Next;
		}
		__ptrNTree->Add_Next(__ptrNTree, _ptrNode, _ptrLexTreeNode);
	}
}


//Adds a Start node to __ptrNNode
void Con_Star_Aux(struct ParseTreeNode *__ptrONode, struct ParseTreeNode *__ptrNNode, struct ParseTree *__ptrNTree){
	//S->S(S)*

	//the child node of the original parse tree that will be added
	struct ParseTreeNode *_ptrChild;
	struct LexTreeNode *_ptrLexNode;

	((struct LexTreeNode *)__ptrNNode->Value)->NodeType = CONCATE;
	//S
	_ptrChild = __ptrONode->FirstChild;
	SLR2LexTreeAux(_ptrChild, __ptrNNode, __ptrNTree);
	_ptrLexNode = (struct LexTreeNode *)malloc(sizeof(struct LexTreeNode));
	_ptrLexNode->NodeType = STAR;
	__ptrNTree->Add_Next(__ptrNTree, __ptrNNode->FirstChild, _ptrLexNode);
	SLR2LexTreeAux(_ptrChild->Next->Next, __ptrNNode->FirstChild->Next, __ptrNTree);

}


//A recursive function used to convert the SLR Parse tree to the syntax tree used in the book.
//__ptrONode: This is the a node in the SLR Parse Tree. All subnodes will be convered to the correct parse tree
//__ptrNNode: The current node in the new tree were additional nodes should be added to
//__ptrNTree: The new parse tree
void SLR2LexTreeAux (struct ParseTreeNode *__ptrONode, struct ParseTreeNode *__ptrNNode, struct ParseTree *__ptrNTree){
		int _nonTerminal;
		int _production;
		struct LexTreeNode *_ptrNew;
		struct ParseTreeNode *_ptrChild;
		struct ParseTreeNode *_ptrNewNode;
		int i;

		_nonTerminal = *(int *)__ptrONode->Value;
		_production = GetProduction(__ptrONode);
		_ptrNew = (struct LexTreeNode *)malloc(sizeof(struct LexTreeNode ));
		_ptrNew->NodeType = MatGrammar[_production].NodeType;

		if (_ptrNew->NodeType != EXTRA){
			_ptrNewNode= SetChild_S(__ptrONode, __ptrNNode, __ptrNTree);
		}
		else{
			_ptrNewNode = __ptrNNode;
		}
		if (_ptrNew->NodeType == CONSTAR){
			 Con_Star_Aux(__ptrONode, _ptrNewNode,__ptrNTree );
			 return;
		}
		_ptrChild = __ptrONode->FirstChild;

		if (*(int *)_ptrChild->Value ==2){
			SetChild_F(_ptrChild, _ptrNewNode, __ptrNTree);
		}
		else if(*(int *)_ptrChild->Value ==1){
			SLR2LexTreeAux(_ptrChild, _ptrNewNode, __ptrNTree);
		}

		i =0;
		while (_ptrChild->Next !=NULL){
			_ptrChild = _ptrChild->Next;
			 i++;
			 if (*(int *)_ptrChild->Value ==2){
				SetChild_F(_ptrChild, _ptrNewNode, __ptrNTree);
			}
			else if(*(int *)_ptrChild->Value ==1){
				SLR2LexTreeAux(_ptrChild, _ptrNewNode, __ptrNTree);
			}

		}

}

//The parse tree generated by the SLR parser is slightly different than the parse tree used in the book.
//This function modifies the parse tree so it becomes the same as the one in the book. Also each node
//has an attribute stating which type it is. The types can be found LexTree.h
struct ParseTree *SLR2LexTree (struct ParseTree *__ptrParseTree){
	struct ParseTree*_ptrOutput;
	struct LexTreeNode *_ptrLexTreeNode;

	_ptrOutput = (struct ParseTree *)Create_Object(PARSETREE);
	_ptrOutput = _ptrOutput->init(_ptrOutput, sizeof(struct LexTreeNode));
	_ptrLexTreeNode =			(struct LexTreeNode *)malloc(sizeof(struct LexTreeNode ));
	_ptrLexTreeNode->NodeType = INIT;
	_ptrOutput->Head->Value = _ptrLexTreeNode;
	SLR2LexTreeAux(__ptrParseTree->Head->FirstChild, _ptrOutput->Head, _ptrOutput);
	return _ptrOutput;  
}




static struct LexTree * init(struct LexTree *__ptrInput, char *__strLexDef){
	struct internals *_ptrInternals = (struct internals *)malloc(sizeof(struct internals));
	int **_matGrammar;
	int *_ptrTokens;

	int _count;
	struct ParseTree *_ptrParseTree;
	_ptrInternals->SLRParser = (struct SLRParser *)Create_Object(SLRPARSER);

	_matGrammar =  Struct2DyMat(MatGrammar);
	_ptrInternals->SLRParser = _ptrInternals->SLRParser->init(_ptrInternals->SLRParser, _matGrammar, arrMap, ROW_COUNT, 
		NONTERMINA_COUNT, TERMINAL_COUNT);

	_ptrTokens =  str2Tokens(__strLexDef, &_count);
	_ptrParseTree=_ptrInternals->SLRParser->Parse(_ptrInternals->SLRParser, _ptrTokens);
	__ptrInput->ParseTree = SLR2LexTree(_ptrParseTree);
	return __ptrInput;
}

static void * c_ctor(){
	struct  LexTree *_ptrOutput = (struct LexTree *)malloc(sizeof(struct LexTree));
	_ptrOutput->init = init;
	_ptrOutput->Print = Print;
	return _ptrOutput;
}

struct  Class_Generic _LEXTREE = {c_ctor};

const void *LEXTREE = &_LEXTREE;