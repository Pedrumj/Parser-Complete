#include "Create_object.h"
#include "Class_Generic.h"
void *Create_Object(const void *_objecType){
	const struct Class_Generic *_ptrClass = (const struct Class_Generic *)_objecType;
	return _ptrClass->c_ctor();
}
