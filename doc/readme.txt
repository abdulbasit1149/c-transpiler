Name: Abdul Basit 
Student ID: 0930431
Date: Sunday December 29, 2017
Class Code: CIS*2750
Assignment Number: #1

**************************************************************************************************************
Notes: / Limitation:
	
	-Sometimes the compiler will threw the warning of incompatible pointer type with the class constructor connecting the function pointer
	to their corresponding function

	-multiple variable initialization on the same line  in class scope, could lead to errors 
	-The massive 2d array storage has a limit of 4000 strings, anything past that will cause seg fault
	-sample2.cc causes: warning: assignment from incompatible pointer type error, which I was not able to solve

**************************************************************************************************************

Compilation:

	1.	Untar the tar directory
	2.  type make
	3.  ./a1 <filename>

To compile the program, you need to type 'make' in the terminal. Which will compile the source code into one executable file

Example:

$ make

gcc -c -Wall -ansi -g src/a1.c -Iinclude -o obj//a1.o
gcc -c -Wall -ansi -g src/utility.c -Iinclude -o obj/utility.o
gcc -c -Wall -ansi -g src/memory.c -Iinclude -o obj/memory.o
gcc -Wall -ansi -g obj/a1.o obj/utility.o obj/memory.o -Iinclude -o a1

*************************************************************************************************************
How to run:

-To start the program type: "./a1 <fileName>

**************************************************************************************************************