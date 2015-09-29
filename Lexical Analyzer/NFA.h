#ifndef NFA_H
#define NFA_H



extern const void * NFA_CLASS;

struct NFA_Class{
	void *internals;
	struct NFA_Class * (*init)(struct NFA_Class *__ptrInput, struct LinkedList *__ptrLexTrees);
	int (*GetLex)(struct NFA_Class *__ptrInput, char *__strInput, int *__Index);
	struct NFA *NFA;
};

struct NFA_Node{
	//a linked list of char *. Each link represents the outgoing character. 
	struct LinkedList *Links;
	//a linked list of NFA nodes that will be reached with the outgoing Links. 
	struct LinkedList *Nodes;
	//points to the first NFA_Node
	struct NFA_Node *First;
	//points to the accepting node of the NFA
	struct NFA_Node *Final;
	//state number. Unique
	int State;
	//determines if this is the accepting state
	int IsFinal;
};



#endif NFA_H