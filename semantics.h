#pragma once

//Different Datum Types
//D_NULL:	a null atom: ()
//D_STRING:	a string atom
//D_FLOAT:	a float atom
//D_INT:	an integer atom
//D_CONS:	a cons pair datum
//D_LAMBDA:	a lambda expression
//D_LAM_ARG:	an argument in a lambda expression (only used internaly)
//		argument number is stored in valInt
enum DATUM_TYPE {D_NULL, D_STR, D_FLOAT, D_INT, D_CONS, D_LAMBDA, D_LAM_ARG};

struct cons
{
	struct datum* car;
	struct datum* cdr;
};

struct lambda
{
	//if numArgs==-1, lambda contains a function pointer
	unsigned int numArgs;
	union
	{
		struct datum* body;
		struct datum*(*funcPtr)(struct datum*);
	};
};

struct datum
{
	enum DATUM_TYPE type;
	union
	{
		char* valStr;
		float valFloat;
		int valInt;
		struct cons valCons;
		struct lambda valLambda;
	};
};

struct tableRecord
{
	char* name;
	struct datum* value;
};

typedef struct cons cons;
typedef struct datum datum;
typedef struct lambda lambda;

struct tableRecord* symbolTable;
int tableSize, tableMaxSize;

int isNumeric(char* str);
datum* executeCommand(datum* cmd);
int defineVar(char* name, datum* value);
datum* createFuncPoint(datum* (*funcPtr) (datum*));
void initializeTable();
datum* lookupVar(char* name);
datum* executeDatum(datum* expr);
datum* doProcedureCall(char* name, datum* args);
datum* doAdd(datum* args);
datum* doSub(datum* args);
datum* doMul(datum* args);
datum* doDiv(datum* args);
datum* doCar(datum* args);
datum* doCdr(datum* args);
datum* createLambda(datum* args, datum* body);
datum* executeLambda(datum* lam, datum* args);

int areEqual(datum* value1, datum* value2);
void printDatum(datum* value);
void printCons(cons value);
