#ifndef STRING_H
#define STRING_H
#include "String.h"
#include <Windows.h>
#define BUFFER 10000

//Gets reg def name from line
 int Sta_Name(char *__strLine, char *__strName, int __index){
	
	if (__strLine[__index] == ' '){
		strcpy_s(__strName,  BUFFER, __strLine);
		__strName[__index ] = '\0';
		return __index;
	}
	else{
		return Sta_Name(__strLine, __strName, __index +1);
	}


}


//passes white space between name and expression
 int Sta_White(char *__strLine, int __index){
	if (__strLine[__index] != ' '){
		return __index;
	}
	else{
		return Sta_White(__strLine, __index +1);
	}
	
}


//gets the length of the input char *
 int strLen(char *__strSrc){
	int _index = 0;
	while (__strSrc[_index]!='\0'){
		
		_index ++;

	} 

	
	return _index;
}


//Starts copying from __startIndex till the end of the string
 char *cpy_right(char *__strDest, char *__strSrc, int __startIndex){
	int _index = 0;
	while (__strSrc[__startIndex]!='\0'&& __strSrc[__startIndex]!='\n'){
		__strDest[_index] = __strSrc[__startIndex];
		_index ++;
		__startIndex ++;
	} 

	__strDest[_index] = '\0';
	return __strDest;
}


//copies string from __sIndex to __eIndex
 char *cpy_mid(char *__strDest, char *__strSrc, int __sIndex, int __eIndex){
	int _index = 0;
	int i;
	for (i = __sIndex; i <= __eIndex; i++){
		__strDest[_index] = __strSrc[i];
		_index ++;

	}
	__strDest[_index] = '\0';
	return __strDest;
}



//removes __count characters from the right of __strInput
 char *rm_right(char *__strInput, int __count){
	int _index = 0;
	int i;
	while (__strInput[_index]!='\0'){
		_index ++;
	} 
	for (i =0; i < __count ;i++){
		_index --;
	}
	__strInput[_index] = '\0';
	return __strInput;
}

 
//replaces the first occurance of  __str2Find in __strInput with __str2Replace
 char *str_Replace(char *__strInput, char *__str2Find, char *__str2Replace){
 char *_ptrFind;
 char *_strOuput =(char *)malloc(sizeof(char)*BUFFER);

 
 _ptrFind = strstr(__strInput, __str2Find);

 if (_ptrFind !=NULL){
	

	memcpy(_strOuput, __strInput, _ptrFind - __strInput);
	_strOuput[_ptrFind - __strInput] ='\0';
	strcat_s(_strOuput, BUFFER, __str2Replace);

	_ptrFind +=strlen(__str2Find);
	strcat_s(_strOuput, BUFFER, _ptrFind);
	 _ptrFind = strstr(_ptrFind, __str2Find);

	return _strOuput;
 }
return __strInput;
 }


#endif STRING_H