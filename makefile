all: driver
driver: parser.o lexer.o stack.o tree.o lookupTable.o ast.o symbolTable.o
	gcc driver.c -o driver parser.o lexer.o stack.o tree.o lookupTable.o ast.o symbolTable.o
compilewithc:
	gcc driver.c -o driver parser.c lexer.c stack.c tree.c lookupTable.c ast.c symbolTable.c
%.o : %.c
	gcc -c $^
