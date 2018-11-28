#pragma once
#include "stdafx.h" // only on windows
#include "Var_Tester.h"
#include "VariablesSet_Tester.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void printOutput(vector<pair<string, int>> output)
{
	int results = 0;
	for (pair< string, int> p : output)
	{
		cout << "Test " << p.first << " result " << p.second<<endl;
		results += p.second;
	}
	cout << "Final result: " << results << endl;
}

int main()
{
	vector<pair< string, int>> output = vector<pair< string, int>>(0);
	testVar(output);
	testVariablesSet(output);
	printOutput(output);
}
