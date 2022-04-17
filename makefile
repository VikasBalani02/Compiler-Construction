all: driver
driver: driver.c parser.o lexer.o stack.o tree.o lookupTable.o ast.o symbolTable.o typeChecker.o intermediate_code.o codegen.o
	gcc driver.c -o driver parser.o lexer.o stack.o tree.o lookupTable.o ast.o symbolTable.o typeChecker.o codegen.o intermediate_code.o 
compilewithc:
	gcc driver.c -o driver parser.c lexer.c stack.c tree.c lookupTable.c ast.c symbolTable.c
%.o : %.c
	gcc -c $^
assembly: 
	rm assembly
	nasm -felf32 -l assembly.lst assembly.asm
	gcc -o assembly -m32 assembly.o
	./assembly