#include "testSemantics.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
	int result=1;
	result&=testIsNumeric();
	result&=testAreEqual();
	result&=testVars();
	result&=testDoProcedureCalls();
	result&=testDoCar();
	result&=testDoCdr();
	result&=testCreateLambda();
	result&=testExecuteLambda();
	result&=testDefineLambda();
	printf("****TESTS COMPLETED WITH %s****\n",result?"SUCCESS":"FAILURE");
	return 0;
}

int testOneIsNumeric(char* input, int expected)
{
	int output=isNumeric(input);
	printf("is %s numeric? %d expected %d.\n",input,output,expected);
	return output==expected;
}
int testIsNumeric()
{
	int result=1;

	result &= testOneIsNumeric("one", 0);
	result &= testOneIsNumeric("1.2.3", 0);
	result &= testOneIsNumeric("14", 1);
	result &= testOneIsNumeric("14.", 1);
	result &= testOneIsNumeric(".25", 1);
	result &= testOneIsNumeric("24.6", 1);

	printf("*isNumeric %s*\n",result?"PASSED":"FAILED");
	return result;
}

int testOneAreEqual(datum* val1, datum* val2, int expected)
{
	printf("are ");
	printDatum(val1);
	printf(" and ");
	printDatum(val2);
	int output=areEqual(val1, val2);
	printf(" equal? %d expected %d\n", output, expected);
	return output==expected;
}
int testAreEqual()
{
	int result=1;
	datum val1, val2;
	val2=val1=(datum){D_INT, {.valInt=7}};
	result&=testOneAreEqual(&val1, &val2, 1);
	val2=val1=(datum){D_FLOAT, {.valFloat=2.5}};
	result&=testOneAreEqual(&val1, &val2, 1);
	val1=(datum){D_STR, {.valStr="Weewoo"}};
	val2=(datum){D_STR, {.valStr="Weewoo"}};
	result&=testOneAreEqual(&val1, &val2, 1);
	val2=val1=(datum){D_NULL};
	result&=testOneAreEqual(&val1, &val2, 1);
	val2=(datum){D_STR, {.valStr="Weewoo"}};
	result&=testOneAreEqual(&val1, &val2, 0);
	datum a1a={D_STR, {.valStr="*"}};
	datum a2a={D_LAM_ARG, {.valInt=0}};
	datum a3a={D_LAM_ARG, {.valInt=1}};
	datum ana={D_NULL};
	datum c3a={D_CONS, {.valCons={&a3a, &ana}}};
	datum c2a={D_CONS, {.valCons={&a2a, &c3a}}};
	datum c1a={D_CONS, {.valCons={&a1a, &c2a}}};
	datum a1b={D_STR, {.valStr="*"}};
	datum a2b={D_LAM_ARG, {.valInt=0}};
	datum a3b={D_LAM_ARG, {.valInt=1}};
	datum anb={D_NULL};
	datum c3b={D_CONS, {.valCons={&a3b, &anb}}};
	datum c2b={D_CONS, {.valCons={&a2b, &c3b}}};
	datum c1b={D_CONS, {.valCons={&a1b, &c2b}}};
	val1=(datum){D_LAMBDA, {.valLambda={2, &c1a}}};
	val2=(datum){D_LAMBDA, {.valLambda={2, &c1b}}};
	result&=testOneAreEqual(&val1, &val2, 1);
	val1=(datum){D_INT, {.valInt=3}};
	result&=testOneAreEqual(&val1, NULL, 0);
	result&=testOneAreEqual(NULL, NULL, 1);
	printf("*areEqual %s*\n",result?"PASSED":"FAILED");
	return result;
}

int testOneVar(char* varName, datum* var)
{
	if(var!=NULL)
	{
		defineVar(varName, var);
		
		printf("defining %s as ", varName);
		printDatum(var);
		printf("\n");
	}
	else
	{
		printf("not defining %s\n", varName);
	}
	datum* pvar=lookupVar(varName);
	
	printf("result value: ");
	printDatum(pvar);
	printf("\n");
	return areEqual(var, pvar);
}
int testVars()
{
	initializeTable();
	int result=1;

	result&=testOneVar("var1",(datum[1]){{D_INT, {.valInt=5}}});

	result&=testOneVar("var2",(datum[1]){{D_STR, {.valStr="TEST"}}});

	result&=testOneVar("var3",(datum[1]){{D_NULL}});

	result&=testOneVar("var4",NULL);

	printf("*testVars %s*\n",result?"PASSED":"FAILED");
	return result;
}

int testOneProcedure(char* op, datum* args, int numArg, datum* expected)
{
	datum dArgs=genListFromArray(args, numArg);
	printf("Performing %s on ", op);
	printDatum(&dArgs);

	datum* out=doProcedureCall(op, &dArgs);
	printf(": ");
	printDatum(out);
	printf(" expected ");
	printDatum(expected);
	printf("\n");
	return areEqual(out, expected);
}
int testDoProcedureCalls()
{
	initializeTable();
	int result=1;
	datum test1[]={{D_INT, {.valInt=1}}, {D_INT, {.valInt=3}}};
	datum expected={D_INT, {.valInt=4}};
	result&=testOneProcedure("+", test1, 2, &expected);
	datum test2[]={{D_INT, {.valInt=5}}, {D_INT, {.valInt=2}}};
	expected=(datum){D_INT, {.valInt=3}};
	result&=testOneProcedure("-", test2, 2, &expected);
	datum test3[]={{D_INT, {.valInt=2}}, {D_INT, {.valInt=3}},
		{D_INT, {.valInt=4}}};
	expected=(datum){D_INT, {.valInt=24}};
	result&=testOneProcedure("*", test3, 3, &expected);
	datum test4[]={{D_INT, {.valInt=20}}, {D_INT, {.valInt=4}}};
	expected=(datum){D_INT, {.valInt=5}};
	result&=testOneProcedure("/", test4, 2, &expected);
	datum test5[]={{D_INT, {.valInt=2}}, {D_INT, {.valInt=0}}};
	result&=testOneProcedure("/", test5, 2, NULL);
	datum test6[]={{D_FLOAT, {.valFloat=4}}};
	expected=(datum){D_FLOAT, {.valFloat=.25}};
	result&=testOneProcedure("/", test6, 1, &expected);
	datum test7[]={{D_INT, {.valInt=3}}, {D_INT, {.valInt=4}},
		{D_INT, {.valInt=5}}};
	expected=(datum){D_INT, {.valInt=-6}};
	result&=testOneProcedure("-", test7, 3, &expected);
	result&=testOneProcedure("+", NULL, 0, NULL);
	printf("*testDoProcedureCalls %s*\n",result?"PASSED":"FAILED");
	return result;
}

int testOneCar(datum* arg, datum* expected)
{
	datum* out = doCar(arg);
	printf("Testing car on ");
	printDatum(arg);
	printf(": ");
	printDatum(out);
	printf(" expected ");
	printDatum(expected);
	printf("\n");
	return areEqual(out, expected);
}
int testDoCar()
{
	int result=1;
	datum dArg=(datum){D_INT, {.valInt=4}};
	datum arg=genListFromArray(&dArg, 1);
	result&=testOneCar(&arg, &dArg);
	deallocateList(&arg);
	datum car=genListFromArray((datum[3]){{D_INT, {.valInt=1}},
			{D_INT, {.valInt=2}}, {D_INT, {.valInt=3}}}, 3);
	datum cdr={D_FLOAT, {.valFloat=2.5}};
	result&=testOneCar((datum[1]){{D_CONS, {.valCons={&car, &cdr}}}}, &car);
	deallocateList(&car);
	car=(datum){D_STR, {.valStr="CAR"}};
	cdr=(datum){D_STR, {.valStr="CDR"}};
	result&=testOneCar((datum[1]){{D_CONS, {.valCons={&car, &cdr}}}}, &car);
	result&=testOneCar(&car, NULL);
	result&=testOneCar(NULL, NULL);
	printf("*testDoCar %s*\n",result?"PASSED":"FAILED");
	return result;
}

int testOneCdr(datum* arg, datum* expected)
{
	datum* out = doCdr(arg);
	printf("Testing cdr on ");
	printDatum(arg);
	printf(": ");
	printDatum(out);
	printf(" expected ");
	printDatum(expected);
	printf("\n");
	return areEqual(out, expected);
}
int testDoCdr()
{
	int result=1;
	int iArg=4;
	datum dArg=(datum){D_INT, {.valInt=4}};
	datum arg=genListFromArray(&dArg, 1);
	result&=testOneCdr(&arg, (datum[1]){{D_NULL}});
	deallocateList(&arg);
	arg=genListFromArray((datum[3]){{D_INT, {.valInt=1}},
			{D_INT, {.valInt=2}}, {D_INT, {.valInt=3}}}, 3);
	datum cdr=genListFromArray((datum[2]){{D_INT, {.valInt=2}},
			{D_INT, {.valInt=3}}}, 2);
	result&=testOneCdr(&arg, &cdr);
	deallocateList(&arg);
	deallocateList(&cdr);
	datum car={D_STR, {.valStr="CAR"}};
	cdr=(datum){D_STR, {.valStr="CDR"}};
	result&=testOneCdr((datum[1]){{D_CONS, {.valCons={&car, &cdr}}}}, &cdr);
	result&=testOneCdr(&cdr, NULL);
	result&=testOneCdr(NULL, NULL);
	printf("*testDoCdr %s*\n",result?"PASSED":"FAILED");
	return result;
}
int testCreateOneLambda(datum* args, datum* expr, datum* expected)
{
	datum* out=createLambda(args, expr);
	printf("Testing create (lambda ");
	printDatum(args);
	printf(" ");
	printDatum(expr);
	printf(") result ");
	printDatum(out);
	printf(" expected ");
	printDatum(expected);
	printf("\n");
	return areEqual(out, expected);
}
int testCreateLambda()
{
	int result=1;
	datum args=genListFromArray((datum[1]){{D_STR,{.valStr="x"}}},1);
	datum body=genListFromArray((datum[3]){{D_STR,{.valStr="*"}},
			{D_STR,{.valStr="x"}}, {D_STR,{.valStr="x"}}},3);
	datum* funcPoint=createFuncPoint(doMul);
	datum expecBody=genListFromArray((datum[3]){*funcPoint,
			{D_LAM_ARG,{.valInt=0}}, {D_LAM_ARG,{.valInt=0}}},3);
	datum expect={D_LAMBDA, {.valLambda=(lambda){1, &expecBody}}};
	result&=testCreateOneLambda(&args, &body, &expect);
	deallocateList(&args);
	deallocateList(&body);
	deallocateList(&expecBody);
	free(funcPoint);

	args=genListFromArray((datum[2]){{D_STR,{.valStr="x"}},
			{D_STR,{.valStr="y"}}},2);
	body=genListFromArray((datum[3]){{D_STR,{.valStr="*"}},
			{D_STR,{.valStr="x"}}, {D_STR,{.valStr="y"}}},3);
	funcPoint=createFuncPoint(doMul);
	expecBody=genListFromArray((datum[3]){*funcPoint,
			{D_LAM_ARG,{.valInt=0}}, {D_LAM_ARG,{.valInt=1}}},3);
	expect=(datum){D_LAMBDA, {.valLambda=(lambda){2, &expecBody}}};
	result&=testCreateOneLambda(&args, &body, &expect);
	result&=testCreateOneLambda(&args, NULL, NULL);
	result&=testCreateOneLambda(NULL, &body, NULL);
	deallocateList(&args);
	deallocateList(&body);
	deallocateList(&expecBody);
	free(funcPoint);

	args=genListFromArray((datum[2]){{D_INT,{.valInt=1}},
			{D_INT,{.valInt=3}}},2);
	body=genListFromArray((datum[3]){{D_STR,{.valStr="*"}},
			{D_STR,{.valStr="x"}}, {D_STR,{.valStr="y"}}},3);
	result&=testCreateOneLambda(&args, &body, NULL);
	deallocateList(&args);
	deallocateList(&body);

	printf("*testCreateLambda %s*\n",result?"PASSED":"FAILED");
	return result;
}

int testExecuteOneLambda(datum* func, datum* args, datum* expected)
{
	datum *out=executeLambda(func, args);
	printf("Testing execute ");
	printDatum(func);
	printf(" with args ");
	printDatum(args);
	printf(" result ");
	printDatum(out);
	printf(" expected ");
	printDatum(expected);
	printf("\n");
	return areEqual(out, expected);
}
int testExecuteLambda()
{
	int result;
	datum* funcPoint=createFuncPoint(doAdd);
	datum body=genListFromArray((datum[3]){*funcPoint,
			{D_LAM_ARG, {.valInt=0}}, {D_INT, {.valInt=5}}}, 3);
	datum args=genListFromArray((datum[1]){{D_INT, {.valInt=4}}}, 1);
	datum lam={D_LAMBDA, {.valLambda=(lambda){1, &body}}};
	datum expected={D_INT, {.valInt=9}};
	result&=testExecuteOneLambda(&lam, &args, &expected);

	funcPoint=createFuncPoint(doMul);
	body=genListFromArray((datum[3]){*funcPoint,
			{D_LAM_ARG, {.valInt=0}}, {D_LAM_ARG, {.valInt=1}}}, 3);
	args=genListFromArray((datum[2]){{D_INT, {.valInt=2}}, {D_INT, {.valInt=3}}}, 2);
	lam=(datum){D_LAMBDA, {.valLambda=(lambda){2, &body}}};
	expected=(datum){D_INT, {.valInt=6}};
	result&=testExecuteOneLambda(&lam, &args, &expected);

	funcPoint=createFuncPoint(doSub);
	body=genListFromArray((datum[3]){*funcPoint,
			{D_LAM_ARG, {.valInt=0}}, {D_LAM_ARG, {.valInt=1}}}, 3);
	args=genListFromArray((datum[2]){{D_INT, {.valInt=6}}, {D_INT, {.valInt=2}}}, 2);
	expected=(datum){D_INT, {.valInt=4}};
	lam=(datum){D_LAMBDA, {.valLambda=(lambda){2, &body}}};
	result&=testExecuteOneLambda(&lam, &args, &expected);

	funcPoint=createFuncPoint(doDiv);
	body=genListFromArray((datum[3]){*funcPoint,
			{D_LAM_ARG, {.valInt=0}}, {D_LAM_ARG, {.valInt=1}}}, 3);
	args=genListFromArray((datum[2]){{D_INT, {.valInt=12}}, {D_INT, {.valInt=4}}}, 2);
	expected=(datum){D_INT, {.valInt=3}};
	lam=(datum){D_LAMBDA, {.valLambda=(lambda){2, &body}}};
	result&=testExecuteOneLambda(&lam, &args, &expected);

	args=genListFromArray((datum[2]){{D_FLOAT, {.valFloat=5}}, {D_INT, {.valInt=2}}}, 2);
	expected=(datum){D_FLOAT, {.valFloat=2.5}};
	result&=testExecuteOneLambda(&lam, &args, &expected);

	args=genListFromArray((datum[1]){{D_INT, {.valInt=3}}}, 1);
	result&=testExecuteOneLambda(&lam, &args, NULL);
	result&=testExecuteOneLambda(NULL, &args, NULL);
	result&=testExecuteOneLambda(&lam, NULL, NULL);
	printf("*testExecuteLambda %s*\n",result?"PASSED":"FAILED");
	return result;
}

int testDefineLambda()
{
	initializeTable();
	datum lamBody=genListFromArray((datum[3]){{D_STR, {.valStr="*"}},
			{D_STR, {.valStr="x"}}, {D_STR, {.valStr="x"}}}, 3);
	datum lamArgs=genListFromArray((datum[1]){{D_STR, {.valStr="x"}}}, 1);
	datum *lam=createLambda(&lamArgs, &lamBody);
	printf("TestDefineLambda:\nCreating (lambda ");
	printDatum(&lamArgs);
	printf(" ");
	printDatum(&lamBody);
	printf(") result: ");
	printDatum(lam);
	printf("\n");

	char* lamName="sq";
	defineVar(lamName, lam);
	printf("defining lambda as %s\n", lamName);
	datum *pLam=lookupVar(lamName);
	printf("recalling, result: ");
	printDatum(pLam);
	printf("\n");

	int result=areEqual(lam,pLam);

	datum args=genListFromArray((datum[1]){{D_INT, {.valInt=4}}}, 1);
	datum expected={D_INT, {.valInt=16}};
	datum* out=executeLambda(pLam, &args);
	printf("executing lambda with args ");
	printDatum(&args);
	printf(" result: ");
	printDatum(out);
	printf(" expected ");
	printDatum(&expected);
	printf("\n");

	result&=areEqual(out,&expected);

	printf("*testDefineLambda %s*\n",result?"PASSED":"FAILED");
	return result;
}

datum genListFromArray(datum* list, int len)
{
	datum result;
	datum* ptr=&result;
	int i;
	for(i=0; i<len; i++)
	{
		datum* car=malloc(sizeof(datum));
		datum* cdr=malloc(sizeof(datum));
		*car=list[i];
		*ptr=(datum){D_CONS, {.valCons={car, cdr}}};
		ptr->valCons.car=car;
		ptr->valCons.cdr=cdr;
		ptr=cdr;
	}
	*ptr=(datum){D_NULL};
	return result;
}

void deallocateList(datum *list)
{
	if(list->type!=D_NULL)
	{
		deallocateList(list->valCons.cdr);
		free(list->valCons.car);
		free(list->valCons.cdr);
	}
}
