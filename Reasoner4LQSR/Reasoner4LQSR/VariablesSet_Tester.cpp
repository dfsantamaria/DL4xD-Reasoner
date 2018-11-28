#include "stdafx.h" // only on windows
#include <string>
#include <sstream>
#include "VariablesSet_Tester.h"


int testVariablesSetCreation(size_t n, vector<size_t>& vec)
{
	VariablesSet* varset = new VariablesSet(n, vec);
	delete varset;
	return 0;
}

int testVariablesSet_getSorting(size_t n, VariablesSet& varset)
{	
	if (varset.getSorting() != n)
	  return 1;	
	return 0;
}

int testVariablesSet_getNumberOfConstantsOfSort(VariablesSet& varset, int sort, int aspected)
{	
	if (varset.getNumberOfConstantsOfSort(sort) != aspected)			
		return 1;
	return 0;
};

//to do insertConstant

void testVariablesSet(vector<pair<string, int>>& results)
{
	//Empty Variables Set Test
	results.push_back(std::make_pair("testVariablesSetCreation input (3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0})", testVariablesSetCreation(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0})));
	results.push_back(std::make_pair("testVariablesSet_getSorting input (3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0})", testVariablesSetCreation(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0})));
	results.push_back(std::make_pair("testVariablesSet_getNumberOfConstantsOfSort(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0}, 0, 0)",  
		                        testVariablesSet_getNumberOfConstantsOfSort(VariablesSet(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0}), 0, 0)));
	//
}