#ifndef STACK_H
#define STACK_H

#include <Windows.h>

extern const void * STACK;

struct Stack{
	void *internals;
	void (*Push)(struct Stack *__ptrStack, void * __newValue);
	void *(*Pop)(struct Stack *__ptrStack);
	//read top item without popint
	void *(*Read)(struct Stack *__ptrStack);
	struct Stack *(*init)(struct Stack *__ptrInput, size_t __size);
};
#endif STACK_H

