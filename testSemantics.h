#pragma once

#include "semantics.h"

datum genListFromArray(datum* list, int len);
void deallocateList(datum* list);

int testOneIsNumeric(char* input, int expected);
int testIsNumeric();
int testOneAreEqual(datum* val1, datum* val2, int expected);
int testAreEqual();
int testOneQuote(datum* in, int expected);
int testDoQuote();
int testOneVar(char* varName, datum* var);
int testVars();
int testOneProcedure(char* op, datum* args, int numArg, datum* expected);
int testProcedureNullArg();
int testDoProcedureCalls();
int testOneCar(datum* arg, datum* expected);
int testDoCar();
int testOneCdr(datum* arg, datum* expected);
int testDoCdr();
int testCreateOneLambda(datum* args, datum* expr, datum* expected);
int testCreateLambda();
int testExecuteOneLambda(datum* func, datum* args, datum* expected);
int testExecuteLambda();
int testDefineLambda();
