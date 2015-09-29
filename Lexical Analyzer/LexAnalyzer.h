#ifndef LEXANALYZER_H
#define LEXANALYZER_H

#include "..\General\LinkedList.h"


extern const void * LEXANALYZER;
struct Lexem{
	//the token type. Defined in LexDefs.h
	enum Tokens Type;
	//some types such as RELOP have a subtype. Defined in LexDefs.h
	enum Sub_Types SubType;
	//The text string associate with the lexem
	char *Value;
};

struct LexAnalyzer{
	void *internals;
	struct LexAnalyzer * (*init)(struct LexAnalyzer *__ptrInput);
	//returns linked list of Lexem structs
	struct LinkedList * (*Analyze)(struct LexAnalyzer *__ptrInput);

};



#endif LEXANALYZER_H

