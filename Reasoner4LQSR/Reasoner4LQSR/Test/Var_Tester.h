#pragma once
#include <string>
#include <vector>
#include "..\Structure4LQSR.h"

void testVar(std::vector<std::pair<std::string, int>>& results);
int testVar_getName(Var& variable, std::string name);
int testVar_setName(Var& variable, std::string name);
int testVar_getSort(Var& variable, int sort);
int testVar_setSort(Var& variable, int sort);
int testVar_equal(Var& variable, Var& variable2, int aspected);
int testVar_equal(Var& variable, std::string name, int sort, int type, int aspected);
int testVar_getIndex(Var& variable, int index);
int testVar_setIndex(Var& variable, int index);
int testVar_getVarType(Var& variable, int type);
int testVar_setVarType(Var& variable, int type);
int testVarCreation();
