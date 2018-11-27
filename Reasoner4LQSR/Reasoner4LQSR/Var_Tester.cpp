#include "stdafx.h" // only on windows
#include <string>
#include <sstream>
#include "Reasoner4LQSR.h"
#include "Var_Tester.h"


int testVarCreation()
{
  Var variable1 = Var("x_0", 0, 0, 0);
  string var1representation = variable1.toString();
  if (var1representation.compare("V0{x_0}") ==0)
	  return 0;
  else return 1;
}


void testVar(vector<pair<string, int>>& results)
{	
	results.push_back(std::make_pair( "testVarCreation", testVarCreation()));	
}