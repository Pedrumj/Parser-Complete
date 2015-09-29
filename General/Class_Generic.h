#ifndef	CLASS_GENERIC_H
#define	CLASS_GENERIC_H

struct Class_Generic{
	//c_ctor must assign the (*init) function of the struct to it
	void * (*c_ctor)();
};

#endif CLASS_GENERIC_H
