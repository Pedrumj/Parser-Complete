#ifndef	GRAMMAR_H
#define	GRAMMAR_H

#include "../Lexical Analyzer/LexDefs.h"	
#include "Parser_Include.h"
#include "First.h"


extern const void * GRAMMAR;



struct Grammar{
	void *internals;
	struct First *First;
	struct Follow *Follow;
	//A map between production numbers and nonterminals. Row[0] is the nontemrinal associated with production number "0"
	int *Rows;
	//the end marker token "$"
	int End_Marker;

	struct Grammar * (*init)(struct Grammar *__ptrGrammar, int **__Grammer, int *__Rows, int __countRows, int __countNonterminals, int __countTerminals);
	void (*Print)(struct Grammar *__ptrGrammar);
	//returns a linked list of integer. Each item is a token of the input production
	struct LinkedList *(*GetProduction)(const struct Grammar *__ptrGrammar, int __intProduction);
	//returns a linked list of integers. Each item is a row, where the production in that row is associated with the input nonterminal
	struct LinkedList *(*GetRows)(const struct Grammar *__ptrGrammar, int __intNonTerminal);
	//Returns the next item after the dot in the production. Returns -1 if the dot is at the end of the production
	int (*GetItemAfterDot)(const struct Grammar *__ptrGramamr, int __intProduction, int __intDot);
	//Returns 1 if the input item is a nonterminal. Returns -1 otherwise
	int (*IsNonTerminal)(const struct Grammar *__ptrGramamr, int __intItem);
	//Returns 1 if the input item is a terminal. Returns -1 otherwise
	int (*IsTerminal)(const struct Grammar *__ptrGramamr, int __intItm);
	//returns 1 if the input item is an epsilon. Returns -1 otherwise
	int (*IsEpsilon)(int __intItem, struct Grammar * __ptrGrammar);
	//Returns 1 if the input item is an end marker. Returns -1 otherwise
	int (*IsEndMarker)(int __intItem, struct Grammar * __ptrGrammar);
	//Returns the number of tokens in of the input production. 
	int (*ProTokCount)(struct Grammar *__ptrGrammar, int __intProd);
};

#endif	GRAMMAR_H
