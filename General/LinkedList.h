#ifndef LINKEDLIST_H
#define LINKEDLIST_H
extern const void *LINKEDLIST;


struct LinkedListNode{
	void * Value;
	struct LinkedListNode *Next;

};


struct LinkedList{
	struct LinkedListNode *Head;
	struct LinkedList * (*init)(struct LinkedList *_ptrInput, int Size);
	void (*Add)(void * __Value, struct LinkedList * const __ptrList);
	void (*AddRef)( void * __ptrValue, struct LinkedList *__ptrList);
	//attemps to add a value to the linked list. Will only add if the value does not already exist in the production. Returns -1
	// if the attemp failed
	int (*AddUnique)(void *__Value, struct LinkedList * const __ptrList, int (*Compare)(void *__ptr1, void *__ptr2));
	void (*Print)(struct LinkedList *__ptrList, void (*Print)(void *__ptrValue));
	int (*Count)(struct LinkedList *__ptrList);
	//The size in bits of the valuues being stored by the linked list
	int Size;
};






#endif LINKEDLIST_H