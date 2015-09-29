#include "General\Create_object.h"
#include "Lexical Analyzer\LexAnalyzer.h"
#include "Lexical Analyzer\LexDefs.h"
#include <stdio.h>
#include "Parser\SLRParser.h"




#define ROWCOUNT 28 //the number of productions
#define NT_COUNT 15 //the extra spaces are reserved
#define T_COUNT 22 //terminal count


//
/* GRAMMAR
S'->S
S->SF;|SW;|SA;|F;|W;|A;
F-><IF>(Expr){S}|<IF>(Expr){S}<ELSE>{S}
W-><WHILE>(Expr){S}
A->LR
L-><ID><EQ>
Expr->R<LT>R|R<LE>R|R<EQ>R|R<NE>R|R<GT>R|R<GE>R
R->T
T->T+E|T-E|E
E->E*Q|E/Q|Q
Q->(R)|<NUMBER>|<ID>
*/

//The Nonterminals are mapped as follow:
//[S',     0]
//[S,      1]
//[F,      2]
//[W,      3]
//[A,      4]
//[Expr,   5]
//[L,      6]
//[R,      7]
//[E,      8]
//[Q,      9]
//[T,     10]
//*/

int CompilerGrammar[ROWCOUNT][NT_COUNT + T_COUNT +1] = {
	//S'->S
	{1, -1},
	//S->SF
	{1, 2,  -1}, 
	//S->SW
	{1, 3, -1}, 
	//S->SA;
	{1, 4, 36, -1}, 
	//S->F
	{2,  -1}, 
	//S->W
	{3,  -1}, 
	//S->A;
	{4, 36, -1}, 

	//F-><IF>(Expr){S}
	{16, 32, 5, 33, 34, 1, 35, -1}, 
	//F-><IF>(Expr){S}<ELSE>{S}
	{16, 32, 5, 33, 34, 1, 35, 18, 34, 1, 35, -1},

	//W-><WHILE>(Expr){S}
	{27, 32, 5, 33, 34, 1, 35, -1},

	//A->LR
	{6, 7, -1}, 

	//L-><ID><EQ>
	{19, 23, -1}, 

	//Expr->R<LT>R
	{7, 21, 7, -1}, 
	//Expr->R<LE>R
	{7, 22, 7, -1}, 
	//Expr->R<EQ>R
	{7, 23, 7, -1}, 
	//Expr->R<NE>R
	{7, 24, 7, -1}, 
	//Expr->R<GT>R
	{7, 25, 7, -1},
	//Expr->R<GE>R
	{7, 26, 7, -1},

	//R->T
	{10, -1}, 

	//T->T+E
	{10, 28, 8, -1}, 
	//T->T-E
	{10, 29, 8, -1},
	//T->E
	{8, -1},

	//E->E*Q
	{8, 30, 9, -1},
	//E->E/Q
	{8, 31, 9, -1},
	//E->Q
	{ 9, -1},

	//Q->(R)
	{32, 7, 33, -1}, 
	//Q-><NUMBER>
	{20, -1}, 
	//Q-><ID>
	{19, -1}

};


//map between production rows and nonterminals
int CompilerRows[ROWCOUNT] = {0, 1, 1, 1, 1, 1, 1, 2, 2, 3, 4, 6, 5, 5, 5, 5,5, 5, 7, 10, 10, 10, 8, 8, 8, 9, 9, 9};


struct LexTokKey{
	int ID;
	enum Tokens LexType;
	enum Sub_Types SubType;
	char * LexString;
};

//this is used to map the output from the Lexical analyzer with the token ids
struct LexTokKey Keys[T_COUNT] ={
	{15, WS,NONE,  ""}, 
	{16, IF,NONE, ""},
	{17, THEN,NONE, ""},
	{18, ELSE,NONE, ""},
	{19, ID,NONE, ""},
	{20, NUMBER,NONE, ""},
	{21, RELOP, LT, ""},
	{22, RELOP, LE, ""},
	{23, RELOP, EQ, ""},
	{24, RELOP, NE, ""},
	{25, RELOP, GT, ""},
	{26, RELOP, GE, ""},
	{27, WHILE, NONE, ""},
	{28, SINGLE, NONE, "+"},
	{29, SINGLE, NONE, "-"},
	{30, SINGLE, NONE, "*"},
	{31, SINGLE, NONE, "/"},
	{32, SINGLE, NONE, "("},
	{33, SINGLE, NONE, ")"},
	{34, SINGLE, NONE, "{"},
	{35, SINGLE, NONE, "}"}, 
	{36, SINGLE, NONE, ";"}

};



//returns the ID associated with the input token. 
static int GetToken(enum Tokens __token, enum Sub_Types __subType, char *__lexString){
	int i;
	for (i = 0; i < T_COUNT; i++){
		if (__token == SINGLE && strcmp(__lexString,  Keys[i].LexString)==0){
			return  Keys[i].ID;
		}
		else if (__token != RELOP && __token != SINGLE && Keys[i].LexType ==__token){
			if (Keys[i].LexType !=WS){
				return  Keys[i].ID;
			}
			else{
				return -1;
			}
		}
		else if(__token == RELOP && Keys[i].SubType ==__subType){
			return  Keys[i].ID;
		}
	}
	return -1;
}

//converts the output from the lexical analyzer to the appropriate input for the SLRParser
int *GetTokens(){
	struct LexAnalyzer *_ptrLexAnalyzer = (struct LexAnalyzer *)Create_Object(LEXANALYZER);
	struct LinkedList *_ptrLexems;
	int *_tokens;
	struct LinkedListNode *_ptrNode;
	struct Lexem *_ptrLexem;
	int i;

	_ptrLexAnalyzer = _ptrLexAnalyzer->init(_ptrLexAnalyzer);
	_ptrLexems= _ptrLexAnalyzer->Analyze(_ptrLexAnalyzer);
	_tokens = (int *)malloc(sizeof(int)*_ptrLexems->Count(_ptrLexems)+2);

	_ptrNode = _ptrLexems->Head;
	i =0;
	while (_ptrNode->Next !=NULL){
		_ptrLexem = (struct Lexem *)_ptrNode->Value;
		_tokens[i] =  GetToken(_ptrLexem->Type, _ptrLexem->SubType, _ptrLexem->Value);
		if (_tokens[i] != -1){
			i++;
		}
		
		_ptrNode = _ptrNode->Next;	
	}
	_tokens[i] = NT_COUNT +T_COUNT +1;
	_tokens[i+1] = -1;
	return _tokens;
}


void PrintTokens(int *_tokens){
	int i;
	i =0;
	while (_tokens[i]!=-1){
		printf("%d\n", _tokens[i]);
		i++;
	}
}

//convert the input 2d array to a 2d pointer
int **MatToP(int __input[][NT_COUNT +T_COUNT +1]){
	int i;
	int j;
	int **_output;

	_output = (int **)malloc(sizeof(int *)*ROWCOUNT);
	for (i = 0; i < ROWCOUNT; i++){
		_output[i] = (int *)malloc(sizeof(int)*(NT_COUNT +T_COUNT +1));
	}
	for (i =0;i <ROWCOUNT ; i++){
		for (j =0; j < NT_COUNT +T_COUNT +1 ;j++){
			_output[i][j] = __input[i][j];
		}
	}
	return _output;
}
void PrintTN(void *_Value){
	printf("%d\n", *(int *)_Value);
}

//calls the lexical analyzer based on the input file. Then runs the parser
int main(){
	int *_tokens;
	struct LexAnalyzer *_ptrLexAnalyzer = (struct LexAnalyzer *)Create_Object(LEXANALYZER);
	struct SLRParser *_SLRParser;
	int **_grammar;
	struct ParseTree *_ptrParseTree;
	
	_tokens = GetTokens();
	PrintTokens(_tokens);
	_SLRParser = (struct SLRParser *)Create_Object(SLRPARSER);
	_grammar = MatToP(CompilerGrammar);
	_SLRParser = _SLRParser->init(_SLRParser, _grammar, CompilerRows, ROWCOUNT, NT_COUNT, T_COUNT);
	_ptrParseTree=_SLRParser->Parse(_SLRParser, _tokens);
	_ptrParseTree->PrintTree(_ptrParseTree,PrintTN);

}