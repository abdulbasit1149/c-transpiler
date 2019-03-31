/*
Name: Abdul Basit 
Student ID: 0930431
Date: Sunday December 29, 2017
Class Code: CIS*2750
Assignment Number: #1
*/

#include "memory.h"

void valid_malloc(void* ptr, char* message)
{
	if (ptr == NULL)
	{
		printf("Error with malloc at: %s\n", message);
		exit(1);
	}
}

char** malloc2dArray(int size)
{
	char** temp = NULL;

	temp = (char**)malloc(sizeof(char*) * size );
	valid_malloc(temp, "mallocing 2d char* pointers*");

	return temp;
}

void free2dArray(char** array, int count)
{
	int i = 0;

	for (i = 0; i <= count; i = i + 1)
	{
		free(array[i]);
	}
	free(array);
}

void add_to_storage(char*** array,char* string,int index, int* maxSize)
{
	int length = strlen(string) + 1;
	(*array)[index] = (char*)malloc(sizeof(char) * length);
	valid_malloc((*array)[index], "mallocing item to add to storage");
	strcpy((*array)[index], string);
}

classStruct* make_class(char* className)
{
	classStruct* temp = (classStruct*)malloc(sizeof(classStruct));
	valid_malloc(temp, "mallocing class");
	
	temp->className = (char*)malloc(sizeof(char) * (strlen(className) + 1));
	valid_malloc(temp->className , "mallocing class name");

	strcpy(temp->className, className);
	temp->next = NULL;

	int i = 0;

	for (i = 0; i < 100; i = i + 1)
	{
		temp->variableNames[i] = (char*)malloc(sizeof(char) * 2);
		strcpy(temp->variableNames[i], "");

		temp->functionPtr[i] =(char*)malloc(sizeof(char) * 2);		
		strcpy(temp->functionPtr[i], "");
	}

	return temp;
}

classStruct* add_to_class(classStruct* head,  char* className)
{
	classStruct* temp = make_class(className);
	valid_malloc(temp, "mallocing temp class");

	if (head == NULL)
	{
		return temp;
	}

	classStruct* tail = head;

	while(tail->next != NULL)
	{
		tail = tail->next;
	}

	tail->next = temp;

	return head;	
}

void add_to_class_variables(classStruct* toHead,char* variableName, char* className)
{
	classStruct* temp = toHead;
	int i = 0;	

	while(temp != NULL && strcmp(temp->className, className) != 0)
	{
		temp = temp->next;
	}	

	while(strcmp(temp->variableNames[i], "") != 0)
	{
		i = i + 1;
	}
	free(temp->variableNames[i]);
	temp->variableNames[i] = NULL;
	temp->variableNames[i] = malloc(sizeof(char) * (strlen(variableName) + 1));
	valid_malloc(temp->variableNames[i], "mallocing class member: variable");
	strcpy(temp->variableNames[i],variableName);			
}

void add_to_class_funcptr(classStruct* toHead,char* ptrName, char* className)
{

	classStruct* temp = toHead;
	int i = 0;	

	while(temp != NULL && strcmp(temp->className, className) != 0)
	{		
		temp = temp->next;
	}	
	
	while(strcmp(temp->functionPtr[i], "") != 0)
	{
		i = i + 1;
	}
	free(temp->functionPtr[i]);
	temp->functionPtr[i] = NULL;
	temp->functionPtr[i] =(char*) malloc(sizeof(char) * (strlen(ptrName) + 1));
	strcpy(temp->functionPtr[i],ptrName);
}

void print_members(char* array[])
{
	int i = 0;

	while(strcmp(array[i], "") != 0)
	{
		printf("%s\n", array[i]);
		i = i + 1;
	}
}

void print_class(classStruct* head)
{
	if (head == NULL)
	{
		return;
	}

	classStruct* temp = head;

	while(temp != NULL)
	{
		printf("class Name: %s\n",temp->className );	
		print_members(temp->variableNames);
		print_members(temp->functionPtr);		
		temp = temp->next;
	}
}

void free_arrays(char* array[])
{
	int i = 0;

	for (i = 0; i < 100; i = i + 1)
	{
		free(array[i]);
	}
}

void free_class(classStruct* head)
{
	free(head->className);
	free(head);
}

void destroy_classes(classStruct* head)
{
	if (head == NULL)
	{
		return;
	}

	classStruct* temp = head;

	while(head != NULL)
	{	
		temp = head->next;
		free_arrays(head->variableNames);
		free_arrays(head->functionPtr);
		free_class(head);
		head = temp;
	}
}

classStruct* find_class_in_list(char* className, classStruct* classHead)
{
	while(strcmp(classHead->className,className) != 0)
	{
		classHead = classHead->next;
	}

	return classHead;
}

int is_variable_in_class(char* className, classStruct* classHead, char* toSearch)
{
	classStruct* head = find_class_in_list(className, classHead);
	char** array = head->variableNames;

	int i = 0;

	while(strcmp(array[i], "") != 0)
	{
		if (strcmp(array[i],className) == 0)
		{
			return 1;
		}
		i = i + 1;
	}

	return 0;
}

functionStruct* make_function(char* funcName, char* functPtrName, char* className, int hasParaVar)
{
	functionStruct* temp = (functionStruct*)malloc(sizeof(functionStruct));
	valid_malloc(temp, "mallocing a function");

	temp->funcName = (char*)malloc(sizeof(char) * (strlen(funcName) + 1));
	valid_malloc(temp->funcName,"mallcing functoin name");
	strcpy(temp->funcName,funcName);

	temp->functPtrName = (char*)malloc(sizeof(char) * (strlen(functPtrName) + 1));
	valid_malloc(temp->functPtrName,"mallcing functoin name");
	strcpy(temp->functPtrName,functPtrName);

	temp->className = (char*)malloc(sizeof(char) * (strlen(className) + 1));
	valid_malloc(temp->className,"mallcing functoin name");
	strcpy(temp->className,className);

	temp->hasParaVariables = hasParaVar;
	temp->pointPara = 0;
	temp->next = NULL;

	int i = 0;

	for (i = 0; i < 100; i = i + 1)
	{
		temp->variableNames[i] = (char*)malloc(sizeof(char) * 2);
		strcpy(temp->variableNames[i], "");
	}


	return temp;
}

functionStruct* add_to_function(functionStruct* head, char* funcName, char* functPtrName, char* className, int hasParaVar)
{
	functionStruct* temp = make_function(funcName, functPtrName,className, hasParaVar);
	valid_malloc(temp, "mallocing temp class");

	if (head == NULL)
	{
		return temp;
	}

	functionStruct* tail = head;

	while(tail->next != NULL)
	{
		tail = tail->next;
	}

	tail->next = temp;

	return head;	
}

void add_to_funct_variables(functionStruct* toHead,char* variableName, char* functPtrName)
{
	functionStruct* temp = toHead;
	int i = 0;	

	while(temp != NULL && strcmp(temp->functPtrName, functPtrName) != 0)
	{
		temp = temp->next;
	}	

	while(strcmp(temp->variableNames[i], "") != 0)
	{
		i = i + 1;
	}

	free(temp->variableNames[i]);
	temp->variableNames[i] = NULL;
	temp->variableNames[i] = (char*)malloc(sizeof(char) * (strlen(variableName) + 1));
	valid_malloc(temp->variableNames[i], "mallocing class member: variable");
	strcpy(temp->variableNames[i],variableName);	
}

void print_funct(functionStruct* head)
{
	if (head == NULL)
	{
		return;
	}

	functionStruct* temp = head;

	while(temp != NULL)
	{
		printf("funct Name: %s\n",temp->funcName);	
		printf("class Name(belongs to): %s\n",temp->className);			
		printf("functoin pointer Name: %s\n",temp->functPtrName);	
		printf("pointer parameter: %d\n", temp->pointPara);	
		print_members(temp->variableNames);	
		printf("--------\n");	
		temp = temp->next;
	}
}

void free_funct(functionStruct* head)
{
	free(head->className);
	free(head->funcName);
	free(head->functPtrName);
	free(head);
}

void destroy_funct(functionStruct* head)
{
	if (head == NULL)
	{
		return;
	}

	functionStruct* temp = head;

	while(head != NULL)
	{	
		temp = head->next;
		free_arrays(head->variableNames);		
		free_funct(head);
		head = temp;
	}
}

functionStruct* find_function_in_list(char* functionPtrName, functionStruct* funcHead)
{
	while(strcmp(funcHead->functPtrName, functionPtrName) != 0)
	{
		funcHead = funcHead->next;
	}		

	return funcHead;
}

int is_variable_in_function(char* functionName, functionStruct* funcHead, char* toSearch)
{

	functionStruct* head = find_function_in_list(functionName, funcHead);
	char** array = head->variableNames;

	int i = 0;

	while(strcmp(array[i], "") != 0)
	{
		if (strcmp(array[i],toSearch) == 0)
		{
			return 1;
		}
		i = i + 1;
	}

	return 0;
}

objectStruct* make_object(char* objectName, char* className) 
{
	objectStruct* temp = (objectStruct*)malloc(sizeof(objectStruct));
	valid_malloc(temp, "mallocing an object");

	temp->objectName = (char*)malloc(sizeof(char) * (strlen(objectName) + 1));
	valid_malloc(temp->objectName, "mallocing object name");
	strcpy(temp->objectName, objectName);

	temp->className = (char*)malloc(sizeof(char) * (strlen(className) + 1));
	valid_malloc(temp->className, "mallocing object name");
	strcpy(temp->className, className);

	temp->next = NULL;

	return temp;
}

objectStruct* add_to_object_list(objectStruct* head,char* objectName, char* className)
{
	objectStruct* toAdd = make_object(objectName, className);

	if (head == NULL)
	{
		return toAdd;
	}
	
	objectStruct* temp = head;

	while(temp->next != NULL)
	{		
		temp = temp->next;
	}

	temp->next = toAdd;

	return head;
}

void print_object_list(objectStruct* head)
{
	if (head == NULL)
	{
		return;
	}

	objectStruct* temp = head;

	while(temp != NULL)
	{
		printf("-----------------------------\n");
		printf("Object Name: %s\n", temp->objectName);
		printf("Class Name %s\n", temp->className);
		temp = temp->next;
	}
}

void destroy_object_list(objectStruct* head)
{
	if (head == NULL)
	{
		return;
	}

	objectStruct* temp = head;

	while(head != NULL)
	{
		temp = head->next;
		if (head->objectName != NULL)
		{
			free(head->objectName);			
		}

		if (head->className != NULL)
		{
			free(head->className);			
		}

		if (head != NULL)
		{
			free(head);			
		}
		head = temp;
	}
}

char* get_object_class(objectStruct* objHead, char* objectName)
{
	objectStruct* temp = objHead;

	while(temp != NULL)
	{
		if (strcmp(temp->objectName, objectName) == 0)
		{
			return temp->className;
		}
		temp = temp->next;
	}

	return "cannot find";
}