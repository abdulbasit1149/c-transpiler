CC = gcc
CCFLAGS = -Wall -ansi -g
TARGET = a1
LIB = -Iinclude
DEPD = obj/a1.o obj/utility.o obj/memory.o

target:$(TARGET)

$(TARGET): obj/a1.o obj/utility.o obj/memory.o
	$(CC) $(CCFLAGS) $(DEPD) $(LIB) -o a1

obj/a1.o: src/a1.c
	$(CC) -c $(CCFLAGS) src/a1.c $(LIB) -o obj//a1.o

obj/utility.o:src/utility.c
	$(CC) -c $(CCFLAGS) src/utility.c $(LIB) -o obj/utility.o

obj/memory.o:src/memory.c
	$(CC) -c $(CCFLAGS) src/memory.c $(LIB) -o obj/memory.o	

clean:
	rm -f obj/*.o a1 test.c sample1.c sample2.c sample3.c *.stackdump *.exe

