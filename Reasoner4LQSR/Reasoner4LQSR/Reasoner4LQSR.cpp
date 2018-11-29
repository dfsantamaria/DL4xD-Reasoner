
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
/// <param name="_qvar">Existential Quantified (value 2), Universally Quantified (value 1), or constant (value 0) </param>
/// <param name="_index">Position in the vector</param>
Var::Var(const string _name,const int _sort, const int _qvar,const int _index)
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
 int  Var::getSort() const { return sort; };
/// <summary>
/// Reeturn the name of the variable
/// </summary>
string Var::getName() const { return name; };
/// <summary>
/// Return wheter or not the variable is quantified (1 for quantification, 0 for constant)
/// </summary>
int Var::getVarType() const { return qvar; };
/// <summary>
/// Return the index of the variable in the corresponding vector
/// </summary>
int Var::getIndex() const { return index; };

/// <summary>
///  Set the index of the variable
/// </summary>
/// <param name="_index">Index of the variable</param>
void Var::setIndex(const int _index) { index = _index; }

/// <summary>
/// Set the name of the variable
/// </summary>
/// <param name="_name">Name of the variable</param>
void Var::setName(const string _name)
{
	name = _name;
};

/// <summary>
/// Set the sort of the variable
/// </summary>
/// <param name="_sort">Sort of the variable</param>
void Var::setSort(const int _sort)
{
	sort = _sort;
};

/// <summary>
/// Set the quantification of the variable
/// </summary>
/// <param name="_qvar">0 for constant, 1 for U-quantified variable, 2 for E-quantified variable</param>
void Var::setVarType(const int _qvar)
{
	qvar = _qvar;
};

/// <summary>
/// Compare two variable, given an object of type Var. Return 0 if two variables coincides, 1 otherwise
/// </summary>
/// <param name="match"></param>
/// <returns></returns>
int Var::equal (const Var& match) const
{
	if ((getSort() == match.getSort()) && 
		(getVarType() == match.getVarType()) && 
		(getName().compare(match.getName()) == 0)
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
int Var::equal(const string _name, const int _sort, const int _varType) const
{
	if ( (getSort() == _sort)  && (getVarType() == _varType) && (getName().compare(_name)) == 0 )
		return 0;
	return 1;
}


/// <summary>
/// Return a string representing the variable.
/// </summary>
/// <returns></returns>
string Var::toString() const
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

/*
  Start Variable Set
*/
/// <summary>
/// This Class is delegated to masks the datastructures of the variables used by a knowledge base
/// </summary>

VariablesSet::VariablesSet() {};

/// <summary>
/// This Class is delegated to masks the datastructures of the variables used by a knowledge base. This constructor builds maxNsorts+1 vectors of lenght maxNConstants
/// for the constants, maxNsorts+1 vectors of lenght maxNUniQuantified for universal quantified variables, and maxNsorts+1 vectors of lenght maxNExistQuantified for existential quantified
/// </summary>
/// <param name="_maxNsorts"> Maximum sorting </param>
/// <param name="_maxNconstants"> Max number of constants</param>
/// <param name="_qvar">Quantified (value 1) or constant (value 0) </param>
/// <param name="_index">Position in the vector</param>
VariablesSet::VariablesSet(const size_t maxNsorts, const size_t maxNConstants, const size_t maxNUniQuantified, const size_t maxNExistQuantified)
{	
	setConstants.reserve(maxNsorts);  //initialize vectors of constants 
	setUniQuantified.reserve(maxNsorts); //initialize vectors of Univ. quantified variables
	setExistQuantified.reserve(maxNsorts); //initialize vectors of Exist. quantified variables	
	
	for (int i = 0; i <= maxNsorts; i++) // for each vector we allocate space for all the sorts
	{
		setConstants.push_back(vector<Var>());
		setUniQuantified.push_back(vector<Var>());
		setExistQuantified.push_back(vector<Var>());

		setConstants.at(i).reserve(maxNConstants);
		setUniQuantified.at(i).reserve(maxNUniQuantified);
		setExistQuantified.at(i).reserve(maxNExistQuantified);
		//	QVQL.at(i).reserve(maxQQSize);
	}
};
/// <summary>
/// This Class is delegated to masks the datastructures of the variables used by a knowledge base. This constructor builds maxNsorts vectors whose sizes
/// are provided by the vector kbSizeVector.
/// </summary>
/// <param name="_maxNsorts"> Numerber of sorts </param>
/// <param name="_kbSizeVector"> vector of sizes of each vector</param>

VariablesSet::VariablesSet(const size_t maxNsorts, const vector<size_t>& kbSizeVector)
{
  setConstants.reserve(maxNsorts);  //initialize vectors of constants 
  setUniQuantified.reserve(maxNsorts); //initialize vectors of Univ. quantified variables
  setExistQuantified.reserve(maxNsorts); //initialize vectors of Exist. quantified variables	

  for (int i = 0; i <= maxNsorts; i++) // for each vector we allocate space for all the sorts
  {
	  setConstants.push_back(vector<Var>());
	  setUniQuantified.push_back(vector<Var>());
	  setExistQuantified.push_back(vector<Var>());

	  setConstants.at(i).reserve(kbSizeVector.at(i));
	  setUniQuantified.at(i).reserve(kbSizeVector.at(i+maxNsorts+1));
	  setExistQuantified.at(i).reserve(i + 2*maxNsorts + 2);	 
  }

};
/// <summary>
/// This function returns the maximum sorting used
/// </summary>
size_t VariablesSet::getSorting() { return setConstants.size()-1; };

/// <summary>
/// This function returns the number of constants of the sort specified by the parameter 
/// </summary>
/// <param name="vectorSort"> The sorting of the vector</param>
size_t VariablesSet::getNumberOfConstantsOfSort(const int vectorSort)
{
	if (vectorSort <= setConstants.size())
		return setConstants.at(vectorSort).size(); 
	return 0;
};

/// <summary>
/// This function returns the number of universally quantified variables of the sort specified by the parameter 
/// </summary>
/// <param name="vectorSort"> The sorting of the vector</param>
size_t VariablesSet::getNumberOfQuantVariablesOfSort(int vectorSort)
{
	if (vectorSort <= setUniQuantified.size())
		return setUniQuantified.at(vectorSort).size();
	return 0;
};

/// <summary>
/// This function returns the number of existentially quantified variables of the sort specified by the parameter 
/// </summary>
/// <param name="vectorSort"> The sorting of the vector</param>
size_t VariablesSet::getNumberOfExistVariablesOfSort(int vectorSort)
{
	if (vectorSort <= setExistQuantified.size())
		return setExistQuantified.at(vectorSort).size();
	return 0;
};

/// <summary>
/// This private function get a reference of the vector of constants of the given sort.  
/// </summary>
/// <param name="vectorSort"> The sort of the vector to get access</param>
vector<Var>& VariablesSet::getUnsafeAccessToConstantsOfSort(const int vectorSort)
{
	return setConstants.at(vectorSort);
};

/// <summary>
/// This function inserts a constant  in the vector of the given sort. This insertion is unsafe. It can cause the reallocation of the vector.  
/// </summary>
/// <param name="variable"> The constant to be inserted</param>
/// <param name="vectorSort"> The sort of the vector where constant is inserted</param>
int  VariablesSet::insertConstant(const Var& variable, const int vectorSort)
{
	if (variable.getVarType() != 0 || variable.getSort() != vectorSort)
		return -1;
	getUnsafeAccessToConstantsOfSort(vectorSort).push_back(variable);
	int pos = getUnsafeAccessToConstantsOfSort(vectorSort).size() - 1;
	getUnsafeAccessToConstantsOfSort(vectorSort).back().setIndex(pos);
	return pos;	
};

/// <summary>
/// This function inserts a constant  in the vector of the given sort. This insertion is unsafe. It can cause the reallocation of the vector.  
/// </summary>
/// <param name="variable_name"> The name of the constant to be inserted</param>
/// <param name="vectorSort"> The sort of the vector where constant is inserted</param>
int  VariablesSet::insertConstant(const string& name, const int vectorSort)
{	
	getUnsafeAccessToConstantsOfSort(vectorSort).push_back(Var(name, vectorSort, 0, 0));
	int pos = getUnsafeAccessToConstantsOfSort(vectorSort).size() - 1;
	getUnsafeAccessToConstantsOfSort(vectorSort).back().setIndex(pos);
	return pos;
};

/// <summary>
/// This function get a constant reference of  the vector of constants of the given sort.  
/// </summary>
/// <param name="vectorSort"> The sort of the vector to get access</param>
const vector<Var>& VariablesSet::getAccessToConstantsOfSort (const int vectorSort) const
{
	return setConstants.at(vectorSort);
};