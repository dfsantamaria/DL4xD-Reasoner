#pragma once
#include <string>
#include <vector>
#include "Structure4LQSR.h"

void testVariablesSet(vector<pair<string, int>>& results);
int testVariablesSetCreation(size_t n, vector<size_t>& vec);
int testVariablesSet_getSorting(size_t n, VariablesSet& varset);

int testVariablesSet_getNumberOfConstantsOfSort(VariablesSet& varset, size_t sort, size_t aspected);
int testVariablesSet_getNumberOfUniQuantifiedVarsOfSort(VariablesSet& varset, size_t sort, size_t aspected);
int testVariablesSet_getNumberOfExQuantifiedVarsOfSort(VariablesSet& varset, size_t sort, size_t aspected);

int testVariablesSet_insertConstant(VariablesSet& varset, vector<pair<string, int>>& names);
int testVariablesSet_insertConstant(VariablesSet& varset, vector<pair<string, int>>& names);

int testVariablesSet_insertUniQuantifiedVariable(VariablesSet& varset, vector<Var>& vars);
int testVariablesSet_insertUniQuantifiedVariable(VariablesSet& varset, vector<pair<string, size_t>>& names);

int testVariablesSet_insertExQuantifiedVariable(VariablesSet& varset, vector<pair<string, size_t>>& names);
int testVariablesSet_insertExQuantifiedVariable(VariablesSet& varset, vector<Var>& vars);

