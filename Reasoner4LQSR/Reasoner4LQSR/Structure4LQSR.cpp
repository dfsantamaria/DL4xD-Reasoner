
#include "stdafx.h" // only on windows
#include <fstream>
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <array>
#include <stack>
#include <sstream>
#include "Structure4LQSR.h"

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
size_t VariablesSet::getNumberOfUniQuantifiedVarsOfSort(int vectorSort)
{
	if (vectorSort <= setUniQuantified.size())
		return setUniQuantified.at(vectorSort).size();
	return 0;
};

/// <summary>
/// This function returns the number of existentially quantified variables of the sort specified by the parameter 
/// </summary>
/// <param name="vectorSort"> The sorting of the vector</param>
size_t VariablesSet::getNumberOfExQuantifiedVarsOfSort(int vectorSort)
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
/// This private function get a reference of the vector of universally quantified variables of the given sort.  
/// </summary>
/// <param name="vectorSort"> The sort of the vector to get access</param>
vector<Var>& VariablesSet::getUnsafeAccessToUniQuantifiedVarsOfSort(const int vectorSort)
{
	return setUniQuantified.at(vectorSort);
};

/// <summary>
/// This private function get a reference of the vector of existentially quantified variables of the given sort.  
/// </summary>
/// <param name="vectorSort"> The sort of the vector to get access</param>
vector<Var>& VariablesSet::getUnsafeAccessToExQuantifiedVarsOfSort(const int vectorSort)
{
	return setExistQuantified.at(vectorSort);
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
/// This function inserts a universally quantified variable  in the vector of the given sort. This insertion is unsafe. It can cause the reallocation of the vector.  
/// </summary>
/// <param name="variable"> The universally quantified variable to be inserted</param>
/// <param name="vectorSort"> The sort of the vector where variable is inserted</param>
int  VariablesSet::insertUniQuantifiedVar(const Var& variable, const int vectorSort)
{
	if (variable.getVarType() != 1 || variable.getSort() != vectorSort)
		return -1;
	getUnsafeAccessToUniQuantifiedVarsOfSort(vectorSort).push_back(variable);
	int pos = getUnsafeAccessToUniQuantifiedVarsOfSort(vectorSort).size() - 1;
	getUnsafeAccessToUniQuantifiedVarsOfSort(vectorSort).back().setIndex(pos);
	return pos;
};



/// <summary>
/// This function inserts a existentially quantified variable  in the vector of the given sort. This insertion is unsafe. It can cause the reallocation of the vector.  
/// </summary>
/// <param name="variable"> The existentially quantified variable to be inserted</param>
/// <param name="vectorSort"> The sort of the vector where variable is inserted</param>
int  VariablesSet::insertExQuantifiedVar(const Var& variable, const int vectorSort)
{
	if (variable.getVarType() != 2 || variable.getSort() != vectorSort)
		return -1;
	getUnsafeAccessToExQuantifiedVarsOfSort(vectorSort).push_back(variable);
	int pos = getUnsafeAccessToExQuantifiedVarsOfSort(vectorSort).size() - 1;
	getUnsafeAccessToExQuantifiedVarsOfSort(vectorSort).back().setIndex(pos);
	return pos;
};


/// <summary>
/// This function inserts a constant  in the vector of the given sort. This insertion is unsafe. It can cause the reallocation of the vector.  
/// </summary>
/// <param name="variable_name"> The name of the constant to be inserted</param>
/// <param name="vectorSort"> The sort of the vector where the constant is inserted</param>
int  VariablesSet::insertConstant(const string& name, const int vectorSort)
{	
	getUnsafeAccessToConstantsOfSort(vectorSort).push_back(Var(name, vectorSort, 0, 0));
	int pos = getUnsafeAccessToConstantsOfSort(vectorSort).size() - 1;
	getUnsafeAccessToConstantsOfSort(vectorSort).back().setIndex(pos);
	return pos;
};


/// <summary>
/// This function inserts a uni. quantified variable  in the vector of the given sort. This insertion is unsafe. It can cause the reallocation of the vector.  
/// </summary>
/// <param name="variable_name"> The name of the universally quantified variable to be inserted</param>
/// <param name="vectorSort"> The sort of the vector where the variable is inserted</param>
int  VariablesSet::insertUniQuantifiedVar(const string& name, const int vectorSort)
{
	getUnsafeAccessToUniQuantifiedVarsOfSort(vectorSort).push_back(Var(name, vectorSort, 0, 0));
	int pos = getUnsafeAccessToUniQuantifiedVarsOfSort(vectorSort).size() - 1;
	getUnsafeAccessToUniQuantifiedVarsOfSort(vectorSort).back().setIndex(pos);
	return pos;
};


/// <summary>
/// This function inserts a exist. quantified variable  in the vector of the given sort. This insertion is unsafe. It can cause the reallocation of the vector.  
/// </summary>
/// <param name="variable_name"> The name of the existantially quantified variable to be inserted</param>
/// <param name="vectorSort"> The sort of the vector where the variable is inserted</param>
int  VariablesSet::insertExQuantifiedVar(const string& name, const int vectorSort)
{
	getUnsafeAccessToExQuantifiedVarsOfSort(vectorSort).push_back(Var(name, vectorSort, 0, 0));
	int pos = getUnsafeAccessToExQuantifiedVarsOfSort(vectorSort).size() - 1;
	getUnsafeAccessToExQuantifiedVarsOfSort(vectorSort).back().setIndex(pos);
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


/// <summary>
/// This function get a constant reference of the vector of universally quantified variables of the given sort.  
/// </summary>
/// <param name="vectorSort"> The sort of the vector to get access</param>
const vector<Var>& VariablesSet::getAccessToUniQuantifiedVarsOfSort(const int vectorSort) const
{
	return setUniQuantified.at(vectorSort);
};


/// <summary>
/// This function get a constant reference of the vector of exist. quantified variables of the given sort.  
/// </summary>
/// <param name="vectorSort"> The sort of the vector to get access</param>
const vector<Var>& VariablesSet::getAccessToExQuantifiedVarsOfSort(const int vectorSort) const
{
	return setExistQuantified.at(vectorSort);
};


/// <summary>
/// This function constructs the objects containg all the operators and connectives used by a 4LQSR formula.   
/// </summary>
Operators::Operators() {};

/// <summary>
/// This function returns the size of the logic connectives vector.  
/// </summary>

size_t Operators::getLogConnectSize()  const
{
	return logConnect.size();
};


/// <summary>
/// This function returns the size of the logic connectives vector.  
/// </summary>
size_t Operators::getSetOpSize() const
{
	return setOp.size();
};


/// <summary>
/// This function returns the size of the quantifies vector.  
/// </summary>
size_t Operators::getQuantSize() const
{
	return quantifiers.size();
};

/// <summary>
/// This function returns the integer representing the given logic connective. Such number corresponds to the position of the connective in the vector.  Returns -1 if the connective does not exist.
/// </summary>
/// <param name="connective"> The logic connective</param>
int Operators::getLogConnectValue(string connective) const
{
	for (int i = 0; i < logConnect.size(); i++)
		if (logConnect.at(i).compare(connective) == 0)
			return i;
	return -1;
};

/// <summary>
/// This function returns the integer representing the given set operator. Such number corresponds to the position of the operator in the vector.  Returns -1 if the operator does not exist.
/// </summary>
/// <param name="setOperator"> The set operator</param>
int Operators::getSetOpValue(string setOperator) const
{
	for (int i = 0; i < setOp.size(); i++)
		if (setOp.at(i).compare(setOperator) == 0)
			return i;
	return -1;
};

/// <summary>
/// This function returns the integer representing the given quantifier. Such number corresponds to the position of the quantifier in the vector.  Returns -1 if the quantifier does not exist.
/// </summary>
/// <param name="quantifier"> The quantifier</param>
int Operators::getQuantValue(string quantifier) const
{
	for (int i = 0; i < quantifiers.size(); i++)
		if (quantifiers.at(i).compare(quantifier) == 0)
			return i;
	return -1;
};

/// <summary>
/// This function returns the logic connective represented by the given integer. Such number corresponds to the position of the connective in the vector.  Returns null if the connective does not exist.
/// </summary>
/// <param name="index_log"> The integer representing the logic connective</param>
string Operators::getLogConnectElement(int index_log) const
{
	if (index_log < logConnect.size())
		return logConnect.at(index_log);
	return "";
};

/// <summary>
/// This function returns the set operator represented by the given integer. Such number corresponds to the position of the operator in the vector.  Returns null if the operator does not exist.
/// </summary>
/// <param name="index_op"> The integer representing the operator</param>
string Operators::getSetOpElement(int index_op) const
{
	if (index_op < setOp.size())
		return setOp.at(index_op);
	return "";
};

/// <summary>
/// This function returns the quantifier represented by the given integer. Such number corresponds to the position of the quantifier in the vector.  Returns null if the quantifier does not exist.
/// </summary>
/// <param name="index_quant"> The integer representing the quantifier</param>
string Operators::getQuantElement(int index_quant) const
{
	if (index_quant < quantifiers.size())
		return quantifiers.at(index_quant);
	return "";
};

/// <summary>
/// This class represents a 4LQSR literal
/// </summary>
Literal::Literal() 
{
	literalOp = -1;
	components = vector<Var*>(0);
};


/// <summary>
/// This class represents a 4LQSR literal
/// </summary>
/// <param name="literalOp_"> The value of the set operator</param>
/// <param name="components_"> The vector of vars</param>
Literal::Literal(const int literalOp_, const vector<Var*>& components_)
{
	literalOp = literalOp_;
	components = components_;
};

/// <summary>
/// This class represents a 4LQSR literal
/// </summary>
/// <param name="literalOp_"> The value of the set operator</param>
/// <param name="componentsSize"> The size of vector of vars</param>
Literal::Literal(const int literalOp_, const int componentsSize)
{
	literalOp = literalOp_;
	components = vector<Var*>(componentsSize);
};

Literal::~Literal() { literalOp = -1; components.clear(); };

/// <summary>
/// This function returns the vector of vars
/// </summary>
const vector<Var*>& Literal::getElements() const
{
	return components;
};

/// <summary>
/// This function returns the set operator
/// </summary>
int Literal::getLiteralOp() const
{
	return literalOp;
};

/// <summary>
/// This function changes the set operator
/// </summary>
/// <param name="literalOp_"> The value of the set operator</param>
void Literal::setLiteralOp(const int literalOp_)
{
	literalOp = literalOp_;
};

/// <summary>
/// This function returns the variable at the given position. Returns null if the index is greater than the size of the vector.
/// </summary>
/// <param name="index"> The index of the var to be returned</param>
const Var* Literal::getElementAt(const int index) const
{
	if(index<components.size())
	 return components.at(index);
	return NULL;
};

/// <summary>
/// This function inserts the given variable in the vector of variables.
/// </summary>
/// <param name="element"> The element to insert</param>
void Literal::addElement(Var* element)
{
	components.push_back(element);
};

/// <summary>
/// This function replaces the element at the given index in the vector of variables.
/// </summary>
/// <param name="index"> The index of the variable to replace</param>
/// <param name="element"> The element to insert</param>
void Literal::setElementAt(const int index, Var* element)
{
	if (index < components.size())
		components.at(index) = element;
};


/// <summary>
/// This function compares the current literal with the given one. Returns 0 if they are equals, 1 otherwise.
/// </summary>
/// <param name="literal"> The literal to compare with</param>
int Literal::equals(const Literal &literal) const
{
  if ((this->getElements().size() == literal.getElements().size()) &&
	  (this->getLiteralOp() == literal.getLiteralOp()) ) //same size and same set operator allows for a deeper check
	{
		int j = 0;
		for (;j < this->getElements().size(); j++) //lets check each single var		
		 if ( this->getElementAt(j) -> equal( *(literal.getElementAt(j))) != 0)
			break;
		
		 if (j == this->getElements().size())		
			return 0;		
	}
	return 1;
};