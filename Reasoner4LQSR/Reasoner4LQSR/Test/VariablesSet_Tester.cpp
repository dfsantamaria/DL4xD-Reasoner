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

int testVariablesSet_getNumberOfUniQuantifiedVarsOfSort(VariablesSet& varset, int sort, int aspected)
{
	if (varset.getNumberOfUniQuantifiedVarsOfSort(sort) != aspected)
		return 1;
	return 0;
};

int testVariablesSet_getNumberOfExQuantifiedVarsOfSort(VariablesSet& varset, int sort, int aspected)
{
	if (varset.getNumberOfExQuantifiedVarsOfSort(sort) != aspected)
		return 1;
	return 0;
};


int testVariablesSet_insertConstant(VariablesSet& varset, vector<Var>& vars)
{
	for (size_t i=0; i<vars.size(); i++)
	{
		int n_elements = (int) varset.getNumberOfConstantsOfSort(vars.at(i).getSort());
		int position=varset.insertConstant(vars.at(i), vars.at(i).getSort());
		int result = 0;
		if (varset.getNumberOfConstantsOfSort(vars.at(i).getSort()) != ++n_elements ) 
			result++; 
		if (position != n_elements)
			result++;
		if (result != 0)
			return result;
	}
	return 0;
}

int testVariablesSet_insertUniQuantifiedVariable(VariablesSet& varset, vector<Var>& vars)
{
	for (size_t i = 0; i < vars.size(); i++)
	{
		int n_elements = (int) varset.getNumberOfUniQuantifiedVarsOfSort(vars.at(i).getSort());
		int position = varset.insertUniQuantifiedVar(vars.at(i), vars.at(i).getSort());
		int result = 0;
		if (varset.getNumberOfUniQuantifiedVarsOfSort(vars.at(i).getSort()) != ++n_elements)
			result++;
		if (position != n_elements)
			result++;
		if (result != 0)
			return result;
	}
	return 0;
}

int testVariablesSet_insertExQuantifiedVariable(VariablesSet& varset, vector<Var>& vars)
{
	for (size_t i = 0; i < vars.size(); i++)
	{
		int n_elements = (int) varset.getNumberOfExQuantifiedVarsOfSort(vars.at(i).getSort());
		int position = varset.insertExQuantifiedVar(vars.at(i), vars.at(i).getSort());
		int result = 0;
		if (varset.getNumberOfExQuantifiedVarsOfSort(vars.at(i).getSort()) != (++n_elements) )
			result++;
		if (position != n_elements)
			result++;
		if (result != 0)
			return result;
	}
	return 0;
}


int testVariablesSet_insertConstant(VariablesSet& varset,  vector<pair<string, int>>& names)
{
	vector<Var> vars=vector<Var>();
	for (pair<string, int> s : names)
		vars.push_back(Var(s.first, s.second, 0, 1));
	return testVariablesSet_insertConstant(varset, vars);
}


int testVariablesSet_insertUniQuantifiedVariable(VariablesSet& varset, vector<pair<string, int>>& names)
{
	vector<Var> vars = vector<Var>();
	for (pair<string, int> s : names)
		vars.push_back(Var(s.first, s.second, 1, 1));
	return testVariablesSet_insertUniQuantifiedVariable(varset, vars);
}

int testVariablesSet_insertExQuantifiedVariable(VariablesSet& varset, vector<pair<string, int>>& names)
{
	vector<Var> vars = vector<Var>();
	for (pair<string, int> s : names)
		vars.push_back(Var(s.first, s.second, 2, 1));
	return testVariablesSet_insertExQuantifiedVariable(varset, vars);
}

void testVariablesSet(vector<pair<string, int>>& results)
{
	//Empty Variables Set Test
	results.push_back(std::make_pair("testVariablesSetCreation input (3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0})", testVariablesSetCreation(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0})));
	results.push_back(std::make_pair("testVariablesSet_getSorting input (3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0})", testVariablesSetCreation(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0})));
	
	//testing constants counter
	results.push_back(std::make_pair("testVariablesSet_getNumberOfConstantsOfSort(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0}, 0, 0)",  
		                        testVariablesSet_getNumberOfConstantsOfSort(VariablesSet(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0}), 0, 0)));
	
	//testing uni. quantified vars counter
	results.push_back(std::make_pair("testVariablesSet_getNumberOfUniQuantifiedVarsOfSort(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0}, 0, 0)",
		testVariablesSet_getNumberOfUniQuantifiedVarsOfSort(VariablesSet(3, vector<size_t>{0, 0, 0, 0,  1, 1, 0, 1,  1, 0, 0, 0}), 0, 0)));

	//testing ex. quantified vars counter
	results.push_back(std::make_pair("testVariablesSet_getNumberOfExQuantifiedOfSort(3, vector<size_t>{1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0}, 0, 0)",
		testVariablesSet_getNumberOfExQuantifiedVarsOfSort(VariablesSet(3, vector<size_t>{0, 0, 0, 0,  0, 0, 0, 0,  1, 0, 1, 1}), 0, 0)));


	//Testing constant insertion	
	int qType = 0;
	results.push_back(std::make_pair("testVariablesSet_insertConstant",	testVariablesSet_insertConstant(VariablesSet(3, 
		                                vector<size_t>{1, 1, 0, 1,   0, 0, 0, 0,   0, 0, 0, 0}),
	                                	vector<Var>{Var("one1", 0, qType, 0), Var("one2", 1, qType, 0), Var("one22", 2, qType, 0), Var("one3", 3, qType, 0)})));

	results.push_back(std::make_pair("testVariablesSet_insertConstant", testVariablesSet_insertConstant(VariablesSet(3,	
		vector<size_t>{1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}),
		vector<pair<string,int>>{std::make_pair("one1", 0), std::make_pair("one2", 1), std::make_pair("one22", 2), std::make_pair("one3",3)})));

	// Testing Univ. quantified variables insertion

	qType = 1;
	results.push_back(std::make_pair("testVariablesSet_insertUniQuantifiedVariable", testVariablesSet_insertUniQuantifiedVariable(VariablesSet(3,
		vector<size_t>{0, 0, 0, 0,  1, 1, 0, 1,  0, 0, 0, 0}),
		vector<Var>{Var("one1", 0, qType, 0), Var("one2", 1, qType, 0), Var("one22", 2, qType, 0), Var("one3", 3, qType, 0)})));

	results.push_back(std::make_pair("testVariablesSet_insertUniquantifiedVariable", testVariablesSet_insertUniQuantifiedVariable(VariablesSet(3,
		vector<size_t>{0, 0, 0, 0,  1, 0, 1, 1,  0, 0, 0, 0}),
		vector<pair<string, int>>{std::make_pair("one1", 0), std::make_pair("one2", 1), std::make_pair("one22", 2), std::make_pair("one3", 3)})));

	// Testing Ex. quantified variables insertion

	qType = 2;
	results.push_back(std::make_pair("testVariablesSet_insertExQuantifiedVariable", testVariablesSet_insertExQuantifiedVariable(VariablesSet(3,
		vector<size_t>{0, 0, 0, 0,  0, 0, 0, 0,  1, 1, 1, 1}),
		vector<Var>{Var("one1", 0, qType, 0), Var("one2", 1, qType, 0), Var("one22", 2, qType, 0), Var("one3", 3, qType, 0)})));

	results.push_back(std::make_pair("testVariablesSet_insertExquantifiedVariable", testVariablesSet_insertExQuantifiedVariable(VariablesSet(3,
		vector<size_t>{0, 0, 0, 0,  0, 0, 0, 0,  1, 1, 1, 1}),
		vector<pair<string, int>>{std::make_pair("one1", 0), std::make_pair("one2", 1), std::make_pair("one22", 2), std::make_pair("one3", 3)})));
}