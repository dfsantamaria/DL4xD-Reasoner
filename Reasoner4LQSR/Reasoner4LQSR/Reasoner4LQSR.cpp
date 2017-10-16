// Reasoner4LQSR.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h" // only on windows
#include <fstream>
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <array>
#include <stack>
#include <sstream>

using namespace std;
#define debug                   //debug istructions
#define debugclash              //debug istructions for clash checking
#define debugexpand             //debug istructions for expansion rule
#define debuginsertf            //debug istructions for internal formula translation
#define eqsetdebug                 //debug istructions for computing EqSet
#define debugquery               //debug istructions for query
std::ofstream logFile("LOG.log");   //log file

									/*
									L0 has type 0
									L1 has type 1
									L2 has type 2
									L3 as ObjectProperty has type 3
									L3 as DataProperty has type 4
									*/
									//const int minVarSize = -1;
									//const int maxVarSize = 3;   //size of variable

									//Class of the single Var
class Var
{
private:
	int type;
	int var;
	int index;
	string name;  // Underscore is allowed to define name with underscore			
				  //int isValidType(int _type) { return _type > minVarSize && _type <= maxVarSize; };
				  //int isValidVar(int _var) { return _var >= 0 && _var <= 1; };

public:
	Var(string _name, int _type, int _var, int _index)
	{
		setName(_name);
		setType(_type);
		setVarType(_var);
		index = _index;
	};
	~Var() {};
	//int isValidType() { return isValidType(type);};
	//int isValidVar() { return isValidVar(var); };
	int getType() { return type; };
	string getName() { return name; };
	int getVarType() { return var; };
	int getIndex() { return index; }
	int setName(string _name)
	{
		name = _name;
		return type;
	};

	void setType(int _type)
	{
		type = _type;
	};

	void setVarType(int _var)
	{
		var = _var;
	};

	int equal(Var* match)
	{
		if ((getName().compare(match->getName()) == 0) && (getType() == match->getType()) && (getVarType() == match->getVarType()))
			return 0;
		return 1;
	};

	int equal(string _name, int _type, int _varType)
	{
		if (getName().compare(_name) == 0 && (getType() == _type) && (getVarType() == _varType))
			return 0;
		return 1;
	}
	string toString()
	{
		string out = "V";
		out.append(to_string(getType()));
		out.append("{");
		out.append(getName());
		out.append("}");
		return out;
	};
};

// vector of 4LQSR Variables. Q stays for quantified.

class VariablesSet
{
private:
	vector< vector <Var> > VVL;
	vector< vector <Var> > VQL;
	//vector< vector <Var> > QVQL;
	int capacity;
	int nlevel;
public:
	VariablesSet() {};
	VariablesSet(int maxNVariable, int maxVSize, int maxQSize)
	{
		nlevel = maxNVariable;
		capacity = maxVSize;
		VQL.reserve(nlevel);  //initialize vectors for variables
		//QVQL.reserve(nlevel);  //initialize vectors for variables
		VVL.reserve(nlevel);
		for (int i = 0; i <= nlevel; i++)
		{
			VQL.push_back(vector<Var>());
			VVL.push_back(vector<Var>());
			//	QVQL.push_back(vector<Var>());
		}
		for (int i = 0; i <= nlevel; i++)
		{
			VQL.at(i).reserve(maxQSize);
			VVL.at(i).reserve(maxVSize);
			//	QVQL.at(i).reserve(maxQQSize);
		}
	};

	vector<Var>*getAt(vector<vector<Var>>& vec, int level)
	{
		if (level < vec.size())
			return &vec.at(level);
		return NULL;
	}

	vector<Var>* getVVLAt(int level)
	{
		if (level < VVL.size())
			return &VVL.at(level);
		return NULL;
	};

	vector<Var>* getVQLAt(int level)
	{
		if (level < VQL.size())
			return &VQL.at(level);
		return NULL;
	};

	//vector<Var>* getQVQLAt(int level)
	//{
	//	if (level < QVQL.size())
	//		return &QVQL.at(level);
	//	return NULL;
	//};


	int pushBack(vector<vector <Var>>& vec, int inslevel, string name, int level, int vartype)
	{
		if (vec.at(inslevel).size() < vec.at(inslevel).capacity())
		{
			vec.at(inslevel).push_back(*new Var(name, level, vartype, getSizeAt(vec, inslevel)));
			return 0;
		}
		return 1;
	}

	int VVLPushBack(int inslevel, string name, int level, int vartype)
	{
#ifdef debug 
#ifdef debuginsertf
		logFile << "-----Inserting Data in Variable Set of level: " << inslevel << ". Name: " << name << ". Level: " << level << ". Type: " << vartype << endl;
		logFile << "-----Variable Set of level: " << inslevel << " has size: " << VVL.at(inslevel).size() << " and capacity: " << VVL.at(inslevel).capacity() << endl;
#endif
#endif // debug

		if (VVL.at(inslevel).size() < VVL.at(inslevel).capacity())
		{
			VVL.at(inslevel).push_back(*new Var(name, level, vartype, VVLGetSizeAt(inslevel)));
			return 0;
		}
		return 1;
	}

	/*
	int VQLPushBack(int inslevel, string name, int level, int vartype)
	{
#ifdef debug
#ifdef debuginsertf
		logFile << "-----Inserting Data in Quantified Variable Set of level: " << inslevel << ". Name: " << name << ". Level: " << level << ". Type: " << vartype << endl;
		logFile << "-----Quantified Variable Set of level: " << inslevel << " has size: " << VQL.at(inslevel).size() << " and capacity: " << VQL.at(inslevel).capacity() << endl;
#endif
#endif // debug
		if (VQL.at(inslevel).size() < VQL.at(inslevel).capacity())
		{
			VQL.at(inslevel).push_back(*new Var(name, level, vartype, VQLGetSizeAt(inslevel)));
			return 0;
		}
		return 1;
	}

	int QVQLPushBack(int inslevel, string name, int level, int vartype)
	{
#ifdef debug
#ifdef debuginsertf
		logFile << "-----Inserting Data in Quantified Variable Set of level: " << inslevel << ". Name: " << name << ". Level: " << level << ". Type: " << vartype << endl;
		logFile << "-----Quantified Variable Set of level: " << inslevel << " has size: " << QVQL.at(inslevel).size() << " and capacity: " << QVQL.at(inslevel).capacity() << endl;
#endif
#endif // debug
		if (QVQL.at(inslevel).size() < QVQL.at(inslevel).capacity())
		{
			QVQL.at(inslevel).push_back(*new Var(name, level, vartype, QVQLGetSizeAt(inslevel)));
			return 0;
		}
		return 1;
	}*/

	vector<vector <Var>>& getVQL() { return VQL; }
	vector<vector <Var>>& getVVL() { return VVL; }
	Var* getBack(vector<vector <Var>>& vec, int level) { return &vec.at(level).back(); }
	Var* VVLGetBack(int level) { return &VVL.at(level).back(); }
	//	Var* VQLGetBack(int level) { return &VQL.at(level).back(); }
	//	Var* QVQLGetBack(int level) { return &QVQL.at(level).back(); }
	size_t getSize(vector<vector <Var>>& vec) { return vec.size(); }
	//	size_t VVLGetSize() { return VVL.size(); }
	size_t VQLGetSize() { return VQL.size(); }
	//	size_t QVQLGetSize() { return QVQL.size(); }
	size_t getSizeAt(vector<vector <Var>>& vec, int level) { return vec.at(level).size(); }
	size_t VVLGetSizeAt(int level) { return VVL.at(level).size(); }
	size_t VQLGetSizeAt(int level) { return VQL.at(level).size(); }
	//	size_t QVQLGetSizeAt(int level) { return QVQL.at(level).size(); }
};

//Special chars of a formula
class Operators
{
private:
	vector<string> logOp = { "$OR","$AD","$RO","$DA", "$NG" }; //logic operators
	vector<string> setOp = { "$IN", "$EQ", "$NI", "$QE", "$OA", "$AO", "$CO" }; //set operators
	vector<string> qutOp = { "$FA" };	//quantifiers
public:
	Operators() {};
	size_t getLogOpSize() { return logOp.size(); };
	size_t getSetOpSize() { return setOp.size(); };
	size_t getQuantiOpSize() { return qutOp.size(); };
	int getLogOpValue(string s)
	{
		for (int i = 0; i < getLogOpSize(); i++)
		{
			if (logOp[i].compare(s) == 0)
				return i;
		}
		return -1;
	};

	int getSetOpValue(string s)
	{
		int i = 0;
		for (; i < getSetOpSize(); i++)
		{
			if (setOp[i].compare(s) == 0)
				return i;
		}
		return -1;
	};

	int getQuantiOpValue(string s)
	{
		for (int i = 0; i < getQuantiOpSize(); i++)
		{
			if (qutOp[i].compare(s) == 0)
				return i;
		}
		return -1;
	};

	string getLogOpElement(int index)
	{
		if (index < getLogOpSize())
			return logOp[index];
		return NULL;
	};

	string getSetOpElement(int index)
	{
		if (index < getSetOpSize())
			return setOp[index];
		return NULL;
	};

	string getQuantiOpElement(int index)
	{
		if (index < getQuantiOpSize())
			return qutOp[index];
		return NULL;
	};

	/*
	Return the value of the given string representing a logic operator
	*/
	int checkLogOp(string *s)
	{
		for (const string &op : logOp)
		{
			if (op.compare(*s) == 0)
				return 1;
		}
		return 0;
	}
};

// VariablesSet varSet = VariablesSet(3, 100);
// Operators operators = Operators();

VariablesSet varSet;
Operators operators;

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
	COn il vettore consideriamo anche i possibili datatype groups
	*/
public:
	Atom() {};
	Atom(int op, vector<Var*> vec)
	{
		//components.reserve(maxVarSize);
		setAtomOp(op);
		components = vec;
	};

	vector<Var*>& getElements()
	{
		return components;
	};

	int getAtomOp()
	{
		return atomOp;
	};

	void setAtomOp(int value)
	{
		atomOp = value;
	};

	Var* getElementAt(int index)
	{
		if (index < components.size())
			return components.at(index);
		return NULL;
	}

	~Atom() {};

	/*
	Use Carefully. Remember the position of the left/right operand
	*/
	void addElement(Var* element)
	{
		getElements().push_back(element);
	};

	string toString()
	{
		string out = "(";
		if (getElements().size() > 2)
		{
			out.append("$OA ");
			out.append(getElementAt(1)->toString());
			out.append(" $CO ");
			out.append(getElementAt(2)->toString());
			out.append("$AO");
		}
		else
			out.append(getElementAt(1)->toString());
		out.append(" ");
		out.append(operators.getSetOpElement(getAtomOp()));
		out.append(" ");
		out.append(getElementAt(0)->toString());
		out.append(")");
		return out;
	};

	int equals(Atom &atom)
	{
		if ((this->getElements().size() == atom.getElements().size()) && (this->getAtomOp() == atom.getAtomOp()))
		{
			int j = 0;
			for (; j < this->getElements().size(); j++)
			{
				if (this->getElementAt(j)->equal(atom.getElementAt(j)) != 0)
					break;
			}
			if (j == this->getElements().size())
			{
				return 0;
			}
		}
		return 1;
	};
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
	Formula()
	{
		setAtom(NULL);
		setOperand(-1);
		fulfilled = 1;
		setLSubformula(NULL);
		setRSubformula(NULL);
		setPreviousFormula(NULL);
	};
	Formula(Atom *at, int op)
	{
		setAtom(at);
		setOperand(op);
		fulfilled = 1;
		setLSubformula(NULL);
		setRSubformula(NULL);
		setPreviousFormula(NULL);
	};
	Formula(Atom *at, int op, Formula *lf, Formula *rf)
	{
		setAtom(at);
		setOperand(op);
		lsubformula = lf;
		rsubformula = rf;
		fulfilled = 1;
		(*lsubformula).setPreviousFormula(this);
		(*rsubformula).setPreviousFormula(this);
	};
	// vector<Var*>* getQuantifiedSet() { return &quantified; };
	Atom* getAtom() { return atom; };
	int getOperand() { return operand; };
	Formula* getLSubformula() { return lsubformula; };
	Formula* getRSubformula() { return rsubformula; };
	Formula* getPreviousformula() { return pformula; };
	void setOperand(int op) { operand = op; };
	void setAtom(Atom *at) { atom = at; };
	void setLSubformula(Formula *sub) { lsubformula = sub; };
	void setRSubformula(Formula *sub) { rsubformula = sub; };
	void setPreviousFormula(Formula *prev) { pformula = prev; }
	void setFulfillness(int val) { fulfilled = val; };
	int getFulfillness() { return fulfilled; };
	~Formula() {};
	string toString()
	{
		if (getAtom() != NULL)
		{
			return (getAtom()->toString().append(" "));
		}
		else if (getOperand() > -1 && getLSubformula() != NULL && getRSubformula() != NULL)
		{
			string ret = "( ";
			ret.append(getLSubformula()->toString());
			ret.append(" ");
			ret.append(operators.getLogOpElement(getOperand()));
			ret.append(" ");
			ret.append(getRSubformula()->toString());
			ret.append(")");
			return ret;
		}
		else if (getOperand() > -1)
			return (operators.getLogOpElement(getOperand()));
		else return "NULL";
	}
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
	int containsFormula(Formula &F) //to be completed and performed
	{
		for (int i = 0; i < this->getSetFormulae().size(); i++)
		{
			Atom* locAt = this->getSetFormulae().at(i).getAtom();
			if (locAt != NULL && F.getAtom() != NULL)
			{
				if (locAt->equals(*(F.getAtom())) == 0)
				{
					return 0;
				}
			}

		}
		return 1;
	}
public:
	Node(int size) { setFormula.reserve(size); leftChild = rightChild = father = NULL; };
	Node() { leftChild = rightChild = father = NULL; };
	Node(vector<Formula>& formula) { setFormula = formula; leftChild = rightChild = father = NULL; };
	Node* getLeftChild() { return leftChild; };
	Node* getRightChild() { return rightChild; };
	Node* getFather() { return father; }
	void setRightChild(Node* child) { rightChild = child; child->setFather(this); };
	void setLeftChild(Node* child) { leftChild = child; child->setFather(this); };
	void setFather(Node* f) { father = f; };
	void insertFormula(Formula& f)
	{
		setFormula.push_back(f);
	};
	void insertCheckFormula(Formula& f)
	{
		if (this->containsFormula(f) == 1)
			setFormula.push_back(f);
	};
	vector<Formula>& getSetFormulae() { return setFormula; };
	void setSetFormulae(vector<Formula>& input) { setFormula = input; };
	~Node() {};
};


/*
The Tableau.
*/

class Tableau
{
private: Node* radix;
private: vector <Node*> openBranches;
private: vector <Node*> closedBranches;
private: vector < vector < vector <Var*> > > EqSet;
public:
	Tableau(Node* initial) { radix = initial; };
	Tableau(int size_radix) { radix = new Node(size_radix); };
	Node* getTableau() { return radix; };
	int insertFormula(Formula &f) { radix->insertFormula(f);  return 0; };
	vector<Node*>& getOpenBranches() { return openBranches; };
	vector<Node*>& getClosedBranches() { return closedBranches; };
	vector < vector < vector <Var*> > >& getEqSet() { return EqSet; }
	void addClosedBranch(Node* node) { getClosedBranches().push_back(node); }
	void areInEqClass(Var& var1, int& varclass1, int& indx1, Var& var2, int& varclass2, int& indx2, int brindx)
	{
		for (int i = 0; i < getEqSet().at(brindx).size(); i++)
		{
			for (int j = 0; j < getEqSet().at(brindx).at(i).size(); j++)
			{
				if (var1.equal(getEqSet().at(brindx).at(i).at(j)) == 0)
				{
					indx1 = j;
					varclass1 = i;
				}
				if (var2.equal(getEqSet().at(brindx).at(i).at(j)) == 0)
				{
					indx2 = j;
					varclass2 = i;
				}
			}
		}
	}

	int sameEqClass(Var& var1, Var& var2, int brindx)
	{
		for (int i = 0; i < getEqSet().at(brindx).size(); i++)
		{
			int varclass1 = -1;
			int varclass2 = -1;
			for (int j = 0; j < getEqSet().at(brindx).at(i).size(); j++)
			{
				if (var1.equal(getEqSet().at(brindx).at(i).at(j)) == 0)
					varclass1 = i;
				if (var2.equal(getEqSet().at(brindx).at(i).at(j)) == 0)
					varclass2 = i;
				if (varclass1 > -1 && varclass2 > -1)
					return 0;
			}
		}
		return 1;
	}

	void mergeEqClass(int brindx, int indx1, int indx2)
	{
#ifdef debug 
#ifdef eqsetdebug
		logFile << "------- Merging equivalence classes with representative " << getEqSet().at(brindx).at(indx1).at(0)->toString() << " and " << getEqSet().at(brindx).at(indx2).at(0)->toString() << " in branch " << brindx << endl;
#endif
#endif // debug

		getEqSet().at(brindx).at(indx1).insert(getEqSet().at(brindx).at(indx1).end(), getEqSet().at(brindx).at(indx2).begin(), getEqSet().at(brindx).at(indx2).end());
		getEqSet().at(brindx).erase(getEqSet().at(brindx).begin() + indx2);
	};

	void mergeEqClass(int brindx, int indx1, Var& var)
	{
#ifdef debug 
#ifdef eqsetdebug
		logFile << "------- Merging equivalence class with representative " << getEqSet().at(brindx).at(indx1).at(0)->toString() << " and " << var.toString() << " in branch " << brindx << endl;
#endif
#endif // debug


		int i = 0;
		for (; i < getEqSet().at(brindx).at(indx1).size() && (getEqSet().at(brindx).at(indx1).at(i)->equal(&var) == 1); i++)
		{
		}
		if (i == getEqSet().at(brindx).at(indx1).size())
			getEqSet().at(brindx).at(indx1).push_back(&var);
	};

	void mergeEqClass(int brindx, Var& var, int indx1)
	{
#ifdef debug 
#ifdef eqsetdebug
		logFile << "------- Merging " << var.toString() << " and equivalence class with representative " << getEqSet().at(brindx).at(indx1).at(0)->toString() << " in branch " << brindx << endl;
#endif
#endif // debug
		getEqSet().at(brindx).at(indx1).insert(getEqSet().at(brindx).at(indx1).begin(), &var);

	};

	~Tableau() {};
};


/*
print a stack of string
*/
void printStack(stack<Formula*> st)
{

	while (!st.empty())
	{
		cout << st.top()->toString() << endl;
		st.pop();
	}
}

void printStack(stack<string> st)
{
	stack<string> out;
	cout << "Stack:" << endl;
	while (!st.empty())
	{
		out.push(st.top());
		st.pop();
	}
	while (!out.empty())
	{
		cout << out.top() << endl;
		out.pop();
	}
}

int containsVariableName(vector<Var>* vect, Var** found, const string *name, const int *start)
{
	for (int s = *start; s < vect->size(); s++)
	{
		if ((name->compare((vect->at(s)).getName()) == 0))
		{
			*found = &(vect->at(s));
			return 0;
		}
	}
	return -1;
}

/*
Extract from the given string the name and the level of the variable
*/
int retrieveVarData(const string input, string* name, int* level)
{
#ifdef debug 
#ifdef debuginsertf
	logFile << "-----Extracting Data From Var. Input String: " << input << endl;
#endif
#endif // debug
	*level = input.at(1) - '0';
	*name = input.substr(3, (input.find_last_of('}') - 3));
#ifdef debug 
#ifdef debuginsertf
	logFile << "-----Data From Var Computed. Name: " << *name << ". Level:" << *level << endl;
#endif
#endif // debug
	return 0;
}


Var* createVarFromString(vector<vector<Var>>& vec, vector<vector<Var>>& vec2, string *name, int *level, int *vartype, int *start, int *typeformula)
{
	Var* ret;
#ifdef debug 
#ifdef debuginsertf
	logFile << "-----Creating  Variable.  Name:" << *name << ". Level: " << *level << ". Type: " << *vartype << endl;
#endif
#endif // debug
	if (containsVariableName(varSet.getAt(vec2, *level), &ret, name, start) == 0)
	{
#ifdef debug 
#ifdef debuginsertf
		logFile << "-----Variable Found in Set. " << ret->toString() << endl;
#endif
#endif // debug
		return ret;
	}
	else if (containsVariableName(varSet.getAt(vec, *level), &ret, name, start) == 0)
	{
#ifdef debug  
#ifdef debuginsertf
		logFile << "-----Variable Found in Quantified Set. " << ret->toString() << endl;
#endif
#endif // debug
		return ret;
	}
	else
	{
		varSet.pushBack(vec2, *level, *name, *level, *vartype);
		ret = varSet.getBack(vec2, *level);
#ifdef debug 
#ifdef debuginsertf
		logFile << "-----Adding Variable to Set. " << ret->toString() << endl;
#endif
#endif // debug
		return ret;

	}
}


Var* createQVarFromString(vector<vector<Var>>& vec, string *name, int *level, int *vartype, int *start)
{
#ifdef debug 
#ifdef debuginsertf
	logFile << "-----Creating Quantified Variable.  Name:" << *name << ". Level: " << *level << ". Type: " << *vartype << endl;
#endif
#endif // debug
	Var* ret;
	if (containsVariableName(varSet.getAt(vec, *level), &ret, name, start) == 0)
	{
#ifdef debug 
#ifdef debuginsertf
		logFile << "-----Quantified Variable Found in Set." << ret->toString() << endl;
#endif
#endif // debug
		return ret;
	}
	else
	{
		varSet.pushBack(vec, *level, *name, *level, *vartype);
		ret = varSet.getBack(vec, *level);
#ifdef debug 
#ifdef debuginsertf
		logFile << "-----Adding Quantified Variable to Set." << ret->toString() << endl;
#endif
#endif // debug
		return ret;
	}
}


/*
Create an Atom from the given string
*/
int createAtom(vector<vector <Var>>& vec, vector<vector <Var>>& vec2, string input, Formula **formula, vector<int>& startQuantVect, int typeformula)
{

#ifdef debug  
#ifdef debuginsertf
	logFile << "-----Computing Atom: " << input << endl;
#endif
#endif // debug
	Var* var1;
	Var* var2;
	Var* var3;
	string name = string();
	int level = -1;
	Atom* atom = NULL;
	if (input[0] == '$') //case pair or quantifier
	{
		string head = input.substr(0, 3); //cout << head << endl;	
		string match = input;
		if (head.compare("$OA") == 0)  //case  pair
		{

			match = input.substr(3, input.size() - 1);
			size_t found = match.find("$");
			retrieveVarData(match.substr(0, found), &name, &level);
			var1 = createVarFromString(vec, vec2, &name, &level, new int(0), &startQuantVect.at(level), &typeformula);
			match = match.substr(found + 3, match.size() - 1); //here the comma
			found = match.find("$");
			retrieveVarData(match.substr(0, found), &name, &level);
			var2 = createVarFromString(vec, vec2, &name, &level, new int(0), &startQuantVect.at(level), &typeformula);
			match = match.substr(found + 3, match.size() - 1);
			int op = operators.getSetOpValue(match.substr(0, 3));
			match = match.substr(3, match.size() - 1);
			retrieveVarData(match, &name, &level);
			var3 = createVarFromString(vec, vec2, &name, &level, new int(0), &startQuantVect.at(level), &typeformula);
			atom = new Atom(op, { var3, var1, var2 });
			//Atom* atom = new Atom(0, { new Var(name,level,0), new Var("b1",0,0), new Var("c1",0,0) });
			*formula = (new Formula(atom, -1));
			//cout << "Atom found: " << formula->print() << endl;			
		}
		else if (head.compare("$FA") == 0)  //case quantified variable
		{
			retrieveVarData(match.substr(3, match.size() - 1), &name, &level);
			var1 = createQVarFromString(vec, &name, &level, new int(1), &startQuantVect.at(level));
			*formula = NULL; //var
		}
	}
	else if (input[0] == 'V') //case single var
	{
		size_t found = input.find("$");//case no pair
		if (found != string::npos)
		{
			retrieveVarData(input.substr(0, found), &name, &level);
			var1 = createVarFromString(vec, vec2, &name, &level, new int(0), &startQuantVect.at(level), &typeformula);
			int op = operators.getSetOpValue(input.substr(found, 3));
			retrieveVarData(input.substr(found + 3, input.size() - 1), &name, &level);
			var2 = createVarFromString(vec, vec2, &name, &level, new int(0), &startQuantVect.at(level), &typeformula);
			atom = new Atom(op, { var2, var1 });
			*formula = (new Formula(atom, -1));
		}
	}
#ifdef debug  
#ifdef debuginsertf
	if (atom != NULL)
		logFile << "-----Atom created: " << atom->toString() << endl;
#endif
#endif // debug
	return 0;
}

/*
Parse a string representing an internal formula and return the corresponding internal formula.
*/
int parseInternalFormula(vector<vector <Var>>& vec, vector<vector <Var>>& vec2, const string *inputformula, Formula **outformula, vector<int>& startQuantVect, int typeformula)
{

#ifdef debug  
	logFile << "-----Analizyng input formula" << endl;
#endif // debug
	stack<string> stackFormula;
	stack<Formula*> stformula; //tracking subformulae
	stackFormula.push(*inputformula);
	string top = stackFormula.top();
	stackFormula.pop();
	string atom = string();
	string operand = string();
	Formula* formula = NULL;
	for (int i = 0; i < top.length(); i++)
	{
		char c = top.at(i);
		switch (c)
		{
		case '(': stackFormula.push(string(1, c)); break;
		case ')':
			if (!atom.empty())
			{
				stackFormula.pop();
#ifdef debug 
#ifdef debuginsertf
				logFile << "-----Candidate atom found: " << atom << endl;
#endif
#endif // debug

				createAtom(vec, vec2, atom, &formula, startQuantVect, typeformula);
				if (formula != NULL) // creation of the formula 
				{
					stformula.push(formula);
				}
				atom.clear();
			}
			else
			{
				Formula *rightf = stformula.top();
				stformula.pop();
				Formula *centerf = stformula.top();
				stformula.pop();
				Formula *leftf = stformula.top();
				stformula.pop();
				centerf->setLSubformula(leftf);
				centerf->setRSubformula(rightf);
#ifdef debug 
#ifdef debuginsertf
				logFile << "-----Computing subformula: " << centerf->toString() << endl;
#endif
#endif // debug
				stformula.push(centerf);
				stackFormula.pop();
			}
			break;
		case '$': operand = string(1, c) + string(1, top.at(i + 1)) + string(1, top.at(i + 2));
			i = i + 2;
			if (operators.checkLogOp(&operand) != 0)
			{
				stformula.push(new Formula(NULL, operators.getLogOpValue(operand)));
			}
			else
			{
				atom.append(operand);
			}
			operand.clear();
			break;
		case ' ': break;
		default:	atom.append(string(1, c)); break;
		}
	}
	*outformula = (stformula.top());
#ifdef debug  
	logFile << "-----Final Formula: " << (*outformula)->toString() << endl;
#endif // debug
	return 0;
}

//only for test
void printVector(vector<Var>& v)
{
	for (Var element : v)
	{
		cout << element.toString() << ", " << element.getIndex() << endl;
	}

}


/*
Create an object of type Formula  from the given string representing a formula.
*/
int insertFormulaKB(vector<vector <Var>>& varset1, vector<vector <Var>>& varset2, string formula, Formula** ffinal, int* typeformula)
{
	/*
	The following vector of int represents the position of the quantified variables of the current formula.
	Initially the vector plainly coincides the the end of the vector modified in the previous execution.
	*/
#ifdef debug  
	logFile << "---Formula Inserted: " << formula << endl;
#endif // debug
	//Formula *ffinal;
	vector<int> vqlsize;
	for (int i = 0; i < varset1.size(); i++)
		vqlsize.push_back((int)varset1.at(i).size());
	parseInternalFormula(varset1, varset2, &formula, ffinal, vqlsize, *typeformula);
	//vec.push_back(*ffinal);
#ifdef debug  
	logFile << "---Formula Ended " << endl;
#endif // debug
	return 0;
}

int insertFormulaKB(vector<vector <Var>>& varset, vector<vector <Var>>& varset2, string formula, vector<Formula> &vec, int* typeformula)
{
	Formula* f = NULL;
	int res = insertFormulaKB(varset, varset2, formula, &f, typeformula);
	vec.push_back(*f);
	delete(f);
	return res;
}

/*int insertFormulaKB(string s, vector<Formula> &t)
{
Formula* f;
insertFormula(&s, &f);
t.push_back(*f);
return 0;
} */

void copyAtom(Atom* source, Atom* dest)
{
	//Atom(int op, vector<Var*> vec)
	dest->setAtomOp(source->getAtomOp());
	for (Var* var : source->getElements())	
	  dest->addElement(var);
}

Atom* copyAtom(Atom* atom, const string *qvar, Var* dest)
{
	Atom* fin = new Atom();
	fin->setAtomOp(atom->getAtomOp());
	for (int i = 0; i < atom->getElements().size(); i++)
	{
		if (qvar != NULL && dest != NULL && ((atom->getElementAt(i)->getName()) == *qvar))
			fin->addElement(dest);
		else
			fin->addElement((atom->getElementAt(i)));
	}
	return fin;
}

Formula* copyFormula(Formula* formula, Formula* father, const string *qvar, Var* dest)
{

	if (formula == NULL)
		return NULL;
	Formula* fin = new Formula();
	fin->setOperand(formula->getOperand());
	if (formula->getAtom() == NULL)
		fin->setAtom(NULL);
	else
	{
		fin->setAtom(copyAtom(formula->getAtom(), qvar, dest));
	}
	fin->setPreviousFormula(father);
	fin->setLSubformula(copyFormula(formula->getLSubformula(), fin, qvar, dest));
	fin->setRSubformula(copyFormula(formula->getRSubformula(), fin, qvar, dest));
	return fin;
}

/*int containsQVar(Formula *f, string &s)
{
if (f == NULL)
return 0;
if (f->getAtom() != NULL)
{
for (Var* var : (f->getAtom()->getElements()))
{
if (var->getVarType() == 1)
{
s = (var->getName());
return 1;
}
}
}
return (containsQVar(f->getLSubformula(), s) + containsQVar(f->getRSubformula(), s));
}
*/
/*
Get the first occurence of quantified variable if present
*/
int containsQVar(Formula *fr, string &s)
{
	vector<Formula*> q = { fr };
	while (!q.empty())
	{
		Formula* f = q.back();
		q.pop_back();
		if (f != NULL)
		{
			if (f->getAtom() != NULL)
			{
				for (Var* var : (f->getAtom()->getElements()))
				{
					if (var->getVarType() == 1)
					{
						s = (var->getName());
						return 1;
					}
				}
			}
			q.push_back(f->getLSubformula());
			q.push_back(f->getRSubformula());
		}
	}
	return 0;
}

int instantiateFormula(Formula f, vector<Formula> &destination)
{
#ifdef debug  
	logFile << "------- Expanding Formula: " << f.toString() << endl;
#endif // debug
	string s;
	vector<Formula> tmp;
	tmp.push_back(f);
	while (!tmp.empty())
	{
		Formula top = tmp.back();
		tmp.pop_back();
		if (containsQVar(&top, s))
		{
			for (int i = 0; i < varSet.getVVLAt(0)->size(); i++)
				tmp.push_back(*(copyFormula(&top, NULL, &s, &varSet.getVVLAt(0)->at(i))));
		}
		else
		{
#ifdef debug 
#ifdef debugexpand
			logFile << "------- Expanded Formula: " << top.toString() << endl;
#endif
#endif // debug		
			if (top.getAtom() != NULL)
				top.setFulfillness(0);
			destination.push_back(top);
		}
	}

	return 0;
}



int expandKB(const vector<Formula> &inpf, vector <Formula> &out)
{
#ifdef debug  
	logFile << "--- Applying Expansion Rule" << endl;
#endif // debug
	int or = operators.getLogOpValue("$OR");
	vector <Formula> tmp;
	for (int i = 0; i < inpf.size(); i++)
		tmp.push_back(inpf.at(i));

	while (!tmp.empty())
	{

		Formula f = tmp.back();
#ifdef debug  
#ifdef debugexpand
		logFile << "----- Computing Formula: " << f.toString() << endl;
#endif
#endif // debug
		tmp.pop_back();
		if (f.getAtom() != NULL || f.getOperand()== or)
		{
			instantiateFormula(f, out);
		}
		else
		{
			f.getLSubformula()->setPreviousFormula(NULL);
			f.getRSubformula()->setPreviousFormula(NULL);
#ifdef debug  
#ifdef debugexpand
			logFile << "----- Obtaining Formula: " << f.getLSubformula()->toString() << endl;
			logFile << "----- Obtaining Formula: " << f.getRSubformula()->toString() << endl;
#endif
#endif // debug
			tmp.push_back(*(f.getLSubformula()));
			tmp.push_back(*(f.getRSubformula()));

			//cout << (f.getOperand()) << endl;
		}
	}
#ifdef debug  
	logFile << "--- End Expansion Rule" << endl;
#endif // debug
	return 0;
}

int checkAtomOpClash(int op1, int op2)
{
	if (abs(op1 - op2) == 2)
		return 0;
	return 1;
}

int checkAtomClash(Atom &atom1, Atom &atom2)
{
#ifdef debug 
#ifdef debugclash
	logFile << "-----Checking for Clash: " << atom1.toString() << " and " << atom2.toString() << endl;
#endif // debug
#endif
	if (atom1.getElements().size() == atom2.getElements().size())
	{
		for (int i = 0; i < atom1.getElements().size(); i++)
		{
			//cout<<atom1.getElementAt(i)->toString()<<endl;
			//cout << atom2.getElementAt(i)->toString() << endl;
			//cout << atom1.getElementAt(i)->equal(atom2.getElementAt(i)) << endl;
			if (atom1.getElementAt(i)->equal(atom2.getElementAt(i)) != 0)
				return 1;
		}
		return checkAtomOpClash(atom1.getAtomOp(), atom2.getAtomOp());
	}
	return 1;
}

int checkAtomClash(Atom &atom) //0 for clash 1 for no-clash
{
#ifdef debug
#ifdef debugclash
	logFile << "-----Checking for Clash: " << atom.toString() << endl;
#endif
#endif // debug
	if (atom.getElements().size() == 2 && atom.getAtomOp() == 3 && atom.getElementAt(0)->equal(atom.getElementAt(1)) == 0)
		return 0;
	return 1;
}

int checkAtomsClash(vector<Atom*> &vec)
{

	for (int i = 0; i < vec.size() - 1; i++)
	{
#ifdef debug 
#ifdef debugclash
		logFile << "-----Checking for Clash: " << vec.at(i)->toString() << " and " << vec.at(i + 1)->toString() << endl;
#endif // debug
#endif
		if (checkAtomClash(*vec.at(i), *vec.at(i + 1)))
			return 0;
	}
	return 1;
}


/*
Check for Clash between the given candidate and a vector of formulae starting from the given index
*/
int checkVectorClash(Atom* candidate, vector<Formula> &formset, int start)
{
#ifdef debug  
	logFile << "---Checking for Clash in Vector of Formulae" << endl;
#endif // debug

	if (candidate != NULL) //atomic formula
	{
		if (checkAtomClash(*candidate) == 0)  // type a != a
		{
#ifdef debug  
			logFile << "-----Clash at: " << candidate->toString() << endl;
#endif // debug
			return 0;
		}
		for (int j = start; j < formset.size(); j++)
		{
			if (formset.at(j).getAtom() != NULL)
			{
				if (checkAtomClash(*formset.at(j).getAtom()) == 0)
				{
#ifdef debug  
					logFile << "-----Clash at: " << formset.at(j).getAtom()->toString() << endl;
#endif // debug
					return 0;
				}
				if (checkAtomClash(*candidate, *formset.at(j).getAtom()) == 0)
				{
#ifdef debug  
					logFile << "-----Clash at: " << candidate->toString() << "," << formset.at(j).getAtom()->toString() << endl;
#endif // debug
					return 0;
				}
			}
		}
	}
#ifdef debug  
	logFile << "---End Check for Clash in Vector of Formulae" << endl;
#endif // debug
	return 1;
}

/*
Check for Clash in a Vector of formulae
*/
int checkNodeClash(vector<Formula> &formset)
{
	for (int i = 0; i < formset.size() - 1; i++)
	{
		if (checkVectorClash(formset.at(i).getAtom(), formset, i + 1) == 0)
			return 0;
	}
	return 1;
}




/*
Chech if in the current node there is a formula that makes the entire branch inconsistent
*/

int checkBranchClash(Node* node, Tableau& tableau)
{
#ifdef debug  
	logFile << "---Checking for Clash in the Current branch" << endl;
#endif // debug
	vector<Formula>local = node->getSetFormulae();
	Node* iterator = node->getFather();
	while (iterator != NULL)
	{
		for (int i = 0; i < local.size(); i++)
		{
			Atom* at = local.at(i).getAtom();
			if (at != NULL && (checkVectorClash(at, iterator->getSetFormulae(), 0) == 0))
			{
#ifdef debug
#ifdef debugclash
				logFile << "----- Adding Closed Branch to Tableau" << endl;
#endif
#endif // debug
				(tableau.addClosedBranch(node));
				return 0;
			}
		}
		iterator = iterator->getFather();
	}
	return 1;
}


/*
Return the set of atomic formula contained in a formula.
*/

void getAtomSet(Formula &f, vector<Atom*> &outf)
{
	if (f.getAtom() != NULL)
		return;
	stack <Formula*> st;
	st.push(f.getRSubformula());
	st.push(f.getLSubformula());
	while (!st.empty())
	{
		Formula* tmp = st.top();
		if (tmp->getAtom() != NULL)
			outf.push_back(tmp->getAtom());                       //There should be only OR because of CNF formula		
		st.pop();
		if (tmp->getRSubformula() != NULL)
			st.push(tmp->getRSubformula());
		if (tmp->getLSubformula() != NULL)
			st.push(tmp->getLSubformula());
	}
}

void closeTableauRoot(Tableau& T)
{
#ifdef debug
#ifdef debugclash
	logFile << "-----Closed Tableau Found on Tableau Expansion " << endl;
#endif
#endif // debug
	T.getOpenBranches().clear();
	T.getClosedBranches().clear();
	T.getClosedBranches().push_back(T.getTableau());
}


Atom* negatedAtom(Atom *input)
{
	Atom* out = copyAtom(input, NULL, NULL);
	int neg = (out->getAtomOp() >= 2 ? -2 : 2);
	out->setAtomOp(out->getAtomOp() + neg);
	return out;
}


int checkBranchClash(Atom* atom, Node* node)
{
	Node* iterator = node;
	while (iterator != NULL)
	{
		//	vector<Formula>local = node->getSetFormulae();
		//	for (int i = 0; i < local.size(); i++)
		//	{
		if ((checkVectorClash(atom, iterator->getSetFormulae(), 0) == 0))
			return 0;
		//	}
		iterator = iterator->getFather();
	}
	return 1;
}

void ERule(Atom* atom, Node* node)
{
#ifdef debug  
	logFile << "---Applying E-RULE" << endl;
#endif // debug		

#ifdef debug  
#ifdef debugexpand
	logFile << "----- Computing Atom from E-Rule: " << copyAtom(atom, NULL, NULL)->toString() << endl;
#endif
#endif // debug

	Node* tmp = node;
	vector<Node*> newNodeSet;
	Atom* neg = negatedAtom(atom);
	//	if (checkBranchClash(neg, tmp) == 1)		  //solve thi inefficiency
	node->insertFormula(*(new Formula(copyAtom(atom, NULL, NULL), -1)));
}

void PBRule(vector<Atom*> atoms, Node* node, vector<Node*> &nodeSet)
{
#ifdef debug  
	logFile << "---Applying PB-RULE" << endl;
#endif // debug		

#ifdef debug  
#ifdef debugexpand
	logFile << "----- Computing Atom from PB-Rule. " << endl;
#endif
#endif // debug
	Node* tmp = node;
	vector<Node*> newNodeSet;
	/*	for (int i = 0; i < atoms.size(); i++)           //solve this inefficiency. Looking for positive in the branch.
		{
			Atom* neg = negatedAtom(atoms.at(i));
			if (checkBranchClash(neg, tmp) == 0)
			{
				newNodeSet.push_back(tmp);
				nodeSet.insert(nodeSet.end(), newNodeSet.begin(), newNodeSet.end());
				return;
			}
		} */

	for (int i = 0; i < atoms.size() - 1; i++)
	{
#ifdef debug  
#ifdef debugexpand
		logFile << "------ Computing Atom from PB-Rule. " << copyAtom(atoms.at(i), NULL, NULL)->toString() << endl;
		logFile << "------ Computing Atom from PB-Rule. " << (negatedAtom(atoms.at(i)))->toString() << endl;
#endif
#endif // debug
		Atom* neg = negatedAtom(atoms.at(i));
		tmp->setLeftChild(new Node(vector<Formula> {*(new Formula(copyAtom(atoms.at(i), NULL, NULL), -1))}));
		newNodeSet.push_back(tmp->getLeftChild());
		tmp->setRightChild(new Node(vector<Formula> {*(new Formula(negatedAtom(atoms.at(i)), -1))}));
		tmp = tmp->getRightChild();
	}
#ifdef debug  
#ifdef debugexpand
	logFile << "------ Computing Atom from PB-Rule. " << (copyAtom(atoms.at(atoms.size() - 1), NULL, NULL))->toString() << endl;
#endif
#endif // debug
	tmp->insertFormula(*(new Formula(copyAtom(atoms.at(atoms.size() - 1), NULL, NULL), -1)));
	newNodeSet.push_back(tmp);
	nodeSet.insert(nodeSet.end(), newNodeSet.begin(), newNodeSet.end());
}




int checkAtoms(Atom* atom, Node* node)
{
	Node* iterator = node;
	while (iterator != NULL)
	{
		vector<Formula> vec = iterator->getSetFormulae();
		for (int i = 0; i < vec.size(); i++)
		{
			if (vec.at(i).getAtom() != NULL)
			{
				if ((checkAtomClash(*atom, *(vec.at(i).getAtom())) == 0))
					return 0;
				if (atom->equals(*(vec.at(i).getAtom())) == 0)
					return 2;
			}
		}
		iterator = iterator->getFather();
	}
	return 1;
}


void chooseRule(Tableau &T, vector<Node*> &nodeSet, Formula &f)
{
	vector<Node*> newNodeSet;
	for (int b = 0; b < nodeSet.size(); b++)
	{
		vector<Atom*> atoms;
		vector<Atom*> atomset;
		getAtomSet(f, atomset);
		/*	if (checkAtomsClash(atomset))
			{
				newNodeSet.push_back(nodeSet.at(b));
				//T.getClosedBranches().push_back(nodeSet.at(b));
				break;
			}*/
		int check = 0;
		for (int j = 0; j < atomset.size(); j++)
		{
			check = checkAtoms(atomset.at(j), nodeSet.at(b));
			if (check == 1)
			{
				atoms.push_back(atomset.at(j));
			}
			else if (check == 2)
			{
				newNodeSet.push_back(nodeSet.at(b));
				break;
			}
		}
		if (check == 2)
			continue;
		switch (atoms.size())
		{
		case 0:       //case closed branch. 
		{
			T.getClosedBranches().push_back(nodeSet.at(b));
			break;
		}
		case 1:  //case of ERULE	
		{
			ERule(atoms.at(0), nodeSet.at(b));
			newNodeSet.push_back(nodeSet.at(b));
			break;
		}
		default:  //case of PBRULE
		{
			PBRule(atoms, nodeSet.at(b), newNodeSet);
			break;
		}
		}

	}
	nodeSet = newNodeSet;
}

void expandTableau(Tableau& T)
{
	vector<Node*> nonComBranches = vector<Node*>();
	nonComBranches.push_back(T.getTableau()); //initially only the root node
	vector<Formula> fset = T.getTableau()->getSetFormulae();
	for (int i = 0; i < fset.size(); i++)
	{
		if (fset.at(i).getAtom() == NULL)  //OR found
		{
#ifdef debug  
#ifdef debugexpand
			logFile << "------Fulfilling formula: " << fset.at(i).toString() << endl;
#endif
#endif // debug
			chooseRule(T, nonComBranches, fset.at(i));
		}
	}
	T.getOpenBranches() = nonComBranches;
}



int checkTableauRootClash(Tableau &T)
{
	return (checkNodeClash(T.getTableau()->getSetFormulae()) == 1);
	/*	for (int i = 0; i < T.getTableau()->getSetFormulae().size(); i++)
	{
	vector<Atom*> atomset;
	getAtomSet(T.getTableau()->getSetFormulae().at(i), atomset);
	if (checkAtomsClash(atomset))
	{
	return 0;
	}
	}
	return 1; */
}


/*
   Get the index of VVL of the vars given as input and return the element that come first.
*/

int getVarsOrder(Var &var1, Var &var2)
{
	if (var1.getVarType() != 0 || var2.getVarType() != 0 || var1.getType() != 0 || var2.getType() != 0)
		return -2;

	cout << var1.toString() << " " << var1.getIndex() << " " << var2.toString() << var2.getIndex() << endl;

	if (var1.getIndex() > var2.getIndex())
		return 2;
	else return 1;

	/*	cout << " " << var1.getIndex() << " "<<var2.getIndex() << endl;
		index1 = index2 = -1;
		for (int i = 0; i < varSet.getVVLAt(0)->size(); i++)
		{
		  if (var1.getName().compare(varSet.getVVLAt(0)->at(i).getName()) == 0)
			  index1 = i;
		  if (var2.getName().compare(varSet.getVVLAt(0)->at(i).getName()) == 0)
			  index2 = i;
		  if (index1 != -1 && index2 != -1)
			  break;
		}
		cout << " " << index1 << " " << index2 << endl;
		if (index1 != -1 && index2 != -1)
		{
			if (index1 > index2)
				return 2;
			else
				return 1;
		}
		else return -1; */
}

/*
   Check if the given vars are in some EqClass end returns the corresponding EqClass index and position.
*/
void areInEqClass(Var& var1, int& varclass1, int& indx1, Var& var2, int& varclass2, int& indx2, Tableau& tab, int brindx)
{
	for (int i = 0; i < tab.getEqSet().at(brindx).size(); i++)
	{
		for (int j = 0; j < tab.getEqSet().at(brindx).at(i).size(); j++)
		{
			if (var1.equal(tab.getEqSet().at(brindx).at(i).at(j)) == 0)
			{
				indx1 = j;
				varclass1 = i;
			}
			if (var2.equal(tab.getEqSet().at(brindx).at(i).at(j)) == 0)
			{
				indx2 = j;
				varclass2 = i;
			}
		}
	}
}



void insertVarsEqClass(Var& var1, Var& var2, Tableau& tab, int brindx)
{
#ifdef debug 
#ifdef eqsetdebug
	logFile << "------- Computing equivalence classes for " << var1.toString() << " and " << var2.toString() << " in branch " << brindx << endl;
#endif
#endif // debug
	int isRepresVar1 = -1; int isRepresVar2 = -1; int checkVar1 = -1; int checkVar2 = -1;
	tab.areInEqClass(var1, checkVar1, isRepresVar1, var2, checkVar2, isRepresVar2, brindx);
	// getVarsOrder(var1, var2, ord1, ord2);  

	if (checkVar1 > -1 && checkVar2 > -1)  //case both vars are in some EqClass
	{
		if (checkVar1 != checkVar2)      //make sure they are not in the same EqClass
		{
			if (isRepresVar1 != 0 || isRepresVar2 != 0) // one of them is not a representative of a EqClass		 
			{
				if ((*(tab.getEqSet().at(brindx).at(checkVar1).at(0))).getIndex() < (*(tab.getEqSet().at(brindx).at(checkVar2).at(0))).getIndex())          //Merge the second EqClass in the first
					tab.mergeEqClass(brindx, checkVar1, checkVar2);
				else        //Merge the first in the second
					tab.mergeEqClass(brindx, checkVar2, checkVar1);

			}
			else //both representative
			{
				if (var1.getIndex() < var2.getIndex())          //Merge the second EqClass in the first
					tab.mergeEqClass(brindx, checkVar1, checkVar2);
				else        //Merge the first in the second
					tab.mergeEqClass(brindx, checkVar2, checkVar1);
			}
		}
		//else do nothing
	}
	else // one of them is nor in some EqClass
	{

		if (checkVar1 == -1 && checkVar2 == -1)
		{
			if (var1.getIndex() < var2.getIndex())
				tab.getEqSet().at(brindx).push_back(vector<Var*>{ &var1, &var2 });
			else
				tab.getEqSet().at(brindx).push_back(vector<Var*>{ &var2, &var1 });
		}
		else if (checkVar1 == -1)
		{
			if ((*(tab.getEqSet().at(brindx).at(checkVar2).at(0))).getIndex() > var1.getIndex())
				tab.mergeEqClass(brindx, var1, checkVar2);
			else
				tab.mergeEqClass(brindx, checkVar2, var1);
		}
		else
		{
			if ((*(tab.getEqSet().at(brindx).at(checkVar1).at(0))).getIndex() > var2.getIndex())
				tab.mergeEqClass(brindx, var2, checkVar1);
			else
				tab.mergeEqClass(brindx, checkVar1, var2);
		}
	}
}

void buildEqSet(Tableau& tab)
{
#ifdef debug 
#ifdef eqsetdebug
	logFile << "----- Computing equivalence classes set " << endl;
#endif
#endif // debug

	if (tab.getOpenBranches().size() == 0)  //no open branches case
		return;

	for (int i = 0; i < tab.getOpenBranches().size(); i++)
	{
		tab.getEqSet().push_back(vector< vector<Var*>>());

	}
	for (int i = 0; i < tab.getOpenBranches().size(); i++)
	{
		Node* node = tab.getOpenBranches().at(i);
		while (node != NULL)
		{
			for (int fcount = 0; fcount < node->getSetFormulae().size(); fcount++)
			{
				Formula localf = node->getSetFormulae().at(fcount);
				if (localf.getAtom() != NULL && localf.getAtom()->getAtomOp() == operators.getSetOpValue("$EQ")) //find equivalences
					if (localf.getAtom()->getElementAt(0)->equal(localf.getAtom()->getElementAt(1)) == 1)  // ignore the case a=a
					{
#ifdef debug 
#ifdef eqsetdebug
						logFile << "----- Computing equivalence classes for " << localf.getAtom()->toString() << " in branch " << i << endl;
#endif
#endif // debug

						Var* var1 = localf.getAtom()->getElements().at(0);
						Var* var2 = localf.getAtom()->getElements().at(1);
						insertVarsEqClass(*var1, *var2, tab, i);
					}
			}
			node = node->getFather();
		}
	}
}


int checkAtomClashEqSet(Atom &atom1, Atom &atom2, Tableau& t, int& brindx)
{
#ifdef debug 
#ifdef debugclash
	logFile << "-----Checking for Clash: " << atom1.toString() << " and " << atom2.toString() << " with Equivalence Classes" << endl;
#endif // debug
#endif


	if (atom1.getElements().size() == atom2.getElements().size())
	{
		if (checkAtomOpClash(atom1.getAtomOp(), atom2.getAtomOp()) == 1)
			return 1;

		if (atom1.getAtomOp() == 3 || atom1.getAtomOp() == 1) // case equivalence		
			return (t.sameEqClass(*(atom1.getElementAt(0)), *(atom2.getElementAt(0)), brindx)) + (t.sameEqClass(*(atom1.getElementAt(1)), *(atom2.getElementAt(1)), brindx));

		//other case
		if (atom1.getElementAt(0)->equal(atom2.getElementAt(0)) == 1)
			return 1;

		if (atom1.getElements().size() == 3)  //case  <a,b> in X^3
			return (t.sameEqClass(*(atom1.getElementAt(1)), *(atom2.getElementAt(1)), brindx)) + (t.sameEqClass(*(atom1.getElementAt(2)), *(atom2.getElementAt(2)), brindx));
		else
			return (t.sameEqClass(*(atom1.getElementAt(1)), *(atom2.getElementAt(1)), brindx));

	}
	return 1;
}



int checkAtomClashEqSet(Atom &atom, Tableau& T, int& brindx) //0 for clash 1 for no-clash
{
#ifdef debug
#ifdef debugclash
	logFile << "-----Checking for Clash: " << atom.toString() << " in EqSet" << endl;
#endif
#endif // debug
	if (atom.getElements().size() == 2 && atom.getAtomOp() == 3)
	{
		//cout << "clash" << atom.toString() << " "<< T.sameEqClass(*(atom.getElementAt(0)), *(atom.getElementAt(1)), brindx) <<endl;
		return (T.sameEqClass(*(atom.getElementAt(0)), *(atom.getElementAt(1)), brindx));
	}
	return 1;
}

int checkNodeClashEqSet(Atom& atom, Node& node, Tableau& T, int& brind)
{
	Node* tmp = &node;
	while (tmp != NULL)
	{
		for (int j = 0; j < tmp->getSetFormulae().size(); j++)
		{
			if (tmp->getSetFormulae().at(j).getAtom() != NULL)
			{
				if (checkAtomClashEqSet(atom, *(tmp->getSetFormulae().at(j).getAtom()), T, brind) == 0)
					return 0;
			}
		}
		tmp = tmp->getFather();
	}
	return 1;
}

void checkTableauClash(Tableau& T)
{
	vector<int> toBeclosed;
	for (int i = 0; i < T.getOpenBranches().size(); i++)
	{
		int tobreak = 0;
		Node* currentNode = T.getOpenBranches().at(i);
		int clash = 1;
		for (int j = 0; j < currentNode->getSetFormulae().size() - 1; j++)
		{
			if (currentNode->getSetFormulae().at(j).getAtom() != NULL)
			{

				if (checkAtomClashEqSet(*(currentNode->getSetFormulae().at(j).getAtom()), T, i) == 1 &&
					checkAtomClashEqSet(*(currentNode->getSetFormulae().at(j).getAtom()), *(currentNode->getSetFormulae().at(j + 1).getAtom()), T, i) == 1)
				{
					tobreak = checkNodeClashEqSet(*(currentNode->getSetFormulae().at(j).getAtom()), *(currentNode->getFather()), T, i);
				}
				else tobreak = 0;

				if (tobreak == 0)
				{
					i = i + 1;
					toBeclosed.push_back(i);
					T.getClosedBranches().push_back(currentNode);
					break;
				}
			}
		}

	}
	for (int i = 0; i < toBeclosed.size(); i++) //removing closed branches
	{
		T.getOpenBranches().erase(T.getOpenBranches().begin() + i);
	}
}

void readKBFromFile(string& name, vector<Formula>& KB)
{
	std::ifstream file(name);
	std::string str;
	cout << "Knowledge Base" << endl;
	int typeformula = 0;
	while (std::getline(file, str))
	{
		if ((str.rfind("//", 0) == 0) || str.empty())
			continue;
		cout << str << endl;
		insertFormulaKB(varSet.getVQL(), varSet.getVVL(), str, KB, &typeformula);
	}

}

void readQueryFromFile(string& name, vector<string>& stringSet)
{
#ifdef debug 
#ifdef debugquery
	logFile << "-----Reading query file" << endl;
#endif
#endif // debug
	std::ifstream file(name);
	std::string str;
	
	while (std::getline(file, str))
	{
		
		if ((str.rfind("//", 0) == 0) || str.empty())
			continue;
		stringSet.push_back(str);
	}
}


class QueryManager
{
private: vector< vector <Var> > QVQL;
		 vector< vector <Var> > QVVL;
		 vector<vector< pair<Var*, Var*>>> Match;
		 Formula formula;
		 int nlevel;
		 int maxQQSize;

public:
	QueryManager(int _nlevel, int _maxQQSize)
	{
		nlevel = _nlevel;
		maxQQSize = _maxQQSize;
		QVQL.reserve(nlevel);
		QVVL.reserve(nlevel);		
		Match=vector<vector< pair<Var*,Var*>>>(0);
		for (int i = 0; i <= nlevel; i++)
		{
			QVQL.push_back(vector<Var>());
			QVVL.push_back(vector<Var>());
		}
		for (int i = 0; i <= nlevel; i++)
		{
			QVVL.at(i).reserve(maxQQSize);
			QVQL.at(i).reserve(maxQQSize);
		}
	};

	void extractLiterals(Formula& f, vector<Atom*> &atoms)
	{
#ifdef debug 
#ifdef debugquery
		logFile << "---Executing Query" << endl;
		logFile << "----Query selected:" << f.toString() << endl;
#endif
#endif // debug
		vector<Formula*> tmp;
		tmp.push_back(&f);
		while (!tmp.empty())
		{
			Formula* back = tmp.back();
			tmp.pop_back();
			if (back != NULL)
			{
				if (back->getAtom() != NULL)
				{
					atoms.push_back(back->getAtom());
#ifdef debug 
#ifdef debugquery
					logFile << "-----Atom computed:" << atoms.back()->toString() << endl;
#endif
#endif // debug					 
				}
				tmp.push_back(back->getLSubformula());
				tmp.push_back(back->getRSubformula());
			}
		}
	};

public:
	vector<vector<Var>>& getQVQL() { return QVQL; };
	vector<vector<Var>>& getQVVL() { return QVVL; };
	vector<vector<pair<Var*, Var*>>>& getMatchSet() { return Match; };

	

	void executeQuery(Formula& f, Tableau& tableau)
	{
		vector<Atom*> qLits;
		formula = f;
		extractLiterals(formula, qLits);
		
		//Print Atoms from Query
		cout << "Literals from query: " << qLits.size() << endl;
		for (Atom* a : qLits)
			cout << a->toString() << endl;
		//---------------------------------
		for (Node* branch : tableau.getOpenBranches())
		{
			vector<vector<pair<Var*, Var*>>> matchSet(0, vector<pair<Var*,Var*>>(0)); //partial solutions set for the current branch
		   

            int qIter=0;
			for (;qIter<qLits.size(); qIter++)
			{
				if (qIter == 0)
				{
					//initialize the Tree 
					cout << "Initialization" << endl;
				}
				else
				{
					vector<vector<pair<Var*, Var*>>> localMatched(0, vector<pair<Var*, Var*>>(0));; //temporay support vector
					for (int solIter = 0; solIter < matchSet.size(); solIter++) //iterate over partial solutions
					{
                        
					}					
					//matchSet.clear();
					matchSet = localMatched;
				}
				if (matchSet.size()<0)
				{
					// fail for q_i
					break;
				}

			}
			if (qIter == qLits.size()) // solution for the branch
			{
				//return the solution
			}
		}
	};
};



void performQuery(string& str, Formula** formula, Tableau& tableau)
{
	QueryManager* queryManager = new QueryManager(5, 50);
	int typeformula = 1; cout << "---------" << endl; 
	insertFormulaKB(queryManager->getQVQL(), queryManager->getQVVL(), str, formula, &typeformula);
	queryManager->executeQuery(**formula, tableau);

	for (vector<Var> i : queryManager->getQVQL())
		for (Var j : i)
			cout << j.toString() << endl;
};

void performQuerySet(vector<string>& strings, vector<Formula>& formulae, Tableau& tableau)
{
#ifdef debug 
#ifdef debugquery
	logFile << "-----Parsing line:" << strings.at(0) << endl;
#endif
#endif // debug
	for (string s : strings)
	{
		Formula *f = NULL;
		performQuery(s, &f, tableau);
		formulae.push_back(*f);
	}
};

/*
  Some printing function
*/
void printClosedBranches(Tableau& tableau)
{
	cout << "Printing closed branches" << endl;
	for (int i = 0; i < tableau.getClosedBranches().size(); i++)
	{
		cout << "Branch: " << i << endl;
		Node* tmp = tableau.getClosedBranches().at(i);
		while (tmp != NULL)
		{
			for (int j = 0; j < tmp->getSetFormulae().size(); j++)
				cout << tmp->getSetFormulae().at(j).toString() << endl;
			cout << "----" << endl;
			tmp = tmp->getFather();
		}
	}
}

void printOpenBranches(Tableau& tableau)
{
	cout << "Printing open branches" << endl;
	for (int i = 0; i < tableau.getOpenBranches().size(); i++)
	{
		cout << "Branch: " << i << endl;
		Node* tmp = tableau.getOpenBranches().at(i);
		while (tmp != NULL)
		{
			for (int j = 0; j < tmp->getSetFormulae().size(); j++)
				cout << tmp->getSetFormulae().at(j).toString() << endl;
			cout << "----" << endl;
			tmp = tmp->getFather();
		}
	}
}

void printTRadix(vector<Formula>& KB)
{
	cout << "---Radix Content ---" << endl;
	for (int i = 0; i < KB.size(); i++)
	{
		cout << KB.at(i).toString() << endl;
	}
}

void printTExpanded(Tableau& tableau)
{
	cout << "Content of Expansion:" << endl;
	for (int i = 0; i < tableau.getTableau()->getSetFormulae().size(); i++)
	{
		cout << (tableau.getTableau()->getSetFormulae().at(i).toString()) << "," << tableau.getTableau()->getSetFormulae().at(i).getFulfillness() << endl;
	}
}

//content of VarSet
void printVarSet()
{
	cout << "Check VVL" << endl;
	cout << "Vector 0" << endl;
	printVector(*varSet.getVVLAt(0));
	cout << "Vector 1" << endl;
	printVector(*varSet.getVVLAt(1));
	cout << "Vector 3" << endl;
	printVector(*varSet.getVVLAt(3));

	cout << "Check VQL" << endl;
	cout << "Vector 0" << endl;
	printVector(*varSet.getVQLAt(0));
	cout << "Vector 1" << endl;
	printVector(*varSet.getVQLAt(1));
	cout << "Vector 3" << endl;
	printVector(*varSet.getVQLAt(3));
}

void printEqSet(Tableau& tableau)
{
	cout << "Printing EqSet" << endl;
	for (int i = 0; i < tableau.getEqSet().size(); i++)
	{
		cout << "Branch " << i << endl;
		for (int j = 0; j < tableau.getEqSet().at(i).size(); j++)
		{
			cout << "EqClass " << j << endl;
			for (int k = 0; k < tableau.getEqSet().at(i).at(j).size(); k++)
			{
				cout << tableau.getEqSet().at(i).at(j).at(k)->toString() << endl;
			}
		}
	}

}

/*
   End of printing function
*/


int main()
{
#ifdef debug  
	logFile << "Debug Started" << endl;
#endif // debug
	/*
	Initialization
	*/
	varSet = VariablesSet(3, 100, 20);
	operators = Operators();

	/*
	Inserting Knowledge Base
	*/
	vector<Formula> KB;
	vector<Formula> expKB;
	//	insertFormulaKB("( ($OA V0{l} $CO V0{j} $AO $IN V3{C333})  $AD (  ($OA V0{k} $CO V0{t} $AO $IN V3{C333}) $OR ($OA V0{s} $CO V0{v} $AO $IN V3{C333}) ) )", KB);
	string kbname = "Example/bg.txt";
	readKBFromFile(kbname, KB);

	//print Tableau Radix	
	printTRadix(KB);

	cout << "Expanding Quantifiers in KB" << endl;
	expandKB(KB, expKB);
	Tableau tableau = Tableau(new Node(expKB));

	//Printing result of expansion
	printTExpanded(tableau);

	//Printing content of VVL and VQL
	printVarSet();

	//cout << "Clash before tableau expansion:" << checkNodeClash(tableau.getTableau()->getSetFormulae()) << endl;

	cout << "Expanding Tableau" << endl;
	expandTableau(tableau);

	cout << "Building EqSet" << endl;
	buildEqSet(tableau);

	//print EqSet
	printEqSet(tableau);

	//print open branches
	printOpenBranches(tableau);

	//print closed branches
	printClosedBranches(tableau);


	/* Query Reading*/
	cout << "---" << endl;
	cout << "Reading Query ..." << endl;
	string queryname = "Example/query.txt";
	vector<Formula> querySet;
	vector<string> stringSet=vector<string>(0); 
	readQueryFromFile(queryname, stringSet);
	//vector<Atom*> qAtoms;  
	//cout << stringSet.at(0) << endl;
	performQuerySet(stringSet, querySet, tableau);
	/*
	for (Formula f : querySet)
		cout << f.toString() << endl;
	*/	
	logFile.close();
	return 0;
}






/*
Mapping from 4LQSR formulae in semi-internal formulae
$FA -> ForAll
$IN -> IN
$NI -> NOTIN
$EQ -> =
$QE -> NOT =
$OA -> <
$AO -> >
$AD -> AND
$DA -> NOT AND
$OR -> OR
$RO -> NOT OR
( ) -> ( )
Vi{name} -> X^i_{name} variable
Example of formula
($FA V0{z})(( ( V0{z} $NI V1{C1})$OR ( V0{z} $NI V1{C2}))$AD(( V0{z} $NI V1{C2})$OR (V0{z} $IN V1{C2})))
Is also allowed the following name convention V1{x_b_3}
that stays for X^1_{ {x_{b_3} }
*/



/*
TO DO LIST
Create doc, in particular UML and separe headers from source.
USE A MAP to MAP from string representing operator and integer representing operator.
check brackets; check format of a formula in general as preprocessing and Check if an atom is built correctly.
define special chars from a config file. Setting the size of the special chars and checking for correctness.
allowing change of the $ char from a config file.
creating a quantified variable for a formula does not check if it is yet present
Optimize Atom management and creation
Test Cases
Optimize expandKB
remove recursion
A more efficient expansion function is required.
Ensure that a) Vectors on VariableSet are only-read b) coerence of pointer to elements of such vectors.
Done Remove the existence of two functions createQueryVarFromString and createKBVarFromString by generalizing the use of vectors in VarSet. Same thing with
DONE - Solve PBRule inefficiency
*/
