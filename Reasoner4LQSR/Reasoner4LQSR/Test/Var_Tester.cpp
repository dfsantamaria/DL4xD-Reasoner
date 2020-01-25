#include <string>
#include <sstream>
#include "Var_Tester.h"


int testVarCreation()
{
  Var variable1 = Var("x_0", 0, 0, 0);  
  return 0;
  
}

int testVar_getName(Var& variable, string name)
{
  string nameVar = variable.getName();
  if (nameVar.compare(name) == 0)
	return 0;
  return 1;
}

int testVar_getSort(Var& variable, int sort)
{	
	if (variable.getSort() == sort)
		return 0;
	return 1;
}


int testVar_setName(Var& variable, string name)
{
	variable.setName(name); 
	if (variable.getName().compare(name) == 0)
	 return 0;
	return 1;
}

int testVar_setSort(Var& variable, int sort)
{
	variable.setSort(sort);
	if (variable.getSort() == sort)
		return 0;
	return 1;
}

int testVar_equal(Var& variable, Var& variable2, int aspected)
{
	int val = variable.equal(&variable2);
	if (val == aspected)
		return 0;
	return 1;
};

int testVar_equal(Var& variable, string name, int sort, int type, int aspected)
{
	int val = variable.equal(name, sort, type);
	if (val == aspected)
		return 0;
	return 1;
};

int testVar_getIndex(Var& variable, int index)
{
	if (variable.getIndex() == index)
		return 0;
	return 1;
}

int testVar_setIndex(Var& variable, int index)
{
	variable.setIndex(index);
	if (variable.getIndex() == index)
		return 0;
	return 1;
}

int testVar_getVarType(Var& variable, int type)
{
	if (variable.getVarType() == type)
		return 0;
	return 1;
}

int testVar_setVarType(Var& variable, int type)
{
	variable.setVarType(type);
	if (variable.getVarType() == type)
		return 0;
	return 1;
}



void testVar(vector<pair<string, int>>& results)
{	
	results.push_back(std::make_pair( "testVarCreation", testVarCreation()));
	results.push_back(std::make_pair("testVar_getName input Var(x_0, 0, 0, 0), x_0", testVar_getName(Var("x_0", 0, 0, 0), "x_0")));
	results.push_back(std::make_pair("testVar_getSort input Var(x_1, 1, 0, 0), 1", testVar_getSort(Var("x_1", 1, 0, 0), 1)));
	results.push_back(std::make_pair("testVar_setName input Var(x_0, 0, 0, 0), newName", testVar_setName(Var("x_0", 0, 0, 0), "newName")));
	results.push_back(std::make_pair("testVar_setSort input Var(x_0, 0, 0, 0), 1", testVar_setSort(Var("x_0", 0, 0, 0), 1)));
	results.push_back(std::make_pair("testVar_getIndex input Var(x_1, 1, 0, 34), 34", testVar_getIndex(Var("x_1", 1, 0, 34), 34)));
	results.push_back(std::make_pair("testVar_setIndex input Var(x_1, 1, 0, 0), 34", testVar_setIndex(Var("x_1", 1, 0, 0), 34)));
	results.push_back(std::make_pair("testVar_getVarType input Var(x_1, 1, 1, 0), 1", testVar_getVarType( Var("x_1", 1, 1, 0), 1)));
	results.push_back(std::make_pair("testVar_setVarType input Var(x_1, 1, 1, 0), 3", testVar_setVarType(Var("x_1", 1, 1, 0), 3)));
	//Checking var equality
	results.push_back(std::make_pair("testVar_equal input Var(one, 1, 0, 0), Var(one, 1, 0, 0), 0 ", testVar_equal(Var("one", 1, 0, 0), Var("one", 1, 0, 0), 0)));
	results.push_back(std::make_pair("testVar_equal input Var(ones, 1, 0, 0), Var(one, 1, 0, 0), 1 ", testVar_equal(Var("ones", 1, 0, 0), Var("one", 1, 0, 0), 1)));
	results.push_back(std::make_pair("testVar_equal input Var(one, 1, 0, 1), Var(one, 1, 0, 5), 0 ", testVar_equal(Var("one", 1, 0, 1), Var("one", 1, 0, 5), 0)));
	results.push_back(std::make_pair("testVar_equal input Var(one, 1, 2, 0), Var(one, 1, 0, 0), 1 ", testVar_equal(Var("one", 1, 2, 0), Var("one", 1, 0, 0), 1)));

	//Checking var equality
	results.push_back(std::make_pair("testVar_equal input Var(one, 1, 0, 0), one, 1, 0, 0, 0 ", testVar_equal(Var("one", 1, 0, 0), "one", 1, 0, 0)));
	results.push_back(std::make_pair("testVar_equal input Var(ones, 1, 0, 0), Var(one, 1, 0, 0), 1 ", testVar_equal(Var("ones", 1, 0, 0), "one", 1, 0, 1)));
	results.push_back(std::make_pair("testVar_equal input Var(one, 1, 0, 1), Var(one, 1, 0, 5), 0 ", testVar_equal(Var("one", 1, 0, 1), "one", 1, 0,  0)));
	results.push_back(std::make_pair("testVar_equal input Var(one, 1, 2, 0), Var(one, 1, 0, 0), 1 ", testVar_equal(Var("one", 1, 2, 0), "one", 1, 0, 1)));

}