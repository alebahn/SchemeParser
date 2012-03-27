#include "semantics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isNumeric(char* str)
{
	int point=0;
	int i;
	for(i=0; i<strlen(str); i++)
	{
		if(!isdigit(str[i]))
			if(str[i]=='.')
				if(point)
					return 0;
				else
					point=1;
			else
				return 0;
	}
	return 1;
}

datum* createFuncPoint(datum* (*funcPtr) (datum*))
{
	datum* result=malloc(sizeof(datum));
	*result=(datum){D_LAMBDA, {.valLambda=(lambda){-1, {.funcPtr=funcPtr}}}};
	return result;
}
void initializeTable()
{
	tableMaxSize=8;
	tableSize=0;
	symbolTable=malloc(sizeof(struct tableRecord)*tableMaxSize);
	symbolTable[tableSize]=(struct tableRecord){"car",createFuncPoint(doCar)};
	++tableSize;
	symbolTable[tableSize]=(struct tableRecord){"cdr",createFuncPoint(doCdr)};
	++tableSize;
	symbolTable[tableSize]=(struct tableRecord){"+",createFuncPoint(doAdd)};
	++tableSize;
	symbolTable[tableSize]=(struct tableRecord){"-",createFuncPoint(doSub)};
	++tableSize;
	symbolTable[tableSize]=(struct tableRecord){"*",createFuncPoint(doMul)};
	++tableSize;
	symbolTable[tableSize]=(struct tableRecord){"/",createFuncPoint(doDiv)};
	++tableSize;
}

int defineVar(char* name, datum* value)
{
	int i;
	for(i=0; i<tableSize; ++i)
	{
		if(strcmp(name, symbolTable[i].name)==0)
		{
			symbolTable[i].value=value;
			return 1;
		}
	}
	if(tableSize>=tableMaxSize)
	{
		tableMaxSize*=2;
		symbolTable=realloc(symbolTable, sizeof(struct tableRecord)*tableMaxSize);
		if(symbolTable==NULL)
			return 0;
	}
	char* nameCpy=malloc(sizeof(char)*(strlen(name)+1));
	strcpy(nameCpy, name);
	symbolTable[tableSize]=(struct tableRecord){nameCpy, value};
	++tableSize;
	return 1;
}
datum* lookupVar(char* name)
{
	int i;
	for(i=0; i<tableSize; ++i)
	{
		if(strcmp(name, symbolTable[i].name)==0)
		{
			return symbolTable[i].value;
		}
	}
	return NULL;
}

datum* replaceVars(datum* body)
{
	if(body->quote)
		return body;
	datum *car, *cdr, *result;
	switch(body->type)
	{
	case D_STR:
		return lookupVar(body->valStr);
	case D_CONS:
		car=replaceVars(body->valCons.car);
		if(car==NULL)
			return NULL;
		cdr=replaceVars(body->valCons.cdr);
		if(car==NULL)
			return NULL;
		result=malloc(sizeof(datum));
		*result=(datum){D_CONS, {.valCons=(cons){car, cdr}}};
		return result;
	default:
		return body;
	}
}
datum* executeCommand(datum* cmd)
{
	datum* expr = replaceVars(cmd);
	return executeDatum(expr);
}
datum* doQuote(datum* dtm)
{
	datum* result = malloc(sizeof(datum));
	*result=*dtm;
	switch(result->type)
	{
	case D_CONS:
	case D_STR:
	case D_NULL:
		result->quote=1;
		break;
	}
	return result;
}

datum* doProcedureCall(char* name, datum* args)
{
	if(args==NULL)
		return NULL;
	if(args->type!=D_CONS)
		return NULL;
	return executeLambda(lookupVar(name), args);
}
datum* executeDatum(datum* expr)
{
	if(expr==NULL)
		return NULL;
	if(expr->type==D_CONS)
	{
		return executeLambda(expr->valCons.car, expr->valCons.cdr);
	}
	else
		return expr;
}

datum* setupArith(datum* args, int identity, datum* (*recurFunc)(datum*), enum DATUM_TYPE *resultType, int* arg1int, int* arg2int, float* arg1float, float* arg2float)
{
	cons argCons=args->valCons;
	if(argCons.car==NULL)
		return NULL;
	datum *arg1, *arg2;
	*resultType = D_INT;
	datum* result;
	if(argCons.cdr->type==D_NULL)
	{
		result=malloc(sizeof(datum));
		arg1=(datum[1]){{D_INT, {.valInt=identity}}};
		arg2=argCons.car;
	}
	else
	{
		arg1=argCons.car;
		arg2=recurFunc(argCons.cdr);
		result=arg2;
	}
	if(arg2==NULL)
		return NULL;
	if(arg1->type==D_FLOAT)
	{
		*resultType=D_FLOAT;
		*arg1float=arg1->valFloat;
	}
	else if(arg1->type==D_INT)
		*arg1float=*arg1int=arg1->valInt;
	else
		return NULL;
	if(arg2->type==D_FLOAT)
	{
		*resultType=D_FLOAT;
		*arg2float=arg2->valFloat;
	}
	else if(arg2->type==D_INT)
		*arg2float=*arg2int=arg2->valInt;
	else
		return NULL;
	*result=(datum){*resultType};
	return result;
}

void finishArith(enum DATUM_TYPE resultType, datum* result, int resultInt, float resultFloat)
{
	if(result == NULL)
		return;
	switch(resultType)
	{
	case D_INT:
		result->valInt=resultInt;
		break;
	case D_FLOAT:
		result->valFloat=resultFloat;
	}
}
datum* doAdd(datum* args)
{
	enum DATUM_TYPE resultType;
	int resultInt, arg1int, arg2int;
	float resultFloat, arg1float, arg2float;
	datum* result= setupArith(args, 0, doAdd, &resultType, &arg1int, &arg2int, &arg1float, &arg2float);
	resultFloat=arg1float+arg2float;
	resultInt=arg1int+arg2int;
	finishArith(resultType, result, resultInt, resultFloat);
	return result;
}
datum* doSub(datum* args)
{
	enum DATUM_TYPE resultType;
	int resultInt, arg1int, arg2int;
	float resultFloat, arg1float, arg2float;
	datum* result= setupArith(args, 0, doAdd, &resultType, &arg1int, &arg2int, &arg1float, &arg2float);
	resultFloat=arg1float-arg2float;
	resultInt=arg1int-arg2int;
	finishArith(resultType, result, resultInt, resultFloat);
	return result;
}
datum* doMul(datum* args)
{
	enum DATUM_TYPE resultType;
	int resultInt, arg1int, arg2int;
	float resultFloat, arg1float, arg2float;
	datum* result= setupArith(args, 1, doMul, &resultType, &arg1int, &arg2int, &arg1float, &arg2float);
	resultFloat=arg1float*arg2float;
	resultInt=arg1int*arg2int;
	finishArith(resultType, result, resultInt, resultFloat);
	return result;
}
datum* doDiv(datum* args)
{
	enum DATUM_TYPE resultType;
	int resultInt, arg1int, arg2int;
	float resultFloat, arg1float, arg2float;
	datum* result= setupArith(args, 1, doMul, &resultType, &arg1int, &arg2int, &arg1float, &arg2float);
	if(arg2float==0 || arg2int==0)
		return NULL;
	resultFloat=arg1float/arg2float;
	resultInt=arg1int/arg2int;
	finishArith(resultType, result, resultInt, resultFloat);
	return result;
}
datum* doCar(datum* args)
{
	if(args==NULL)
		return NULL;
	if(args->type!=D_CONS)
		return NULL;
	if(args->valCons.cdr->type!=D_NULL)
		return NULL;
	datum* arg=args->valCons.car;
	return arg->valCons.car;
}
datum* doCdr(datum* args)
{
	if(args==NULL)
		return NULL;
	if(args->type!=D_CONS)
		return NULL;
	if(args->valCons.cdr->type!=D_NULL)
		return NULL;
	datum* arg=args->valCons.car;
	return arg->valCons.cdr;
}
datum* replaceArg(datum* args, datum* arg)
{
	datum* ptr=args;
	int i=0;
	while(ptr->type!=D_NULL)
	{
		datum* car=ptr->valCons.car;
		if(car->type!=D_STR)
			return NULL;
		if(strcmp(car->valStr,arg->valStr)==0)
		{
			datum* result=malloc(sizeof(datum));
			*result=(datum){D_LAM_ARG, {.valInt=i}};
			return result;
		}
		ptr=ptr->valCons.cdr;
		++i;
	}
	return lookupVar(arg->valStr);

}
datum* replaceLamArgs(datum* args, datum* body)
{
	datum *car, *cdr, *result;
	switch(body->type)
	{
	case D_STR:
		return replaceArg(args, body);
	case D_CONS:
		car=replaceLamArgs(args, body->valCons.car);
		if(car==NULL)
			return NULL;
		cdr=replaceLamArgs(args, body->valCons.cdr);
		if(car==NULL)
			return NULL;
		result=malloc(sizeof(datum));
		*result=(datum){D_CONS, {.valCons=(cons){car, cdr}}};
		return result;
	default:
		return body;
	}
}
int getListSize(datum* list)
{
	datum* ptr=list;
	int size=0;
	while(ptr->type!=D_NULL)
	{
		ptr=ptr->valCons.cdr;
		++size;
	}
	return size;
}
datum* createLambda(datum* args, datum* body)
{
	if(args==NULL || body==NULL)
		return NULL;
	if(args->type!=D_CONS || body->type!=D_CONS)
		return NULL;
	int numArgs=getListSize(args);
	datum* newBody=replaceLamArgs(args, body);
	if(newBody==NULL)
		return NULL;
	datum* result=malloc(sizeof(datum));
	*result=(datum){D_LAMBDA, {.valLambda=(lambda){numArgs, newBody}}};
	return result;
}
datum* initializeArgs(datum* body, datum** args)
{
	datum* result;
	switch(body->type)
	{
	case D_LAM_ARG:
		return args[body->valInt];
	case D_CONS:
		result=malloc(sizeof(datum));
		*result=(datum){D_CONS, {.valCons=(cons){initializeArgs(body->valCons.car, args), initializeArgs(body->valCons.cdr, args)}}};
		return result;
	default:
		return body;
	}
}
datum* executeLambda(datum* lam, datum* args)
{
	if(lam==NULL || args==NULL)
		return NULL;
	int numArgs=lam->valLambda.numArgs;
	if(numArgs==-1)
		return lam->valLambda.funcPtr(args);
	if(getListSize(args)!=numArgs)
		return NULL;
	datum** argsArray=malloc(sizeof(datum*)*numArgs);
	datum* ptr=args;
	int i=0;
	while(ptr->type!=D_NULL)
	{
		argsArray[i]=ptr->valCons.car;
		ptr=ptr->valCons.cdr;
		++i;
	}
	datum* initializedBody=initializeArgs(lam->valLambda.body, argsArray);
	executeDatum(initializedBody);
}

int areEqual(datum* value1, datum* value2)
{
	if(value1==NULL || value2==NULL)
		return value1==value2;
	if(value1->type!=value2->type)
		return 0;
	switch(value1->type)
	{
	case D_STR:
		return strcmp(value1->valStr,value2->valStr)==0;
	case D_LAM_ARG:
	case D_INT:
		return value1->valInt==value2->valInt;
	case D_FLOAT:
		return value1->valFloat==value2->valFloat;
	case D_CONS:
		return areEqual(value1->valCons.car, value2->valCons.car) &&areEqual(value1->valCons.car, value2->valCons.car);
	case D_LAMBDA:
		if(value1->valLambda.numArgs==-1)
			return value1->valLambda.numArgs==value2->valLambda.numArgs && value1->valLambda.funcPtr==value2->valLambda.funcPtr;
		else
			return value1->valLambda.numArgs==value2->valLambda.numArgs && areEqual(value1->valLambda.body, value2->valLambda.body);
	case D_NULL:
		return 1;
	}
}

void printDatum(datum* value)
{
	if(value==NULL)
		printf("ERROR");
	else
	{
		char* procName;
		switch(value->type)
		{
		case D_STR:
			printf("%s", value->valStr);
			break;
		case D_INT:
			printf("%d", value->valInt);
			break;
		case D_FLOAT:
			printf("%f", value->valFloat);
			break;
		case D_CONS:
			printf("(");
			printCons(value->valCons);
			printf(")");
			break;
		case D_LAM_ARG:
			printf("_ARG%d", value->valInt);
			break;
		case D_LAMBDA:
#ifdef DEBUG
			if(value->valLambda.numArgs!=-1)
			{
				printf("(lambda %d ", value->valLambda.numArgs);
				printDatum(value->valLambda.body);
				printf(")");
			}
			else
			{
				if(value->valLambda.funcPtr==doAdd)
					procName="+";
				else if(value->valLambda.funcPtr==doSub)
					procName="-";
				else if(value->valLambda.funcPtr==doMul)
					procName="*";
				else if(value->valLambda.funcPtr==doDiv)
					procName="/";
				else
					procName="unknown";
				printf("#<c_procedure %s>", procName);
			}
#else
			printf("#<procedure>");
#endif
			break;
		case D_NULL:
			printf("()");
		}
	}
}

void printCons(cons value)
{
	datum* car=value.car;
	datum* cdr=value.cdr;
	printDatum(car);
	switch(cdr->type)
	{
	case D_CONS:
		printf(" ");
		printCons(cdr->valCons);
		break;
	case D_NULL:
		break;
	default:
		printf(" . ");
		printDatum(cdr);
	}
}
