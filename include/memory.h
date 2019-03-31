/*
Name: Abdul Basit 
Date: Sunday December 29, 2017
*/

#ifndef __ABASIT__MEMORY__
#define __ABASIT__MEMORY__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct classStruct
{
	char* className;
	char* functionPtr[100];
	char* variableNames[100];	
	struct classStruct* next;
}classStruct;

classStruct* make_class(char*);
classStruct* add_to_class(classStruct*, char*);

void add_to_class_variables(classStruct*,char* , char*);
void add_to_class_funcptr(classStruct*,char*, char*);

void print_members(char* []);
void print_class(classStruct*);
void free_class(classStruct*);
void free_arrays(char* []);
void destroy_classes(classStruct*);

classStruct* find_class_in_list(char*, classStruct*);
int is_variable_in_class(char*, classStruct*, char*);

typedef struct functionStruct
{
	char* funcName;
	char* className;
	char* functPtrName;
	char* variableNames[100];
	int hasParaVariables;
	int pointPara;
	struct functionStruct* next;	
}functionStruct;

functionStruct* make_function(char*, char*, char*, int);
functionStruct* add_to_function(functionStruct* , char*, char*, char*, int);

void add_to_funct_variables(functionStruct*,char* , char*);

void print_funct(functionStruct*);
void free_funct(functionStruct*);
void destroy_funct(functionStruct*);

void valid_malloc(void*, char*);
char** malloc2dArray(int);
void free2dArray(char**, int);
void add_to_storage(char***,char*,int,int*);

functionStruct* find_function_in_list(char*, functionStruct*);
int is_variable_in_function(char*, functionStruct*, char*);

typedef struct objectStruct
{
	char* objectName;
	char* className;
	struct objectStruct* next;
}objectStruct;

objectStruct* make_object(char*, char*);
objectStruct* add_to_object_list(objectStruct*,char*, char*);

void print_object_list(objectStruct*);
void destroy_object_list(objectStruct*);

char* get_object_class(objectStruct*, char*);

#endif