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
	int qvar; //establish wheter the variable is quantified (value 1) or not (value 0)
	int index; //index of the variable in the corresponding vector
	string name; //name of the varaible
public:
	Var(string _name, int _sort, int _qvar, int _index);
	~Var();	
	int getSort();
	string getName();
	int getVarType();
	int getIndex();
	void setIndex(int _index);
	void setName(string _name);
	void setSort(int _type);
	void setVarType(int _var);
	int equal(Var* match);
	int equal(string _name, int _type, int _varType);
	string toString();
};