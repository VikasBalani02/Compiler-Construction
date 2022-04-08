#include "ast.h"

/********Actual Symbol Structure*******/

//declared Type in astDef.h for use in ast_node
// typedef enum TYPE{
// 	INT,
// 	REAL,
// 	RECORD,
// 	UNION,
// 	RUID,
// }Type;

struct record_field{
	char * lexeme;
	Type type;
	char * ruid;
	int offset;
	int width;
	struct record_field * next;
	int line_no;
};
typedef struct typeInfo{
	Type type;
	char * type_ruid;
	struct typeInfo * next;
}typeInfo;
struct function_field{
	typeInfo *InputHead;
	typeInfo *OutputHead;
};
struct symbolTableRecord{

	//Lexeme will be in it only
	char* lexeme;
	int line_no;

	Type type;
	char* type_ruid;

	//Memory occupied by variable
	int width;

	int offset;	
	struct record_field * recordFields;
    SymbolTableRecord* next;
	struct symbolTable *global;
	struct function_field * function_field;
	//Whether the declared variable has been assigned some value or not
	//int assigned;

	//Corresponding AST node of the symbol
	//struct astNode* astNode;

	//For matrices
	//int dimension1, dimension2;

	//For sorting symbols to print them in order
	//int number;

	//For Code Generation
	//int* val;
	//char* str;

};
typedef struct symbolTableRecord SymbolTableRecord;



/***********************************************************************************************/

struct symbolTableSlot{
    int noOfRecords;
    SymbolTableRecord * head;
};
typedef struct symbolTableSlot symbolTableSlot;
struct symbolTable{
	int currentOffset;
	symbolTableSlot **list;
	int no_slots;
};	

typedef struct symbolTable symbolTable;


