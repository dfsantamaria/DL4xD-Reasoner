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
	int type;
	int var;
	int index;
	string name;
public:
	Var(string _name, int _type, int _var, int _index);
	~Var();
	//int isValidType() { return isValidType(type);};
	//int isValidVar() { return isValidVar(var); };
	int getType();
	string getName();
	int getVarType();
	int getIndex();
	void setIndex(int _index);
	int setName(string _name);
	void setType(int _type);
	void setVarType(int _var);
	int equal(Var* match);
	int equal(string _name, int _type, int _varType);
	string toString();
};