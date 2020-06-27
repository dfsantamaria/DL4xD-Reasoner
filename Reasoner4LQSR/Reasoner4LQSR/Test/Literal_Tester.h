#pragma once

#include <string>
#include <vector>
#include "../Structure4LQSR.h"


int testLiteral_getElements(Literal &literal, Var* &var);
int testLiteral_getLiteralOp(Literal &literal, int checkLiteralOp);
int testLiteral_setLiteralOp(Literal &literal, int checkLiteralOp);
int testLiteral_getElementAt(Literal &literal, size_t position, Var* check);
int testLiteral_equals(Literal &literal, Literal& literal2, int check);
int testLiteral_addElement(Literal literal, Var* var);
int testLiteral_setElementAt(Literal literal, Var* var, size_t position);

void testLiteral(vector<pair<string, int>>& results);
