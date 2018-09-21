
#include "stdafx.h" // only on windows
#include <fstream>
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <array>
#include <stack>
#include <sstream>
#include "Reasoner4LQSR.h"

using namespace std;

/// <summary>
/// This Class define a single 4LQSR variable
/// </summary>
/// <param name="_name"> Name of the variable</param>
/// <param name="_type"> Quantified (value 1) or constant (value 0)</param>
/// <param name="_var">Sort of the variable</param>
/// <param name="_index">Position in the vector</param>
Var::Var(string _name, int _type, int _var, int _index)
{
	setName(_name);
	setType(_type);
	setVarType(_var);
	index = _index;
};
Var::~Var() {};
//int isValidType() { return isValidType(type);};
//int isValidVar() { return isValidVar(var); };
int Var::getType() { return type; };
string Var::getName() { return name; };
int Var::getVarType() { return var; };
int Var::getIndex() { return index; };
void Var::setIndex(int _index) { index = _index; }
int Var::setName(string _name)
{
	name = _name;
	return type;
};
void Var::setType(int _type)
{
	type = _type;
};

void Var::setVarType(int _var)
{
	var = _var;
};

int Var::equal(Var* match)
{
	if ((getName().compare(match->getName()) == 0) && (getType() == match->getType()) && (getVarType() == match->getVarType()))
		return 0;
	return 1;
};
int Var::equal(string _name, int _type, int _varType)
{
	if (getName().compare(_name) == 0 && (getType() == _type) && (getVarType() == _varType))
		return 0;
	return 1;
}
string Var::toString()
{
	string out = "V";
	out.append(to_string(getType()));
	out.append("{");
	out.append(getName());
	out.append("}");
	return out;
};

/*
  End Var
*/
