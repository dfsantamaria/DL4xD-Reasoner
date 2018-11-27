
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
/// <param name="_sort"> Sort of the variable</param>
/// <param name="_qvar">Quantified (value 1) or constant (value 0) </param>
/// <param name="_index">Position in the vector</param>
Var::Var(string _name, int _sort, int _qvar, int _index)
{
	setName(_name);
	setSort(_sort);
	setVarType(_qvar);
	index = _index;
};
/// <summary>
/// The destructor
/// </summary>
/// 
Var::~Var(){};
/// <summary>
/// Return the sort of the variable
/// </summary>
/// 
int Var::getSort() { return sort; };
/// <summary>
/// Reeturn the name of the variable
/// </summary>
string Var::getName() { return name; };
/// <summary>
/// Return wheter or not the variable is quantified (1 for quantification, 0 for constant)
/// </summary>
int Var::getVarType() { return qvar; };
/// <summary>
/// Return the index of the variable in the corresponding vector
/// </summary>
int Var::getIndex() { return index; };

/// <summary>
///  Set the index of the variable
/// </summary>
/// <param name="_index">Index of the variable</param>
void Var::setIndex(int _index) { index = _index; }

/// <summary>
/// Set the name of the variable
/// </summary>
/// <param name="_name">Name of the variable</param>
void Var::setName(string _name)
{
	name = _name;
};

/// <summary>
/// Set the sort of the variable
/// </summary>
/// <param name="_sort">Sort of the variable</param>
void Var::setSort(int _sort)
{
	sort = _sort;
};

/// <summary>
/// Set the quantification of the variable
/// </summary>
/// <param name="_qvar">0 for constant, 1 for U-quantified variable, 2 for E-quantified variable</param>
void Var::setVarType(int _qvar)
{
	qvar = _qvar;
};

/// <summary>
/// Compare two variable, given an object of type Var. Return 0 if two variables coincides, 1 otherwise
/// </summary>
/// <param name="match"></param>
/// <returns></returns>
int Var::equal(Var* match)
{
	if ((getSort() == match->getSort()) && 
		(getVarType() == match->getVarType()) && 
		(getName().compare(match->getName()) == 0)
		)
	  return 0;
	return 1;
};

/// <summary>
/// Compare two variable, given a variable name, sort of the variable, and quantification of variable. Return 0 if two variables coincide, 1 otherwise
/// </summary>
/// <param name="_name"></param>
/// <param name="_sort"></param>
/// <param name="_varType"></param>
/// <returns></returns>
int Var::equal(string _name, int _sort, int _varType)
{
	if ( (getSort() == _sort)  && (getVarType() == _varType) && (getName().compare(_name)) == 0 )
		return 0;
	return 1;
}


/// <summary>
/// Return a string representing the variable.
/// </summary>
/// <returns></returns>
string Var::toString()
{
	string out = "V";
	out.append(to_string(getSort()));
	out.append("{");
	out.append(getName());
	out.append("}");
	return out;
};

/*
  End Var
*/
