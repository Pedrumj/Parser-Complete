#ifndef STRING_H
#define STRING_H

#include <Windows.h>
#define BUFFER 10000

//Gets reg def name from line
int Sta_Name(char *__strLine, char *__strName, int __index);


//passes white space between name and expression
int Sta_White(char *__strLine, int __index);


//gets the length of the input char *
int strLen(char *__strSrc);

//Starts copying from __startIndex till the end of the string
char *cpy_right(char *__strDest, char *__strSrc, int __startIndex);


//copies string from __sIndex to __eIndex
 char *cpy_mid(char *__strDest, char *__strSrc, int __sIndex, int __eIndex);



//removes __count characters from the right of __strInput
char *rm_right(char *__strInput, int __count);


//replaces the first occurance of  __str2Find in __strInput with __str2Replace
char *str_Replace(char *__strInput, char *__str2Find, char *__str2Replace);


#endif STRING_H