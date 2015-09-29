#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "Parser_Include.h"
#include "Grammar.h"


extern const void *AUTOMATON;

struct Automaton{
	//private members
	void *internals;
	struct Automaton *(*init)(struct Automaton *__ptrInput, struct Grammar* __ptrGrammar );
	void (*Print)(struct Automaton *__ptrInput);
	//Linked list of closures in automaton
	struct LinkedList *Closures;
};

#endif AUTOMATON_H