#ifndef SLRPARSER_H
#define SLRPARSER_H
//#include "Additional Source Files\Parse-Tree-2\ParseTree.h"

#include "ParseTable.h"

extern const void *SLRPARSER;

struct SLRParser{
	void *internals;
	struct SLRParser * (*init)(struct SLRParser *__ptrInput, int **__Grammar, int *__Rows, int __countRows,	int __countNonterminals, int __countTerminals);
	struct ParseTree * (*Parse)(struct SLRParser *__ptrParser, int *__tokens);
	 
	//void (*Print)(struct ParseTable *__ptrInput);

};


#endif SLRPARSER_H