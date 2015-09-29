#ifndef PARSETABLE_H
#define PARSETABLE_H

#include "Automaton.h"


enum EnumActions{
 SHIFT,
 REDUCE,
 ACCEPT,
 GOTO ,
 ERROR_A 
};

extern const void *PARSETABLE;

struct ParseTable{
	void *internals;

	struct ParseTable * (*init)(struct ParseTable *__ptrInput, int **__Grammer, 
		int *__Rows, int __countRows, int __countNonterminals, int __countTerminals);
	//determins what to do based on the input paramters (i.e SHIFT, REDUCE, ...)
	void (*NextAction)(struct ParseTable *__ptrParseTable, int __nextToken, int __state, 
		int *__nextState, enum EnumActions *__actionType, int *__reduceNTer, int *__reducePoC);
	//determins what to do based on the input parameters. This is when a reduce was determined by the NextAction function.
	//After the poping is finished the nonterminal is checked in the Goto table.
	void (*NextGoto)(struct ParseTable *__ptrParseTable, int __state, int *__nextState, int __reduceNTer);
};





#endif PARSETABLE_H