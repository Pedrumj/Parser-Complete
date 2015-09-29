#ifndef REG_DEF_H
#define REG_DEF_H
#include "..\General\LinkedList.h"
#include "..\General\LinkedList.h"
extern const void * REGDEFS;

struct RegDefs{
	void *internals;
	void (*Print)(struct RegDefs *__ptrInput);
	struct RegDefs * (*init)(struct RegDefs *__ptrInput);
	char * (*GetExpr)(char *__strName, struct RegDefs *__ptrRegDefs);

};

#endif REG_DEF_H