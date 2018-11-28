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
	int equal(const Var& match) const;
	int equal(const string _name, const int _type, const int _varType) const;
	string toString();
};


//This class is delegated to mask the datastructure of the variables used by a knowledge base
class VariablesSet
{
private:
	vector< vector <Var> > setConstants; //vector of constants
	vector< vector <Var> > setUniQuantified; //vector of quantified variables
	vector< vector <Var> > setExistQuantified; //vector of existential quantified variables 
	vector<Var>& getAccessToConstantsOfSort(int vectorSort);
public:
	VariablesSet();
	
	VariablesSet(const size_t maxNsorts, const size_t maxNConstants, const size_t maxNUniQuantified, const size_t maxNExistQuantified);
	VariablesSet(const size_t maxNSorts, const vector<size_t>& kbSizeVector);
	size_t getSorting();
	size_t getNumberOfConstantsOfSort(const int vectorSort);
	size_t getNumberOfQuantVariablesOfSort(const int vectorSort);
	size_t getNumberOfExistVariablesOfSort(const int vectorSort);
	int  insertConstant(const Var& variable, const int vectorSort);

/*	vector<Var>& getVectorAt(vector<vector<Var>>& vec, int level);
	vector<Var>* getVVLAt(int level);
	vector<Var>* getVQLAt(int level);
	int pushBack(vector<vector <Var>>& vec, int inslevel, string name, int level, int vartype);
	int VVLPushBack(int inslevel, string name, int level, int vartype);
	vector<vector <Var>>& getVQL();
	vector<vector <Var>>& getVVL();
	Var* getBack(vector<vector <Var>>& vec, int level);
	Var* VVLGetBack(int level);
	//	Var* VQLGetBack(int level) { return &VQL.at(level).back(); }
	//	Var* QVQLGetBack(int level) { return &QVQL.at(level).back(); }
	*/
};