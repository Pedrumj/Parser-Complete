#ifndef PARSETREE_H
#define PARSETREE_H
#include <Windows.h>


extern const void * PARSETREE;


struct ParseTreeNode{
	//parent node
	struct ParseTreeNode *Parent;
	//next sibling
	struct ParseTreeNode *Next;
	//first child
	struct ParseTreeNode *FirstChild;

	void * Value;
	
	
};



struct ParseTree{
	//parent node
	struct ParseTreeNode *Head;
	void *internals;

	//Add a child to the input node. (It is assumed the node does not have any childs)
	struct ParseTreeNode * (*Add_Ch)(struct ParseTree *__ParseTree, struct ParseTreeNode *__Node, void *__newValue);	
	//Add parent to the input node. Push current node and all following sibling down to become the child of the new node
	struct ParseTreeNode * (*Add_PSh)(struct ParseTree *__ParseTree,struct ParseTreeNode *__Node, void *__newValue);
	//not used
	struct ParseTreeNode * (*Add_Root)(struct ParseTree *__ParseTree,struct ParseTreeNode *__Node, void *__newValue);
	//Add node as next sibling of the input node
	struct ParseTreeNode * (* Add_Next)(struct ParseTree *__ParseTree,struct ParseTreeNode *__Node, void *__newValue);

	//creates an empty node. Used to create the root
	struct ParseTree *(*init)(struct ParseTree *__ParseTree, size_t __size);	
	//prints the tree
	void (*PrintTree)(struct ParseTree *__ptrHeader, void (*Print)(void *__ptrValue));
	struct ParseTreeNode *(*GetPrevious)(struct ParseTreeNode*__Node);
	int (*ChildCount)(struct ParseTreeNode *__ptrNode);
};


#endif PARSETREE_H


