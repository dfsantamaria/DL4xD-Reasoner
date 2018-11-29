#pragma once
#include <string>
#include <vector>
#include "Reasoner4LQSR.h"

void testVariablesSet(std::vector<std::pair<std::string, int>>& results);
int testVariablesSetCreation(size_t n, std::vector<size_t>& vec);
int testVariablesSet_getSorting(size_t n, VariablesSet& varset);
int testVariablesSet_getNumberOfConstantsOfSort(VariablesSet& varset, int sort, int aspected);
int testVariablesSet_insertConstant(VariablesSet& varset, std::vector<std::pair<std::string, int>>& names);
int testVariablesSet_insertConstant(VariablesSet& varset, vector<pair<string, int>>& names);