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
	int getSort() const;
	string getName() const;
	int getVarType() const;
	int getIndex() const;
	void setIndex(const int _index);
	void setName(const string _name);
	void setSort(const int _type);
	void setVarType(const int _var);
	int equal(const Var& match) const ;
	int equal(const string _name, const int _type, const int _varType) const;
	string toString() const;
};


//This class is delegated to mask the datastructure of the variables used by a knowledge base
class VariablesSet
{
private:
	vector< vector <Var> > setConstants; //vector of constants
	vector< vector <Var> > setUniQuantified; //vector of quantified variables
	vector< vector <Var> > setExistQuantified; //vector of existential quantified variables 
	vector<Var>& getUnsafeAccessToConstantsOfSort(const int vectorSort);
	vector<Var>& getUnsafeAccessToUniQuantifiedVarsOfSort(const int vectorSort);
	vector<Var>& getUnsafeAccessToExQuantifiedVarsOfSort(const int vectorSort);
public:
	VariablesSet();
	
	VariablesSet(const size_t maxNsorts, const size_t maxNConstants, const size_t maxNUniQuantified, const size_t maxNExistQuantified);
	VariablesSet(const size_t maxNSorts, const vector<size_t>& kbSizeVector);
	size_t getSorting();
	size_t getNumberOfConstantsOfSort(const int vectorSort);
	size_t getNumberOfUniQuantifiedVarsOfSort(const int vectorSort);
	size_t getNumberOfExQuantifiedVarsOfSort(const int vectorSort);
	
	const vector<Var>& getAccessToConstantsOfSort(const int vectorSort) const;
	const vector<Var>& getAccessToUniQuantifiedVarsOfSort(const int vectorSort) const;
	const vector<Var>& getAccessToExQuantifiedVarsOfSort(const int vectorSort) const;

	int  insertConstant(const Var& variable, const int vectorSort);
	int  insertConstant(const string& name, const int vectorSort);	

	int  insertUniQuantifiedVar(const Var& variable, const int vectorSort);
	int  insertUniQuantifiedVar(const string& name, const int vectorSort);

	int  insertExQuantifiedVar(const Var& variable, const int vectorSort);
	int  insertExQuantifiedVar(const string& name, const int vectorSort);

};


//This Class contains  the operators used by 4LQSR formulae
class Operators
{
private:
	vector<string> logConnect = { "$OR","$AD","$RO","$DA", "$NG", "$IF", "$FI", "$II" }; //logic connectors {OR,AND,NOR,NAND,NOT,RIMPLIES,LIMPLIES,DIMPLIES}
	vector<string> setOp = { "$IN", "$EQ", "$NI", "$QE", "$OA", "$AO", "$CO" }; //set operators {IN,EQUIVALENT,NOTIN,NOTEQUIVALENT, LEFTANGLE, RIGHTANGLE, COMMA}
	vector<string> quantifiers = { "$FA", "$EX"};	//quantifiers {FORALL,EXISTS}
public:
	Operators();
	size_t getLogConnectSize();
	size_t getSetOpSize();
	size_t getQuantSize();
	int getLogConnectValue(string connect);
	int getSetOpValue(string setOperator);
	int getQuantValue(string quantifier);
	string getLogConnectElement(int index_log);
	string getSetOpElement(int index_op);
	string getQuantElement(int index_quant);
};