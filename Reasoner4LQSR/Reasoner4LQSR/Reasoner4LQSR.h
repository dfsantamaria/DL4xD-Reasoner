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

class Atom
{
private:
	int atomOp;
	/*
	-1 unsetted;
	*/
	vector<Var*> components;
	/*
	Right Operand is the first element of the vector.
	Pair is the second and the third.
	Con il vettore consideriamo anche i possibili datatype groups
	*/
 public:
	Atom();
	Atom(int op, vector<Var*> vec);
	vector<Var*>& getElements();
	int getAtomOp();
	void setAtomOp(int value);
	Var* getElementAt(int index);
	~Atom();
	/*
	Use Carefully. Remember the position of the left/right operand
	*/
	void addElement(Var* element);
	int setElementAt(int index, Var* element);
	string toString();
	int containsQVariable();
	int equals(Atom &atom);
};

//Special chars of a formula
class Operators
{
private:
	vector<string> logOp = { "$OR","$AD","$RO","$DA", "$NG" }; //logic operators
	vector<string> setOp = { "$IN", "$EQ", "$NI", "$QE", "$OA", "$AO", "$CO" }; //set operators
	vector<string> qutOp = { "$FA" };	//quantifiers
public:
	Operators();
	size_t getLogOpSize();
	size_t getSetOpSize();
	size_t getQuantiOpSize();
	int getLogOpValue(string s);
	int getSetOpValue(string s);
	int getQuantiOpValue(string s);
	string getLogOpElement(int index);
	string getSetOpElement(int index);
	string getQuantiOpElement(int index);
	/*
	Return the value of the given string representing a logic operator
	*/
	int checkLogOp(string *s); 	
};


class Formula  //struttura più generica da cambiare eventualmente con strutture per CNF
{
private:
	//vector<Var*> quantified;
	Atom *atom;
	int operand;
	int fulfilled;
	Formula *lsubformula;
	Formula *rsubformula;
	Formula *pformula;
public:
	Formula();
	Formula(Atom *at, int op);
	Formula(Atom *at, int op, Formula *lf, Formula *rf);
	// vector<Var*>* getQuantifiedSet() { return &quantified; };
	Atom* getAtom();
	int getOperand();
	Formula* getLSubformula();
	Formula* getRSubformula();
	Formula* getPreviousformula();
	void setOperand(int op);
	void setAtom(Atom *at);
	void setLSubformula(Formula *sub);
	void setRSubformula(Formula *sub);
	void setPreviousFormula(Formula *prev);
	void setFulfillness(int val);
	int getFulfillness();
	~Formula();
	string toString();
};


class Node
{
private:
	vector<Formula> setFormula;
	Node* leftChild;
	Node* rightChild;
	Node* father;
	//flag di fullfilled
	//flag di completeness
	int containsFormula(Formula &F);
public:
	Node(int size);
	Node();
	Node(vector<Formula>& formula);
	Node* getLeftChild();
	Node* getRightChild();
	Node* getFather();
	void setRightChild(Node* child);
	void setLeftChild(Node* child);
	void setFather(Node* f);
	void insertFormula(Formula& f);	
	void insertCheckFormula(Formula& f);
	vector<Formula>& getSetFormulae();
	void setSetFormulae(vector<Formula>& input);
	~Node();
};

class VariablesSet
{
private:
	vector< vector <Var> > VVL;
	vector< vector <Var> > VQL;
	//vector< vector <Var> > QVQL;
	int capacity;
	int nlevel;
public:
	VariablesSet();
	VariablesSet(int maxNVariable, int maxVSize, int maxQSize);
	vector<Var>*getAt(vector<vector<Var>>& vec, int level);
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
	size_t getSize(vector<vector <Var>>& vec);
	//	size_t VVLGetSize() { return VVL.size(); }
	size_t VQLGetSize();
	//	size_t QVQLGetSize() { return QVQL.size(); }
	size_t getSizeAt(vector<vector <Var>>& vec, int level);
	size_t VVLGetSizeAt(int level);
	size_t VQLGetSizeAt(int level);
	//	size_t QVQLGetSizeAt(int level) { return QVQL.at(level).size(); }
};


class Tableau
{
private: Node* radix;
         vector <Node*> openBranches;
         vector <Node*> closedBranches;
         vector < vector < vector <Var*> > > EqSet;
public:
	Tableau(Node* initial);
	Tableau(int size_radix);
	Node* getTableau();
	int insertFormula(Formula &f);
	vector<Node*>& getOpenBranches();
	vector<Node*>& getClosedBranches();
	vector < vector < vector <Var*> > >& getEqSet();
	void addClosedBranch(Node* node);
	void areInEqClass(Var& var1, int& varclass1, int& indx1, Var& var2, int& varclass2, int& indx2, int brindx);
	int sameEqClass(Var& var1, Var& var2, int brindx);
	void mergeEqClass(int brindx, int indx1, int indx2);
	void mergeEqClass(int brindx, int indx1, Var& var);
	void mergeEqClass(int brindx, Var& var, int indx1);
	~Tableau();
};


class QueryManager
{
private: vector< vector <Var> > QVQL;
		 vector< vector <Var> > QVVL;
		 vector < int > ynAnswer;
		 pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>> Match;
		 Formula formula;
		 int nlevel;
		 int maxQQSize;

public:
	QueryManager(int _nlevel, int _maxQQSize);
	void extractLiterals(Formula& f, vector<Atom*> &atoms);
	vector<vector<Var>>& getQVQL();
	vector<vector<Var>>& getQVVL();
	vector<int>& getAnswerSet();
	pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& getMatchSet();
	void setMatchSet(pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& input);
	void setAnswerSet(vector<int> input);
	int checkQueryLiteralMatchInBranch(Node* branch, Atom* query);
	int checkQueryVariableMatchInBranch(Node* branch, Atom* query, vector<pair<Var*, Var*>>& currentMatch, vector<vector<pair<Var*, Var*>>>& matches);
	Atom applySubstitution(Atom* result, Atom* query, const vector<pair<Var*, Var*>>& matches);
	void executeQuery(Formula& f, Tableau& tableau, pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& result, int YN, vector<int>& ynAnswer);
};