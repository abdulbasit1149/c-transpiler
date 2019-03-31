/*
Name: Abdul Basit 
Date: Sunday December 29, 2017
*/

#include "utility.h"
#include "memory.h"

char singleOrDouble(int c)
{
	if (c == '"')
	{
		return '"';
	}
	else
	{
		return '\'';
	}

	return '"';	
}

/*
	Create word from input buffer, and store in 2d char array
*/
void storeToken(char*** tokenStrings, char* string, int* indexEOL, int* currentIndex, int *totalSize)
{
		string[*indexEOL] = '\0';	
		int length = strlen(string);
		if(length > 0)
		{			
			*currentIndex = *currentIndex + 1;								
			add_to_storage(tokenStrings,string,*currentIndex, totalSize);									
		}
		strcpy(string,"");									
		*indexEOL  = 0;
}

/*
	Go through file character by character and create words and store them
*/
void parseFile(char* fileName, char*** tokenStrings, int* currentIndex, int* totalSize)
{		
	FILE* ifp = NULL;
	char buffer[513];
	char styleBuff[513];

	int styleIndex = 0;
	int index = 0;
	int c = 0;
	int prevC = 0;
	int forQuote = 0;	
	char check = '?';

	ifp = fopen(fileName, "r");
	valid_malloc(ifp, "opening a file");

	while((c = getc(ifp)) != EOF)
	{	

		/*
				Malloc new TwoD array 
		*/

		switch(c)
		{		
			case '\'':				
			case '"':
				check = singleOrDouble(c);				
				do
				{			
					/*
						To grab whole comment as one line
					*/	
					buffer[index++] = c;
					forQuote = prevC;
					prevC = c;
					c = getc(ifp);															

					if (c == check)
					{									
						if ((prevC != '\\' || (prevC == check && forQuote == '\\')))
						{																			
							break;								
						}
					}

				}while(1);		

						
				buffer[index++] = c;	
				storeToken(tokenStrings, buffer, &index, currentIndex, totalSize);					

				break;			
			case '/':														
			case '#':	
				storeToken(tokenStrings, styleBuff, &styleIndex, currentIndex, totalSize);														
				if(c == '#')
				{
					do
					{						
						buffer[index++] = c;
					}while((c = getc(ifp)) != '\n');	
						ungetc(c,ifp);				
				}	
				else if (c == '/')
				{					
					int loop = 1;
					prevC = c;
					c = getc(ifp);
					if(c == '/')
					{
						ungetc(c,ifp);
						do
						{						
							buffer[index++] = c;
						}while((c = getc(ifp)) != '\n');
						ungetc(c,ifp);	
					}
					else if(c == '*')
					{
						buffer[index++] = prevC;
						buffer[index++] = c;
						do
						{	
							c = getc(ifp);
							if(c == '*')
							{
								prevC = c;
								c = getc(ifp);
								if(c == '/')
								{
									buffer[index++] = prevC;
									loop = 0;
								}
							}
						
							buffer[index++] = c;						
						}while(loop);
					}
					else
					{
						storeToken(tokenStrings, buffer, &index, currentIndex, totalSize);										
						buffer[index++] = prevC;
						ungetc(c,ifp);
					}				
				}

				storeToken(tokenStrings, buffer, &index, currentIndex, totalSize);									
				break;																
			case ' ':
			case '\t':
			case '\n':				
			case '\r':
				styleBuff[styleIndex++] = c;			
				storeToken(tokenStrings, buffer, &index, currentIndex, totalSize);				
				break;	
			case '*':
			case '=':
			case '%':
			case '+':	
			case '-':
				storeToken(tokenStrings, buffer, &index, currentIndex, totalSize);						

				prevC = c;

				c = getc(ifp);
				if(c == prevC)
				{
					buffer[index++] = prevC;	
				}
				else
				{
					ungetc(c, ifp);
					c = prevC;
				}
						
			case ',':
			case ';':
			case '(':
			case ')':
			case '{':
			case '}':					
				storeToken(tokenStrings, buffer, &index, currentIndex, totalSize);
				buffer[index++] = c;	
				storeToken(tokenStrings, styleBuff, &styleIndex, currentIndex, totalSize);										
				storeToken(tokenStrings, buffer, &index, currentIndex, totalSize);				
				break;
			default:	
				buffer[index++] = c;
				storeToken(tokenStrings, styleBuff, &styleIndex, currentIndex, totalSize);															
		}
	}
	fclose(ifp);
}

int isWhiteSpaceChar(char* token)
{
	if(strchr(token, '\n') == NULL && strchr(token, ' ') == NULL &&
		 strchr(token, '\t') == NULL && strchr(token, '\r') == NULL)
		{
			return 0;
		}
	return 1;
}

int isVariable(char* token)
{
	if ((ispunct((int)token[0]) || isdigit((int)token[0])))
	{
		return 0;
	}

	return 1;
}

/*
	Check whether or not variable is function call and or pointer reference
*/
int isClassVariable(int index, char** tokenString)
{	
	while(tokenString[index][0] != ';' && tokenString[index][0] != '(')
	{
		index = index + 1;
	}

	if (strcmp(tokenString[index],"(") != 0 && strcmp(tokenString[index],"*") != 0 )
	{
		return 1;
	}

	return 0;	
}

int isBasicDataType(char* token)
{
	if (strcmp(token, "int") == 0 || strcmp(token, "float") == 0 || strcmp(token, "double") == 0 || strcmp(token, "char") == 0 || strcmp(token, "signed") == 0 || strcmp(token, "unsigned") == 0 || strcmp(token, "void") == 0 || strcmp(token, "short") == 0 || strcmp(token, "long") == 0)
	{
		return 1;
	}

	return 0;
}

/*
	To store function and class information within linked list
*/	
void processText(char*** tokens, int currEntires, classStruct** classHead, functionStruct** funcHead)
{	
	int i = 0;
	char** tokenStrings = *tokens;

	/*
		Flag Variables
	*/
	int isFunction = 0;
	int isClass = 0;
	int isParaVariable = 0;
	int isClassObject = 0;

	/*
		Temp Store and flag variables
	*/
	int leftBrac = 0;
	int rightBrac = 0;

	int firstFunction = 0;
	int toggleFindType = 1;

	/*
		Variables to temporarily hold place holder values
	*/
	char className[513];
	char functionName[513];
	char functionType[513];
	char functionParaType[513];
	char prevToken[513];
	char functPara[100][513];
	int functParaIndex = 0;
	char uniqueFuncName[513];
	int hasParaVariable = 0;

	char variableType[513];
	strcpy(variableType,"");

	char functionPtr[513];

	for(i = 0; i <= currEntires; i = i + 1)
	{	
		if (!isWhiteSpaceChar(tokenStrings[i]))
		{
			/*
				To find starting of class variable: either class object, or class definition
			*/
			if ((strcmp(tokenStrings[i],"class") == 0))
			{
				while((strcmp(tokenStrings[i],"{") != 0) && (strcmp(tokenStrings[i],";") != 0 ))
				{
					if (!isWhiteSpaceChar(tokenStrings[i]))
					{
						strcpy(className,tokenStrings[i]);
					}
					i = i + 1;
				}
				if (strcmp(tokenStrings[i],"{") == 0)
				{		
					isClass = 1;

					/*
						New class struct
					*/
					*classHead = add_to_class(*classHead, className);

					leftBrac = leftBrac + 1;

					while((leftBrac - rightBrac) != 0)
					{								
						if (!isWhiteSpaceChar(tokenStrings[i]))
						{												
							strcpy(prevToken,tokenStrings[i]);	
						}
		
						i = i + 1;	

						if (strcmp(tokenStrings[i], "class") == 0)
						{
							isClassObject = 1;
						}						

						/*
							Going threw tokens to identify which type of token it is and how to handle it
						*/

						if (
						   (isBasicDataType(tokenStrings[i]) || strcmp(tokenStrings[i],"*") == 0 || (
						   (strcmp(tokenStrings[i], "struct") == 0|| (strcmp(prevToken, "struct") == 0)) && !isFunction)) 
							&& !isWhiteSpaceChar(tokenStrings[i]))
						{	
							strcat(variableType,tokenStrings[i]);							
							strcat(variableType," ");
						}													
						
						if (strcmp(tokenStrings[i], ";") == 0)
						{
							strcpy(variableType,"");	
							isClassObject = 0;			
						}	
													
						if (strcmp(tokenStrings[i],"}") == 0) 
						{
							rightBrac = rightBrac + 1;
							isFunction = 0;
						}
						else if (strcmp(tokenStrings[i],"{") == 0)
						{
							strcpy(variableType,"");								
							leftBrac = leftBrac + 1;							
						}
						else if (strcmp(tokenStrings[i],")") == 0)
						{
							
						 	if (isParaVariable)
						 	{						 	
						 		char temp[513];
						 		strcpy(temp, "");
						 		strcpy(uniqueFuncName,"");
						 	
						 		sprintf(functionPtr,"%s(*%s%s%s)();",functionType,className,functionName,functionParaType);

						 		/*
									Store function pointer to class list
								*/
						 		add_to_class_funcptr(*classHead, functionPtr, className);					 		

						 		sprintf(temp, "%s%s%s", className,functionName,functionParaType);
								strcpy(uniqueFuncName,temp);

								/*
									Store function to function list
								*/
						 		*funcHead  = add_to_function(*funcHead, functionName, temp, className, hasParaVariable);
						 		hasParaVariable = 0;
							
						 		int j = 0;

						 		for (j = 0; j < functParaIndex; j = j + 1)
						 		{
						 			add_to_funct_variables(*funcHead,functPara[j] , temp);	
						 			strcpy(functPara[j],"");				 									 		
						 		}
						 				
						 		functParaIndex = 0;
						 	}
			
						 	isParaVariable = 0;							
						}							

						if (!isWhiteSpaceChar(tokenStrings[i]))
						{
						 	if (strcmp(tokenStrings[i], "(") == 0 && isFunction == 0)
						 	{
						 		isFunction = 1;
						 		isParaVariable = 1;
						 		firstFunction = 1;
						 
						 		strcpy(functionName,"");						 		
						 		strcpy(functionType,"");
						 		strcpy(functionParaType,"");

						 		strcpy(functionName,prevToken);
						 		strcpy(functionType,variableType);	

						 		strcpy(variableType,"");						 		
						 		strcpy(prevToken,"");					 								 		
						 	}
						 	else if (isVariable(tokenStrings[i]) && isParaVariable)
						 	{
						 		/*
									Store the return type of function pointer
						 		*/
						 		if (toggleFindType || isBasicDataType(tokenStrings[i]))
						 		{
						 			char temp[2];
						 			temp[0] = tokenStrings[i][0];
						 			temp[1] = '\0';
									strcat(functionParaType, temp);
									toggleFindType = 0;
						 		}
						 		else
						 		{
						 			toggleFindType = 1;							 			
						 			hasParaVariable = 1;
						 			/*
										Store function parameters
						 			*/
							 		strcpy(functPara[functParaIndex++],tokenStrings[i]);														 			
						 		}						
						 	}
						 	else if(isVariable(tokenStrings[i]) && isFunction && !isBasicDataType(tokenStrings[i]))
						 	{
						 		if (strcmp(variableType,"") != 0 && strchr(tokenStrings[i], '.') == NULL && strchr(variableType, '*') == NULL)
						 		{						 		
						 			add_to_funct_variables(*funcHead,tokenStrings[i] , uniqueFuncName);						 		
						 		}
						 	}
						 	else if(isVariable(tokenStrings[i]) && isClass && !firstFunction && !isFunction && !isBasicDataType(tokenStrings[i]) && !isClassObject)
						 	{
						 		if (isClassVariable(i , tokenStrings))
						 		{
						 			add_to_class_variables(*classHead,tokenStrings[i],className);
						 		}
						 	}					 	
						} 	
					}
					leftBrac = 0;
					rightBrac = 0;
					firstFunction = 0;
					isClass = 0;
				}			
			}			
		}
	}		
}

/*
	Replace string with newString
*/
void replace_string(char** string,char* newString)
{
	int newLength = strlen(newString) + 1;

	free(*string);
	*string = (char*)(malloc(sizeof(char) * newLength));

	strcpy(*string, newString);
}

void add_function_pointers_to_class(char* className, classStruct* classHead, char** location)
{
	classStruct* head = find_class_in_list(className, classHead);	
	char** array = head->functionPtr;
	int length = strlen(*location);
	int totalSize = 0;

	int i = 0;

	while(strcmp(array[i], "") != 0)
	{
		totalSize = totalSize + strlen(array[i]) + 2;
		i = i + 1;
	}

	char* newString = (char*)malloc(sizeof(char) * (length + totalSize + 4));
	strcpy(newString, *location);
	free(*location);	

	i = 0;

	while(strcmp(array[i], "") != 0)
	{		
		strcat(newString,"\n");
		strcat(newString,"\t");
		strcat(newString, array[i]);
		i = i + 1;
	}	

	strcat(newString, "\n};");
	*location = newString;
}

/*
	Change function name	
*/
void switch_function_name(functionStruct* funcHead, int* offset, char** functionName)
{
	functionStruct* temp = funcHead;

	int counter = 0;

	while(temp != NULL)
	{	
		if (counter == *offset)
		{
			replace_string(functionName, temp->functPtrName);
			break;			
		}		
		counter = counter + 1;
		temp = temp->next;
	}
	*offset = *offset + 1;
}

void insert_constructor_function(char* className, functionStruct* funcHead, char** location,char prevValue[])
{	
	int addVariable = 0;
	
	if (strchr(prevValue, '='))
	{
		addVariable = 1;
	}
	else
	{
		addVariable = 0;
	}

	char tempString[513];
	char* token = NULL;
	int varLength = 0;

	if (addVariable)
	{
		strcpy(tempString, prevValue);
		token  = strtok(tempString, " ");
		while(token != NULL)
		{
			if (strchr(token, '='))
			{
				varLength = varLength + strlen(token) + 9;				
			}
			token = strtok(NULL, " ");
		}
	}

	free(*location);
	
	int stringLength = (strlen(className) * 2) + 38;

	if (addVariable)
	{
		stringLength = stringLength + varLength;
	}

	int funcPointerLength = 0;

	functionStruct* temp = funcHead;

	while(temp != NULL)
	{
		if (strcmp(temp->className, className) == 0)
		{
			funcPointerLength = funcPointerLength + 11 + (strlen(temp->functPtrName) * 2);
		}
		temp = temp->next;
	}

	int totalSize = stringLength + funcPointerLength;

	*location = (char*)malloc(sizeof(char) * totalSize);
	valid_malloc(*location, "mallocing a string");

	strcpy(*location,"");

	strcat(*location, "\nvoid constructor");
	strcat(*location, className);
	strcat(*location, "(struct ");
	strcat(*location, className);
	strcat(*location, "* ptr)\n{");

	temp = funcHead;

	while(temp != NULL)
	{
		if (strcmp(temp->className, className) == 0)
		{
			strcat(*location, "\n\tptr->");
			strcat(*location, temp->functPtrName);
			strcat(*location, " = ");
			strcat(*location, temp->functPtrName);
			strcat(*location, ";");			
		}
		temp = temp->next;
	}	

	if (addVariable)
	{
		char storage[513];
		strcpy(storage, prevValue);
		char* varToAdd  = strtok(storage, " ");

		while(varToAdd != NULL)
		{			
			if (strchr(varToAdd, '='))
			{
				strcat(*location, "\n\t");
				strcat(*location, "ptr->");
				strcat(*location,varToAdd);			
				strcat(*location, ";");
			}
			
			varToAdd = strtok(NULL, " ");
		}
		
	}

	strcat(*location, "\n}\n");
}

/*
	Add reference to variable within the struct definiton
*/
void change_funct_para(functionStruct** funcHead,char* className, char** string, char** openBrac, char* identifer)
{	
	functionStruct* head = find_function_in_list(identifer,*funcHead);

	head->pointPara = 1;

	int length = 0;

	if (head->hasParaVariables)
	{
		length = (strlen(className) * 3) + 15;
	}
	else
	{
		length = (strlen(className)* 3) + 14;
	}

	free(*openBrac);
	*openBrac = (char*)malloc(sizeof(char) * length);
	valid_malloc(*openBrac, "mallocing string");
	strcpy(*openBrac,"");

	strcat(*openBrac, "(struct ");
	strcat(*openBrac, className);
		

	if (head->hasParaVariables)
	{
		strcat(*openBrac, "* ptr");
		strcat(*openBrac, className);		
		strcat(*openBrac, ",");
	}
	else
	{
		strcat(*openBrac, "* ptr");
		strcat(*openBrac, className);
	}

	char buffer[513];
	strcpy(buffer,*string);

	free(*string);
	length = strlen(buffer) + 7;

	*string = (char*)malloc(sizeof(char) * length);
	valid_malloc(*string, "mallocing string");
	strcpy(*string,"");

	strcat(*string, "ptr");
	strcat(*string,className);
	strcat(*string,"->");
	strcat(*string, buffer);
}

/*
	Verify whether function is class scope, function scope and replace at appropriate  time
*/
void verify_variable_scope(functionStruct** funcHead, classStruct* classHead, char* className,char** string,char** openBrac, char* identifer)
{
	functionStruct* functionHead = find_function_in_list(identifer, *funcHead);
	char** search = functionHead->variableNames;

	int j = 0;

	while(strcmp(search[j], "") != 0)
	{			
		if (strcmp(search[j], *string) == 0)
		{
			return;
		}
		j = j + 1; 
	}


	classStruct* head = find_class_in_list(className, classHead);
	char** array = head->variableNames;	 

	int i = 0;

	while(strcmp(array[i], "") != 0)
	{
		if (strcmp(array[i], *string) == 0)
		{
			change_funct_para(funcHead, className, string, openBrac, identifer);
			break;
		}
		i = i + 1;
	}
}

/*
	Add constructor to class object definitions
*/
void change_object_name(char* objectName, char* className, char** location)
{
	char buffer[513];
	strcpy(buffer, *location);
	free(*location);

	int length = strlen(className) + strlen(objectName) + 30 + strlen(buffer);

	*location = (char*)malloc(sizeof(char ) * length);
	strcpy(*location,"");
	strcat(*location, buffer);
	strcat(*location, "\n\t\tconstructor");
	strcat(*location, className);
	strcat(*location, "(&");
	strcat(*location, objectName);
	strcat(*location, ");\n");
}

/*
	Change method calls called from objects
*/
void handle_objects_constructors(char*** storage, int* index, objectStruct** objectHead)
{	
	char** tokenStrings = *storage;
	
	int lineIndex = 0;							
	char classObjectNames[513][513];
	char className[513];
	strcpy(className,"");

	while(strcmp(tokenStrings[*index], ";") != 0 && strcmp(tokenStrings[*index], ")") != 0)
	{	
		strcpy(classObjectNames[lineIndex],tokenStrings[*index]);
		lineIndex = lineIndex + 1;
		*index = *index + 1;		
	}

	int i = 0;

	for(i = 0; i < lineIndex; i = i + 1)
	{

		if (!isWhiteSpaceChar(classObjectNames[i]) && strcmp(classObjectNames[i], "struct") != 0 && !ispunct((int)classObjectNames[i][0]))
		{		
			if (strcmp(className,"") == 0)
			{
				strcpy(className,classObjectNames[i]);
			}
			else
			{
				*objectHead = add_to_object_list(*objectHead, classObjectNames[i], className);				
				change_object_name(classObjectNames[i], className, &tokenStrings[*index]);				

			}	
		}
	}					
}

/*
	Insert constructor for any global class object variable
*/
void insert_constructor_at_main(char** location, char arrayToInput[][513] ,int arrIndex)
{
	free(*location);
	int i = 0;
	int totalSize = 0;

	for (i = 0; i < arrIndex; i = i + 1)
	{
		arrayToInput[i][0] = ' ';
		totalSize = totalSize + strlen(arrayToInput[i]);
	}

	totalSize = totalSize + 3;

	*location = (char*)malloc(sizeof(char) * totalSize);
	valid_malloc(*location, "mallocing constructors in main");

	strcpy(*location,"");
	strcat(*location, "{\n");

	i = 0;

	for (i = 0; i < arrIndex; i = i + 1)
	{
		strcat(*location, arrayToInput[i]);
	}
}

/*
	Change function method call name
*/
void swap_function_call_name(char** location, char* className, functionStruct* funcHead)
{
	functionStruct* temp = funcHead;

	while(temp != NULL)
	{
		if (strcmp(temp->className, className) == 0)
		{
			if (strcmp(temp->funcName, *location) == 0)
			{				
				free(*location);
				*location = (char*)malloc(sizeof(char) * (strlen(temp->functPtrName) + 1));
				valid_malloc(*location, "mallocing new function name");
				strcpy(*location,  temp->functPtrName);
				return;
			}
		}
		temp = temp->next;
	}
}

/*
	Insert reference to struct variable to method of that struct
*/
void insert_object_method(char** location, int varCount, objectStruct* objHead,functionStruct* funcHead, char** endOfString)
{
	char temp[513];
	char* token;
	char objectName[513];
	char* className;

	char temp2[513];
	strcpy(temp2,*location);

	char buffer[513];

	strcpy(temp,*location);
	token = strtok(temp, ".");	
	strcpy(objectName, token);

	className = get_object_class(objHead, token);

	char* breakPoint =  strchr(temp2, '.');

	int i = 0;

	for (i = 1; i < strlen(breakPoint); i = i + 1)
	{
		buffer[i - 1] = breakPoint[i];
	}
	buffer[i - 1] = '\0';	

	functionStruct* tempHead = funcHead;

	while(tempHead != NULL)
	{	
		if (strcmp(tempHead->className, className) == 0)
		{
			if (strcmp(tempHead	->funcName,buffer ) == 0)
			{
				if (((strlen(tempHead->functPtrName) - strlen(tempHead->className) - varCount)) == strlen(tempHead->funcName))
				{

					free(*location);
					*location = (char*)malloc(sizeof(char) * (strlen(objectName) + strlen(tempHead->functPtrName) + 2));
					valid_malloc(*location, "mallocing string in object method");
					strcpy(*location, "");
					strcat(*location, objectName);
					strcat(*location, ".");
					strcat(*location,tempHead->functPtrName);

					if (tempHead->pointPara)
					{
						free(*endOfString);

						if (varCount > 0)
						{
							*endOfString = (char*)malloc(sizeof(char) * (strlen(objectName)) + 4);	
							valid_malloc(*endOfString, "mallocing end of method call");						
						}
						else
						{
							*endOfString = (char*)malloc(sizeof(char) * (strlen(objectName)) + 3);
							valid_malloc(*endOfString, "mallocing end of method call");								
							
						}	
						strcpy(*endOfString, "");
						strcat(*endOfString, "(");
						strcat(*endOfString, "&");				
						strcat(*endOfString, objectName);						
						if (varCount > 0)
						{
							strcat(*endOfString, ",");			
						}
					}

					break;				
				}
			}
		}
		tempHead = tempHead->next;
	}

}

/*
	Replace tokens with c standard ones
*/
void replaceText(char*** tokens, int currEntires, classStruct** classHead, functionStruct** funcHead)
{
	char** tokenStrings = *tokens;		
	int i = 0;
	char className[513];	
	int indexLocation = 0;	
	int isMain = 0;
	int isFunction = 0;
	int isClass = 0;
	int indexOfClass = 0;

	int constArrIndex = 0;
	char constrInMain[513][513];
	int toErase[1000];
	int eraseIndex = 0;
	
	objectStruct* mainObjHead = NULL;
	objectStruct* globalObjHead = NULL;

	for(i = 0; i <= currEntires; i = i + 1)
	{			
		if (!isWhiteSpaceChar(tokenStrings[i]))
		{
			if (strcmp(tokenStrings[i],"class") == 0)
			{									
				int leftBrac = 0;
				int rightBrac = 0;
				indexOfClass = i;

				replace_string(&tokenStrings[i], "struct");

				/*
					To find class name
				*/
				while((strcmp(tokenStrings[i],"{") != 0) && (strcmp(tokenStrings[i],";") != 0 ))
				{
					if (!isWhiteSpaceChar(tokenStrings[i]))
					{
						strcpy(className,tokenStrings[i]);
					}
					i = i + 1;
				}
				
				if (strcmp(tokenStrings[i],"{") == 0)
				{
					isClass = 1;
					leftBrac = leftBrac + 1;
					int LastPosToBreak = i;
					int emptyClass = 0;
					int lastBrack = 0;
					int noMethod = 0;
					int functionNameIndex = 0;
					int functionStartIndex = 0;
					char functName[513];

					char prevValue [1000];
					strcpy(prevValue, "");
			
					while((strcmp(tokenStrings[i],"(") != 0))
					{			
						if (strcmp(tokenStrings[i], "=") == 0 || strcmp(tokenStrings[i], ";") == 0 || (!isWhiteSpaceChar(tokenStrings[i]) && !ispunct((int)tokenStrings[i][0]) && !isBasicDataType(tokenStrings[i])))
						{
							if (strcmp(tokenStrings[i], ";") == 0)
							{
								strcat(prevValue, " ");
							}
							else
							{
								strcat(prevValue, tokenStrings[i]);															
							}
						}		

						if (strcmp(tokenStrings[i],";") == 0 || strchr(tokenStrings[i], '/'))
						{
							/*
								Where to insert function pointers and close the struct
							*/
							LastPosToBreak = i;
						}
						else if (strcmp(tokenStrings[i], "}") == 0)
						{
							/*
								If class has no data inside of it
							*/
							emptyClass = 1;	
							noMethod = 1;						
							break;
						}
						if (!isWhiteSpaceChar(tokenStrings[i]) && !noMethod)
						{
							functionNameIndex = i;	
						}

						if (strcmp(tokenStrings[i], "=") == 0) 
						{		
							int breakPoint = i;				
							while(strcmp(tokenStrings[i], ";") != 0)
							{
								toErase[eraseIndex++] = i;
								i = i + 1;								
							}
							i = breakPoint;
						}

						/*
							To store any variables declared in global scope of class
						*/					
						i = i + 1;
					}	
					/*
						Empty variable init. location
					*/
					int z = 0;

					for (z = 0; z< eraseIndex; z = z + 1)
					{
						strcpy(tokenStrings[toErase[z]],"");
					}

					if (emptyClass && noMethod)
					{
						replace_string(&tokenStrings[i], "};");					
						insert_constructor_function(className, *funcHead, &tokenStrings[i+1], prevValue );
						strcpy(prevValue, "");
						continue;
					}		
					else if(!noMethod)
					{
						isFunction = 1;						
					}

					functionStartIndex = i;		
					switch_function_name(*funcHead ,&indexLocation, &tokenStrings[functionNameIndex]);
					strcpy(functName,tokenStrings[functionNameIndex]);

					if (!emptyClass)
					{
						if (strcmp(tokenStrings[i],"(") == 0)
						{
							add_function_pointers_to_class(className, *classHead, &tokenStrings[LastPosToBreak]);				
						} 
						else
						{
							add_function_pointers_to_class(className, *classHead, &tokenStrings[i]);	
						}
					}	

					objectStruct* objHead = NULL;	
					/*
						Loop until end of class
					*/				
					while((leftBrac - rightBrac) != 0)
					{	

						if (strcmp(tokenStrings[i], "}") == 0)
						{	
							lastBrack = i;						
							rightBrac = rightBrac + 1;
						}
						
						if (strcmp(tokenStrings[i], "{") == 0)
						{
							leftBrac = leftBrac + 1;						
						}					

						if (!isWhiteSpaceChar(tokenStrings[i]))
						{
							if (!isBasicDataType(tokenStrings[i]) &&  !ispunct((int)tokenStrings[i][0]) && (leftBrac - rightBrac) == 2)
							{			
								/*
									Verify each variable inside class and change it
								*/					
								verify_variable_scope(funcHead,*classHead, className,&tokenStrings[i] ,&tokenStrings[functionStartIndex], functName);																														
							}						
							functionNameIndex = i;							
						}			
						
						if (strcmp(tokenStrings[i],"class") == 0 && isFunction && !isMain)
						{			
							replace_string(&tokenStrings[i], "struct");	
							/*
								Ensure each class object calls a constructor
							*/
							handle_objects_constructors(&tokenStrings, &i, &objHead);	
						}			
						if (strchr(tokenStrings[i], '.') && !strchr(tokenStrings[i], '"'))
						{
							int varCount = 0;
							int callIndex = i;
							int firstBrack = 0;

							/*
								To change function calls from objects
							*/
							while(strcmp(tokenStrings[i], ";") != 0)
							{
								if (strcmp(tokenStrings[i],"(") == 0)
								{
									firstBrack = i;
									while(strcmp(tokenStrings[i],")") != 0)
									{
										if(!isWhiteSpaceChar(tokenStrings[i]) && strcmp(tokenStrings[i], ",") != 0)
										{
											varCount = varCount + 1;
										}									
										i = i + 1;
									}	
									varCount = varCount - 1;
									insert_object_method(&tokenStrings[callIndex], varCount, objHead, *funcHead,  &tokenStrings[firstBrack]);									
									break;
								}
								i = i + 1;
							}							
						}
						
						i = i + 1;
						isClass = 0;						
						if (i > currEntires)
						{
							i = i - 1;
							break;
						}

						/*
							Find function name within class, and replace it 
						*/
						if ( strcmp(tokenStrings[i], "(") == 0 && (leftBrac - rightBrac) == 1)
						{								
							isFunction = 1;
							functionStartIndex = i;		
							switch_function_name(*funcHead ,&indexLocation, &tokenStrings[functionNameIndex]);
							strcpy(functName,tokenStrings[functionNameIndex]);
							
						}	

						if (!isWhiteSpaceChar(tokenStrings[i])  && isVariable(tokenStrings[i]))
						{
							if (!is_variable_in_class(className, *classHead, tokenStrings[i]) && !is_variable_in_function(functName, *funcHead, tokenStrings[i]))
							{
								swap_function_call_name(&tokenStrings[i], className, *funcHead);					
							}
						}	
					}

					destroy_object_list(objHead);					
					isFunction = 1;	

					if (!noMethod)
					{
						strcpy(tokenStrings[lastBrack], "");								
					}			

					while(i <= currEntires && isWhiteSpaceChar(tokenStrings[i]) && strcmp(tokenStrings[i], ";") != 0)
					{										
						i = i + 1;						
					}	

					if (strcmp(tokenStrings[i], ";") != 0)
					{
						i = i - 1;
					}
					strcpy(tokenStrings[i], "");	
					insert_constructor_function(className, *funcHead, &tokenStrings[i], prevValue);	
					strcpy(prevValue, "");					
				}				
				else if (isMain && strcmp(tokenStrings[i],"}") == 0)
				{
					/*
						End of main
					*/
					isMain = 0;
				}	
				else
				{						
					if (isMain)
					{
						/*
							Add constructor calls to object in main scope
						*/
						i = indexOfClass;
						handle_objects_constructors(&tokenStrings, &i, &mainObjHead);	
					}					
					else if(!isClass && !isMain)
					{
						/*
							Add constructor calls to object in global scope
						*/
						i = indexOfClass;						
						handle_objects_constructors(&tokenStrings, &i, &globalObjHead);
						strcpy(constrInMain[constArrIndex++], tokenStrings[i]);
						strcpy(tokenStrings[i],";");
					}									
				}	
					
			}
			else if (strcmp(tokenStrings[i], "main") == 0)
			{
				isMain = 1;				
				while(strcmp(tokenStrings[i], "{") != 0)
				{
					i = i + 1;					
				}
				insert_constructor_at_main(&tokenStrings[i], constrInMain,constArrIndex);				
			}
			else if(isMain)
			{
				if (strchr(tokenStrings[i], '.') && !strchr(tokenStrings[i], '"'))
				{
					int varCount = 0;
					int callIndex = i;
					int firstBrack = 0;			

					while(strcmp(tokenStrings[i], ";") != 0)
					{

						if (strcmp(tokenStrings[i],"(") == 0)
						{
							firstBrack = i;
							while(strcmp(tokenStrings[i],")") != 0)
							{
								if(!isWhiteSpaceChar(tokenStrings[i]) && strcmp(tokenStrings[i], ",") != 0)
								{
									varCount = varCount + 1;
								}									
								i = i + 1;
							}	
							varCount = varCount - 1;	
							/*
								Change funcion method with object to correct function pointer name
							*/	
							insert_object_method(&tokenStrings[callIndex], varCount, mainObjHead, *funcHead, &tokenStrings[firstBrack]);	
							insert_object_method(&tokenStrings[callIndex], varCount, globalObjHead, *funcHead, &tokenStrings[firstBrack]);					
							break;
						}
						i = i + 1;
					}	

				}
			}

		}
	}

	destroy_object_list(mainObjHead);				
	destroy_object_list(globalObjHead);
}

void printFile(char** tokenStrings, int currEntires, char* fileName)
{
	int i = 0;
	char buffer[600];
	strcpy(buffer,"");
	strcpy(buffer,fileName);
	buffer[strlen(buffer) - 1] = '\0';

	FILE* ifp = fopen(buffer, "w");

	for (i = 0; i <= currEntires; i = i + 1)
	{
		fprintf(ifp,"%s", tokenStrings[i]);
	}

	fclose(ifp);
}