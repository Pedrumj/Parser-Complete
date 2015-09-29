#ifndef CLOSURE_H
#define CLOSURE_H

extern const void *CLOSURE;


 struct ItemInClosure{
	 // production number (rows of grammar matrix)
	int intProduction;
	//dot location in the production. starts from zero
	int intDot;
	int IsKernel;
};
struct Closure{
	
	struct LinkedList *Items;
	//closure state. starts at zero
	int State;
	//outgoing edges
	struct LinkedList *Links;
	struct Closure * (*init)(struct Closure *_ptrInput, const struct Grammar * const __ptrGrammar, int __state);
	void (*Print)(struct Closure *_ptrInput);
	//if the input item is in the closure and is a kernel in the closure returns "1", else "0"
	int (*MatchKernelItem)(struct Closure *__ptrInput, struct ItemInClosure *__ptrItem2Find);
	//adds the input production to the closure
	int (*AddItemToClosure)(struct Closure *_ptrInput, int __intProduction, int __intDot, int __IsKernel);
	//returns a linked list of items from the closure that have the dot at the end of the production
	struct LinkedList * (*ItemsDotEnd)(struct Closure *__ptrInput);
	void *internals;
	void (*GenerateClosure)(struct Closure *_ptrInput);
};



 struct Link{
	 //outgoing terminal or nontermianl
	int Token;
	//is the link going to the accept state
	int IsAccept;
	//the item resulting from adding the token to the closure
	struct ItemInClosure *NewItem;
	//the next closure
	struct Closure * NextClosure;
 };

#endif CLOSURE_H