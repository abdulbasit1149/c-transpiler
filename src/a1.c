/*
Name: Abdul Basit 
Date: Sunday December 29, 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include "memory.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Incorrect amount of command line arguments\n");
		printf("a1 <filename>\n"); 
		exit(1);
	}			  

	char** tokenStrings = NULL;
	int totalSizeOf2dArray =4000;
	int currEntires = -1;	

	/*
		To store all the different functions and class information
	*/
	classStruct* classHead = NULL;
	functionStruct* functHead = NULL;

	tokenStrings = malloc2dArray(totalSizeOf2dArray);

	/*
		Open the file and store information within 2d char arrays
	*/
	parseFile(argv[1], &tokenStrings, &currEntires, &totalSizeOf2dArray);

	/*
		Gather information regarding class, functions etc...
	*/
	processText(&tokenStrings, currEntires, &classHead, &functHead);	

	/*
		Replace Tokens
	*/
	replaceText(&tokenStrings, currEntires, &classHead, &functHead);	

	/*
		Print tokens to files
	*/ 
	printFile(tokenStrings, currEntires, argv[1]);

	/*
		Free all memory for program
	*/
	free2dArray(tokenStrings,currEntires);
	destroy_classes(classHead);
	destroy_funct(functHead);

	return 0;
}