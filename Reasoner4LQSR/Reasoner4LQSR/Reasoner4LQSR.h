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

class Literal
{
private:
	int literalOp;
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
	Literal();
	Literal(int op, vector<Var*> vec);
	vector<Var*>& getElements();
	int getLiteralOp();
	void setLiteralOp(int value);
	Var* getElementAt(int index);
	~Literal();
	/*
	Use Carefully. Remember the position of the left/right operand
	*/
	void addElement(Var* element);
	int setElementAt(int index, Var* element);
	string toString();
	int containsQVariable();
	int containsEVariable();
	int equals(Literal &atom);
};

//Special chars of a formula
class Operators
{
private:
	vector<string> logOp = { "$OR","$AD","$RO","$DA", "$NG", "$IF", "$FI", "$II" }; //logic operators
	vector<string> setOp = { "$IN", "$EQ", "$NI", "$QE", "$OA", "$AO", "$CO" }; //set operators
	vector<string> qutOp = { "$FA", "$EX" };	//quantifiers
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
	Return the value of the given string representing a logic connector
	*/
	int checkLogOp(string *s); 	
};


class Formula  //struttura più generica da cambiare eventualmente con strutture per CNF
{
private:
	//vector<Var*> quantified;
	Literal *atom;
	int operand;
	int fulfilled;
	Formula *lsubformula;
	Formula *rsubformula;
	Formula *pformula;
public:
	Formula();
	Formula(Literal *at, int op);
	Formula(Literal *at, int op, Formula *lf, Formula *rf);
	// vector<Var*>* getQuantifiedSet() { return &quantified; };
	Literal* getLiteral();
	int getOperand();
	Formula* getLSubformula();
	Formula* getRSubformula();
	Formula* getPreviousformula();
	void setOperand(int op);
	void setLiteral(Literal *at);
	void setLSubformula(Formula *sub);
	void setRSubformula(Formula *sub);
	void setPreviousFormula(Formula *prev);
	void setFulfillness(int val);
	int getFulfillness();
	~Formula();
	string toString();
	string toStringNoQVar();
	void toStringQVar(vector<Var*>& qu);
};


class Node
{
private:
	vector<Formula*> setFormula;
	Node* leftChild;
	Node* rightChild;
	Node* father;
	//flag di fullfilled
	//flag di completeness
	int containsFormula(Formula* F);
public:
	Node(int size);
	Node();
	Node(vector<Formula*> formula);
	Node* getLeftChild();
	Node* getRightChild();
	Node* getFather();
	void setRightChild(Node* child);
	void setLeftChild(Node* child);
	void setFather(Node* f);
	void insertFormula(Formula* f);	
	void insertCheckFormula(Formula* f);
	vector<Formula*> getSetFormulae();
	void setSetFormulae(vector<Formula*> input);
	~Node();
};

class VariablesSet
{
private:
	vector< vector <Var> > VVL;
	vector< vector <Var> > VQL;
	vector< vector <Var> > VEL;
	//vector< vector <Var> > QVQL;
	//int capacity;
	//int nlevel;
public:
	VariablesSet();
	VariablesSet(int maxNVariable, int maxVSize, int maxQSize, int maxESize);
	VariablesSet(int maxNVariable, vector<int>& KBsize);
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
	int insertFormula(Formula* f);
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
	void extractLiterals(Formula& f, vector<Literal*> &atoms);
	vector<vector<Var>>& getQVQL();
	vector<vector<Var>>& getQVVL();
	vector<int>& getAnswerSet();
	pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& getMatchSet();
	void setMatchSet(pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& input);
	void setAnswerSet(vector<int> input);
	int checkQueryLiteralMatchInBranch(Node* branch, Literal* query);
	int checkQueryVariableMatchInBranch(Node* branch, Literal* query, vector<pair<Var*, Var*>>& currentMatch, vector<vector<pair<Var*, Var*>>>& matches);
	Literal applySubstitution(Literal* result, Literal* query, const vector<pair<Var*, Var*>>& matches);
	int executeQuery(Formula& f, Tableau& tableau, pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& result, int YN, vector<int>& ynAnswer);
	//int QueryManager::executeQueryEq(Formula& f, Tableau& tableau, pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& result, int YN, vector<int>& ynAnswer);
	//void QueryManager::computeQueryPartialSolution(Tableau& tableau, int branchIt, Literal& query, vector<pair<Var*, Var*>>& partSol, vector<vector<pair<Var*, Var*>>>& matchSet);
};



void areInEqClass(Var& var1, int& varclass1, int& indx1, Var& var2, int& varclass2, int& indx2, Tableau& tab, int brindx);
void buildEqSet(Tableau& tab);
int checkLiteralClash(Literal &atom); //0 for clash 1 for no-clash
int checkLiteralClash(Literal &atom1, Literal &atom2, int checkQvar);
int checkLiteralClashEqSet(Literal &atom, Tableau& T, int& brindx); //0 for clash 1 for no-clash
int checkLiteralClashEqSet(Literal &atom1, Literal &atom2, Tableau& t, int& brindx);
int checkLiteralOpClash(int op1, int op2);
int checkLiterals(Literal* atom, Node* node, int checkQvar);
int checkLiteralTautology(Literal &atom);
int checkLiteralsClash(vector<Literal*> &vec, int checkQvar);
int checkBranchClash(Literal* atom, Node* node, int checkQvar);
int checkBranchClash(Node* node, Tableau& tableau, int checkQvar);
int checkNodeClash(vector<Formula> &formset, int checkQvar);
int checkNodeClashEqSet(Literal& atom, Node& node, Tableau& T, int& brind);
void checkTableauClash(Tableau& T);
//int checkTableauRootClash(Tableau &T);
int checkVectorClash(Literal* candidate, vector<Formula> &formset, int start, int checkQvar);
//void chooseRule(Tableau &T, vector<Node*> &nodeSet, Formula* f);
void closeTableauRoot(Tableau& T);
//int containsQVar(Formula *fr, string &s);
int containsQVar(Formula *fr);
int containsVariableName(vector<Var>* vect, Var** found, const string *name, const int *start);
Formula* convertFormulaToCNF(Formula* formula);
void convertKBToCNF(vector<Formula*>& KB, vector<Formula*>& KBcnf);
Literal* copyLiteral(Literal* atom, const string *qvar, Var* dest);
void copyLiteral(Literal* source, Literal* dest);
Formula* copyFormula(Formula* formula, Formula* father);
Formula* copyFormula(Formula* formula, Formula* father, const string *qvar, Var* dest);
int createLiteral(vector<vector <Var>>& vec, vector<vector <Var>>& vec2, string input, Formula **formula, vector<int>& startQuantVect, int typeformula);
Var* createQVarFromString(vector<vector<Var>>& vec, string *name, int *level, int *vartype, int *start);
Var* createVarFromString(vector<vector<Var>>& vec, vector<vector<Var>>& vec2, string *name, int *level, int *vartype, int *start, int *typeformula);
void EGRule(Literal* atom, Node* node);
//int expandKB(const vector<Formula*> &inpf, vector <Formula*> &out);
//void expandTableau(Tableau& T);
int computeRuleLiteral(Literal* litstl, Literal* lit);
int computeRule(Node* leaf, vector<Literal*>& stl, vector<Literal*>& nodeLitStack);
int expandGammaTableau(Tableau& T);
void retrieveQVarSet(vector<Literal*>&atomset, vector<Var*>&varset);
//void retrieveQVarSet(vector<Literal*>&atomset, vector<vector<Var*>>&varset);
template <class Iter>
bool next_variation(Iter first, Iter last, const typename std::iterator_traits<Iter>::value_type max);
int instantiateLiteral(Literal& lit, Literal* dest, vector<int>& ind, vector<Var*>& varset);
template <typename C>
string toStringVector(const vector <C> &data);
int getLiteralSet(Formula* f, vector<Literal*> &outf, int checkQvar);
int getVarsOrder(Var &var1, Var &var2);
int insertFormulaKB(int keepQ, vector<vector <Var>>& varset, vector<vector <Var>>& varset2, string formula, vector<Formula> &vec, int* typeformula);
int insertFormulaKB(int keepQ, vector<vector <Var>>& varset1, vector<vector <Var>>& varset2, string formula, Formula** ffinal, int* typeformula);
void insertVarsEqClass(Var& var1, Var& var2, Tableau& tab, int brindx);
//int instantiateFormula(Formula* f, vector<Formula*> &destination);
void moveQuantifier(int qFlag, Formula* formula, vector<vector <Var>>& varset1, vector<Formula>& data);
void moveQuantifierKB(int qflag, vector<Formula*>& KB, vector<Formula*>& KBout);
Literal* negatedLiteral(Literal *input);
int parseInternalFormula(vector<vector <Var>>& vec, vector<vector <Var>>& vec2, const string *inputformula, Formula **outformula, vector<int>& startQuantVect, int typeformula);
void PBRule(vector<Literal*>& atoms, Node* node, vector<Node*> &nodeSet);
int performQuery(QueryManager*& queryManager, string& str, Formula** formula, Tableau& tableau, int yn);
void performQuerySet(vector<QueryManager*>& results, vector<string>& strings, vector<Formula>& formulae, Tableau& tableau);
void readKBFromFile(int qflag, string &name, vector<Formula*>& KB);
void readKBFromStrings(int qflag, vector<string>&names, vector<Formula*>& KB);
void readQueryFromFile(string& name, vector<string>& stringSet);
void renameQVariables(Formula* formula, vector<vector <Var>>& varset1);
int retrieveVarData(const string input, string* name, int* level);
void debugStart();
void debugEnd();
void InitializeReasoner(int sizeV, int sizeQ, int sizet);
void InitializeReasoner(int sizeV, vector<int>& KBsize);
Formula* convertFormulaToCNF(Formula* formula);
Formula* convertFormulaToNNF(Formula* formula);
Formula* convertFormulaToDNF(Formula* formula);
void convertKBToNNF(vector<Formula*>& KB, vector<Formula*>& KBnorm);
void propagateNegation(Formula* current, vector<bool>& isNegated, vector<Formula*>& stackF, bool valLeft, bool valRight);
void dropLRImplication(Formula* f);
Formula* dropNegation(Formula *f, Formula **topform);
void precomputeKBSpaceFromFile(string &name, vector<int>& KBsize, vector<int>& Litsize);
int precomputeKBSpaceFromString(string& line, vector<int>& KBsize, vector<int>& Litsize);
void computeClassHierarchy(vector<vector<int>>& hierarchy, vector<Formula*>& KB);
void printClassHierarchy(vector<vector<int>>& hierarchy);
//Printing Function;
void printClosedBranches(Tableau& tableau);
void printEqSet(Tableau& tableau);
void printOpenBranches(Tableau& tableau);
void printStack(stack<Formula*> st);
void printStack(stack<string> st);
void printTExpanded(Tableau& tableau);
void printTRadix(vector<Formula*>& KB);
void printVarSet();
void printVector(vector<Var>& v);


