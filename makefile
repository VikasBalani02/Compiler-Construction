all: driver
driver: parser.o lexer.o stack.o tree.o lookupTable.o ast.o
	gcc driver.c -o driver parser.o lexer.o stack.o tree.o lookupTable.o ast.o
%.o : %.c
	gcc -c $^
