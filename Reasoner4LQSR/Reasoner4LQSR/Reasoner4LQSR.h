#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <array>
#include <stack>
#include <sstream>

using namespace std;



// VariablesSet varSet = VariablesSet(3, 100);
// Operators operators = Operators();

class Var
{
private:
	int sort; //sort of the variable
	int qvar; //establish wheter the variable is quantified (value 1-2) or not (value 0)
	int index; //index of the variable in the corresponding vector
	string name; //name of the varaible
public:
	Var(const string _name, const int _sort, const int _qvar, const int _index);
	~Var();	
	int getSort() const ;
	string getName() const;
	int getVarType() const;
	int getIndex() const;
	void setIndex(const int _index);
	void setName(const string _name);
	void setSort(const int _type);
	void setVarType(const int _var);
	int equal(const Var& match);
	int equal(const string _name, const int _type, const int _varType);
	string toString();
};