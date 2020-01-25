#include <string>
#include <sstream>
#include "Operators_Tester.h"



int test_getLogConnectSize(Operators& operators, int aspected)
{
	if (operators.getLogConnectSize() == aspected)
		return 0;
	return 1;
}

int test_getSetOpSize(Operators& operators, int aspected)
{
	if (operators.getSetOpSize() == aspected)
		return 0;
	return 1;
}

int test_getQuantSize(Operators& operators, int aspected)
{
	if (operators.getQuantSize() == aspected)
		return 0;
	return 1;
}

int test_getLogConnectValue(Operators& operators, string connect, int aspected)
{
	if (operators.getLogConnectValue(connect) == aspected)
		return 0;
	return 1;
}

int test_getSetOpValue(Operators& operators, string connect, int aspected)
{
	if (operators.getSetOpValue(connect) == aspected)
		return 0;
	return 1;
}

int test_getQuantValue(Operators& operators, string connect, int aspected)
{
	if (operators.getQuantValue(connect) == aspected)
		return 0;
	return 1;
}


int test_getLogConnectElement(Operators& operators, int position, string aspected)
{
	if (operators.getLogConnectElement(position).compare(aspected) == 0)
		return 0;
	return 1;
}

int test_getSetOpElement(Operators& operators, int position, string aspected)
{
	if (operators.getSetOpElement(position).compare(aspected) == 0)
		return 0;
	return 1;
}

int test_getQuantElement(Operators& operators, int position, string aspected)
{
	if (operators.getQuantElement(position).compare(aspected) == 0)
		return 0;
	return 1;
}


void testOperators(vector<pair<string, int>>& results)
{
	Operators operators = Operators();
	//Testing sizes
	results.push_back(std::make_pair("testOperators_getLogConnectSize input ( 8 )", test_getLogConnectSize(operators,8)));
	results.push_back(std::make_pair("testOperators_getSetOpSize input ( 7 ) ", test_getSetOpSize(operators, 7)));
	results.push_back(std::make_pair("testOperators_getQuantSize input ( 2 )", test_getQuantSize(operators, 2)));

	//Testing getLogConnectValue 
	results.push_back(std::make_pair("testOperators_getLogConnectValue input ($AD, 1)", test_getLogConnectValue(operators, "$AD", 1)));
	results.push_back(std::make_pair("testOperators_getLogConnectValue input ($II, 7)", test_getLogConnectValue(operators, "$II", 7)));
	results.push_back(std::make_pair("testOperators_getLogConnectValue input ($GG, -1)", test_getLogConnectValue(operators, "$GG", -1)));

	//Testing getSetOpValue 
	results.push_back(std::make_pair("testOperators_getSetOpValue input ($FA, 2)", test_getSetOpValue(operators, "$NI", 2)));
	results.push_back(std::make_pair("testOperators_getSetOpValue input ($CO, 6)", test_getSetOpValue(operators, "$CO", 6)));
	results.push_back(std::make_pair("testOperators_getSetOpValue input ($GG, -1)", test_getSetOpValue(operators, "$GG", -1)));

	//Testing getQuantValue 
	results.push_back(std::make_pair("testOperators_getQuantValue input ($FA, 0)", test_getQuantValue(operators, "$FA", 0)));
	results.push_back(std::make_pair("testOperators_getQuantValue input ($EX, 1)", test_getQuantValue(operators, "$EX", 1)));
	results.push_back(std::make_pair("testOperators_getQuantValue input ($GG, -1)", test_getQuantValue(operators, "$GG", -1)));

	//Testing getLogConnectElement
	results.push_back(std::make_pair("testOperators_getLogConnectElement input (0, $OR)", test_getLogConnectElement(operators, 0, "$OR")));
	results.push_back(std::make_pair("testOperators_getLogConnectElement input (5, $IF)", test_getLogConnectElement(operators, 5, "$IF")));
	results.push_back(std::make_pair("testOperators_getLogConnectElement input (8, \"\")", test_getLogConnectElement(operators, 8, "")));

	//Testing getSetOpElement
	results.push_back(std::make_pair("testOperators_getSetOpElement input (0, $IN)", test_getSetOpElement(operators, 0, "$IN")));
	results.push_back(std::make_pair("testOperators_getSetOpElement input (6, $CO)", test_getSetOpElement(operators, 6, "$CO")));
	results.push_back(std::make_pair("testOperators_getSetOpElement input (7, \"\")", test_getSetOpElement(operators, 7, "")));

	//Testing getQuantElement
	results.push_back(std::make_pair("testOperators_getQuantElement input (0, $FA)", test_getQuantElement(operators, 0, "$FA")));
	results.push_back(std::make_pair("testOperators_getQuantElement input (1, $EX)", test_getQuantElement(operators, 1, "$EX")));
	results.push_back(std::make_pair("testOperators_getQuantElement input (2, \"\")", test_getQuantElement(operators, 2, "")));
}