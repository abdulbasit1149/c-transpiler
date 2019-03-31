/*
Name: Abdul Basit 
Student ID: 0930431
Date: Sunday December 29, 2017
Class Code: CIS*2750
Assignment Number: #1
*/

#ifndef __ABASIT__UTILITY__
#define __ABASIT__UTILITY__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "memory.h"

char singleOrDouble(int);
void parseFile(char*, char***, int*, int*);
void storeToken(char*** , char* , int* , int* , int * );

int isWhiteSpaceChar(char*);
int isVariable(char*);
int isClassVariable(int, char**);
int isBasicDataType(char*);
void processText(char***, int, classStruct**, functionStruct**);

void replace_string(char**,char*);
void add_function_pointers_to_class(char*, classStruct*, char**);
void switch_function_name(functionStruct*, int*, char**);
void insert_constructor_function(char*, functionStruct*, char**, char []);
void change_funct_para(functionStruct**, char*, char** , char**, char*);
void verify_variable_scope(functionStruct**, classStruct*, char* ,char** ,char**, char*);
void change_object_name(char*, char*, char**);	
void handle_objects_constructors(char***, int*, objectStruct**);
void insert_constructor_at_main(char**, char [][513],int);
void swap_function_call_name(char**, char*, functionStruct*);														
void insert_object_method(char**, int, objectStruct*,functionStruct*, char** );									
void replaceText(char***, int, classStruct**, functionStruct**);

void printFile(char**, int, char*);

#endif