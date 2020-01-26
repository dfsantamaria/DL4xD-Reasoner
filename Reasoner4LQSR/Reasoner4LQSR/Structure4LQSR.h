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
	size_t sort; //sort of the variable
	size_t qvar; //establish wheter the variable is quantified (value 1-2) or not (value 0)
	int index; //index of the variable in the corresponding vector
	int stuffing; //stuffing data
	string name; //name of the varaible		
public:
	Var(const string _name, const size_t _sort, const size_t _qvar, const int _index);
	~Var();	
	size_t getSort() const;
	string getName() const;
	size_t getVarType() const;
	int getIndex() const;
	void setIndex(const int _index);
	void setName(const string _name);
	void setSort(const size_t _type);
	void setVarType(const size_t _var);
	int equal(const Var* match) const ;
	int equal(const string _name, const size_t _type, const size_t _varType) const;
	string toString() const;
};


//This class is delegated to mask the datastructure of the variables used by a knowledge base
class VariablesSet
{
private:
	vector< vector <Var> > setConstants; //vector of constants
	vector< vector <Var> > setUniQuantified; //vector of quantified variables
	vector< vector <Var> > setExistQuantified; //vector of existential quantified variables 
	vector<Var>& getUnsafeAccessToConstantsOfSort(const size_t vectorSort);
	vector<Var>& getUnsafeAccessToUniQuantifiedVarsOfSort(const size_t vectorSort);
	vector<Var>& getUnsafeAccessToExQuantifiedVarsOfSort(const size_t vectorSort);
public:
	VariablesSet();
	
	VariablesSet(const size_t maxNsorts, const size_t maxNConstants, const size_t maxNUniQuantified, const size_t maxNExistQuantified);
	VariablesSet(const size_t maxNSorts, const vector<size_t>& kbSizeVector);
	size_t getSorting();
	size_t getNumberOfConstantsOfSort(const size_t vectorSort);
	size_t getNumberOfUniQuantifiedVarsOfSort(const size_t vectorSort);
	size_t getNumberOfExQuantifiedVarsOfSort(const size_t vectorSort);
	
	const vector<Var>& getAccessToConstantsOfSort(const size_t vectorSort) const;
	const vector<Var>& getAccessToUniQuantifiedVarsOfSort(const size_t vectorSort) const;
	const vector<Var>& getAccessToExQuantifiedVarsOfSort(const size_t vectorSort) const;

	int  insertConstant(const Var& variable, const size_t vectorSort);
	int  insertConstant(const string& name, const size_t vectorSort);

	int  insertUniQuantifiedVar(const Var& variable, const size_t vectorSort);
	int  insertUniQuantifiedVar(const string& name, const size_t vectorSort);

	int  insertExQuantifiedVar(const Var& variable, const size_t vectorSort);
	int  insertExQuantifiedVar(const string& name, const size_t vectorSort);

};

//This class is used to represented either a Literal or an Operator in a Formula. A formula is just a vector of 
//FormulaObject that are represent in the Polish Notation.
class FormulaObject
{
 public:
   virtual ~FormulaObject()=0;	
   virtual int isALiteral()=0;   
};


//This Class contains  the operators used by 4LQSR formulae
class Operators : public FormulaObject
{
private:
	vector<string> logConnect = { "$OR","$AD","$RO","$DA", "$NG", "$IF", "$FI", "$II" }; //logic connectors {OR,AND,NOR,NAND,NOT,RIMPLIES,LIMPLIES,DIMPLIES}
	vector<string> setOp = { "$IN", "$EQ", "$NI", "$QE", "$OA", "$AO", "$CO" }; //set operators {IN,EQUIVALENT,NOTIN,NOTEQUIVALENT, LEFTANGLE, RIGHTANGLE, COMMA}
	vector<string> quantifiers = { "$FA", "$EX"};	//quantifiers {FORALL,EXISTS}
public:
	Operators();
	size_t getLogConnectSize() const;
	size_t getSetOpSize() const;
	size_t getQuantSize() const;
	int getLogConnectValue(string connect) const;
	int getSetOpValue(string setOperator) const;
	int getQuantValue(string quantifier) const;
	string getLogConnectElement(size_t index_log)const;
	string getSetOpElement(size_t index_op) const;
	string getQuantElement(size_t index_quant) const;
	int isALiteral();
};

//This class manages a 4LQSR literal
class Literal : public FormulaObject
{
private:
	int literalOp;//-1 if unsetted, otherwise a value corresponding to an index of the setOp vector	
	int stuffing; //stuffing data
	vector<Var*> components;
	/*
	Right Operand is the first element of the vector.
	Pair is the second and the third.	
	*/
public:
	Literal();
	Literal(const int literalOp_, const vector<Var*>& components_);
	Literal(const int literalOp_, const size_t componentsSize);
	~Literal();
	const vector<Var*>& getElements() const;
	int getLiteralOp() const;
	void setLiteralOp(const int literalOp_);
	const Var* getElementAt(const size_t index) const;		
	void addElement(Var* element);
	void setElementAt(const size_t index, Var* element);	
	int equals(const Literal &literal) const;
	int isALiteral();
};
