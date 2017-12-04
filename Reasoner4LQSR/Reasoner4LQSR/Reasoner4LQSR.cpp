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
#include "Reasoner4LQSR.h"
//#include "XMLparser.h"
#include "log.h"

using namespace std;


// VariablesSet varSet = VariablesSet(3, 100);
// Operators operators = Operators();
VariablesSet varSet;
Operators operators;

									/*
									L0 has type 0
									L1 has type 1
									L2 has type 2
									L3 as ObjectProperty has type 3
									L3 as DataProperty has type 4
									*/
									//const int minVarSize = -1;
									//const int maxVarSize = 3;   //size of variable


void InitializeReasoner(int sizeV, int sizeQ, int sizet)
{
	varSet= VariablesSet(sizeV, sizeQ, sizet);
	operators = Operators();
};

//Class of the single Var
/*
 Start Var
*/
Var::Var(string _name, int _type, int _var, int _index)
	{
		setName(_name);
		setType(_type);
		setVarType(_var);
		index = _index;
	};
Var::~Var() {};
	//int isValidType() { return isValidType(type);};
	//int isValidVar() { return isValidVar(var); };
int Var::getType() { return type; };
string Var::getName() { return name; };
int Var::getVarType() { return var; };
int Var::getIndex() { return index; };
void Var::setIndex(int _index) { index = _index; }
int Var::setName(string _name)
	{
		name = _name;
		return type;
	};
void Var::setType(int _type)
	{
		type = _type;
	};

void Var::setVarType(int _var)
	{
		var = _var;
	};

int Var::equal(Var* match)
	{
		if ((getName().compare(match->getName()) == 0) && (getType() == match->getType()) && (getVarType() == match->getVarType()))
			return 0;
		return 1;
	};
int Var::equal(string _name, int _type, int _varType)
	{
		if (getName().compare(_name) == 0 && (getType() == _type) && (getVarType() == _varType))
			return 0;
		return 1;
	}
string Var::toString()
	{
		string out = "V";
		out.append(to_string(getType()));
		out.append("{");
		out.append(getName());
		out.append("}");
		return out;
	};

/*
  End Var
*/




// vector of 4LQSR Variables. Q stays for quantified.
/*
  Start Variable Set
*/
VariablesSet::VariablesSet() {};
VariablesSet::VariablesSet(int maxNVariable, int maxVSize, int maxQSize)
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

vector<Var>* VariablesSet::getAt(vector<vector<Var>>& vec, int level)
	{
		if (level < vec.size())
			return &vec.at(level);
		return NULL;
};

vector<Var>* VariablesSet::getVVLAt(int level)
	{
		if (level < VVL.size())
			return &VVL.at(level);
		return NULL;
	};

vector<Var>* VariablesSet::getVQLAt(int level)
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
int VariablesSet::pushBack(vector<vector <Var>>& vec, int inslevel, string name, int level, int vartype)
	{
		if (vec.at(inslevel).size() < vec.at(inslevel).capacity())
		{
			vec.at(inslevel).push_back(*new Var(name, level, vartype, getSizeAt(vec, inslevel)));
			return 0;
		}
		return 1;
};

int VariablesSet::VVLPushBack(int inslevel, string name, int level, int vartype)
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
};

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

vector<vector <Var>>& VariablesSet::getVQL() { return VQL; };
vector<vector <Var>>& VariablesSet::getVVL() { return VVL; };
Var* VariablesSet::getBack(vector<vector <Var>>& vec, int level) { return &vec.at(level).back(); };
Var* VariablesSet::VVLGetBack(int level) { return &VVL.at(level).back(); };
	//	Var* VQLGetBack(int level) { return &VQL.at(level).back(); }
	//	Var* QVQLGetBack(int level) { return &QVQL.at(level).back(); }
size_t VariablesSet::getSize(vector<vector <Var>>& vec) { return vec.size(); };
	//	size_t VVLGetSize() { return VVL.size(); }
size_t VariablesSet::VQLGetSize() { return VQL.size(); };
	//	size_t QVQLGetSize() { return QVQL.size(); }
size_t VariablesSet::getSizeAt(vector<vector <Var>>& vec, int level) { return vec.at(level).size(); };
size_t VariablesSet::VVLGetSizeAt(int level) { return VVL.at(level).size(); };
size_t VariablesSet::VQLGetSizeAt(int level) { return VQL.at(level).size(); };
	//	size_t QVQLGetSizeAt(int level) { return QVQL.at(level).size(); }

/*
End VariableSet
*/

//Special chars of a formula
/*
Start Operators
*/
Operators::Operators() {};
size_t Operators::getLogOpSize() { return logOp.size(); };
size_t Operators::getSetOpSize() { return setOp.size(); };
size_t Operators::getQuantiOpSize() { return qutOp.size(); };
int Operators::getLogOpValue(string s)
	{
		for (int i = 0; i < getLogOpSize(); i++)
		{
			if (logOp[i].compare(s) == 0)
				return i;
		}
		return -1;
	};

int Operators::getSetOpValue(string s)
	{
		int i = 0;
		for (; i < getSetOpSize(); i++)
		{
			if (setOp[i].compare(s) == 0)
				return i;
		}
		return -1;
	};

int Operators::getQuantiOpValue(string s)
	{
		for (int i = 0; i < getQuantiOpSize(); i++)
		{
			if (qutOp[i].compare(s) == 0)
				return i;
		}
		return -1;
	};

string Operators::getLogOpElement(int index)
	{
		if (index < getLogOpSize())
			return logOp[index];
		return NULL;
	};

string Operators::getSetOpElement(int index)
	{
		if (index < getSetOpSize())
			return setOp[index];
		return NULL;
	};

string Operators::getQuantiOpElement(int index)
	{
		if (index < getQuantiOpSize())
			return qutOp[index];
		return NULL;
	};

	/*
	Return the value of the given string representing a logic operator
	*/
int Operators::checkLogOp(string *s)
	{
		for (const string &op : logOp)
		{
			if (op.compare(*s) == 0)
				return 1;
		}
		return 0;
	}

/**
   End Operators
*/


/**
  Start Atom
*/
Atom::Atom() {};

Atom::Atom(int op, vector<Var*> vec)
	{
		//components.reserve(maxVarSize);
		setAtomOp(op);
		components = vec;
	};

vector<Var*>& Atom::getElements()
	{
		return components;
	};

int Atom::getAtomOp()
	{
		return atomOp;
	};

void Atom::setAtomOp(int value)
	{
		atomOp = value;
	};

Var* Atom::getElementAt(int index)
	{
		if (index < components.size())
			return components.at(index);
		return NULL;
	}

Atom::~Atom() {};

	/*
	Use Carefully. Remember the position of the left/right operand
	*/
void Atom::addElement(Var* element)
	{
		getElements().push_back(element);
	};

int Atom::setElementAt(int index, Var* element)
	{
		if (index < components.size())
		{
			components.at(index) = element;
			return 0;
		}
		return 1;
     };

string Atom::toString()
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

int Atom::containsQVariable()
	{		
		for (Var* var: getElements())
		{
			if (var->getVarType() == 1)
				return 1;
		}
		return 0;
};

int Atom::equals(Atom &atom)
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
/**
End Atom
*/


/**
  Start Formula
*/
Formula::Formula()
	{
		setAtom(NULL);
		setOperand(-1);
		fulfilled = 1;
		setLSubformula(NULL);
		setRSubformula(NULL);
		setPreviousFormula(NULL);
	};

Formula::Formula(Atom *at, int op)
	{
		setAtom(at);
		setOperand(op);
		fulfilled = 1;
		setLSubformula(NULL);
		setRSubformula(NULL);
		setPreviousFormula(NULL);
	};

Formula::Formula(Atom *at, int op, Formula *lf, Formula *rf)
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
Atom* Formula::getAtom() { return atom; };
int Formula::getOperand() { return operand; };
Formula* Formula::getLSubformula() { return lsubformula; };
Formula* Formula::getRSubformula() { return rsubformula; };
Formula* Formula::getPreviousformula() { return pformula; };
void Formula::setOperand(int op) { operand = op; };
void Formula::setAtom(Atom *at) { atom = at; };
void Formula::setLSubformula(Formula *sub) { lsubformula = sub; };
void Formula::setRSubformula(Formula *sub) { rsubformula = sub; };
void Formula::setPreviousFormula(Formula *prev) { pformula = prev; };
void Formula::setFulfillness(int val) { fulfilled = val; };
int Formula::getFulfillness() { return fulfilled; };
Formula::~Formula() {};
string Formula::toString()
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
};

/**
End Formula
*/


/**
  Start Node
*/

int Node::containsFormula(Formula &F) //to be completed and performed
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
};

Node::Node(int size) { setFormula.reserve(size); leftChild = rightChild = father = NULL; };
Node::Node() { leftChild = rightChild = father = NULL; };
Node::Node(vector<Formula>& formula) { setFormula = formula; leftChild = rightChild = father = NULL; };
Node* Node::getLeftChild() { return leftChild; };
Node* Node::getRightChild() { return rightChild; };
Node* Node::getFather() { return father; };
void Node::setRightChild(Node* child) { rightChild = child; child->setFather(this); };
void Node::setLeftChild(Node* child) { leftChild = child; child->setFather(this); };
void Node::setFather(Node* f) { father = f; };
void Node::insertFormula(Formula& f)
	{
		setFormula.push_back(f);
	};
void Node::insertCheckFormula(Formula& f)
	{
		if (this->containsFormula(f) == 1)
			setFormula.push_back(f);
	};
vector<Formula>& Node::getSetFormulae() { return setFormula; };
void Node::setSetFormulae(vector<Formula>& input) { setFormula = input; };
Node::~Node() {};
/**
End Node
*/


/*
Start Tableau
*/
Tableau::Tableau(Node* initial) { radix = initial; };
Tableau::Tableau(int size_radix) { radix = new Node(size_radix); };
Node* Tableau::getTableau() { return radix; };
int Tableau::insertFormula(Formula &f) { radix->insertFormula(f);  return 0; };
vector<Node*>& Tableau::getOpenBranches() { return openBranches; };
vector<Node*>& Tableau::getClosedBranches() { return closedBranches; };
vector < vector < vector <Var*> > >& Tableau::getEqSet() { return EqSet; };
void Tableau::addClosedBranch(Node* node) { getClosedBranches().push_back(node); };
void Tableau::areInEqClass(Var& var1, int& varclass1, int& indx1, Var& var2, int& varclass2, int& indx2, int brindx)
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
};

int Tableau::sameEqClass(Var& var1, Var& var2, int brindx)
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
};

void Tableau::mergeEqClass(int brindx, int indx1, int indx2)
	{
#ifdef debug 
#ifdef eqsetdebug
		logFile << "------- Merging equivalence classes with representative " << getEqSet().at(brindx).at(indx1).at(0)->toString() << " and " << getEqSet().at(brindx).at(indx2).at(0)->toString() << " in branch " << brindx << endl;
#endif
#endif // debug

		getEqSet().at(brindx).at(indx1).insert(getEqSet().at(brindx).at(indx1).end(), getEqSet().at(brindx).at(indx2).begin(), getEqSet().at(brindx).at(indx2).end());
		getEqSet().at(brindx).erase(getEqSet().at(brindx).begin() + indx2);
	};

void Tableau::mergeEqClass(int brindx, int indx1, Var& var)
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

void Tableau::mergeEqClass(int brindx, Var& var, int indx1)
	{
#ifdef debug 
#ifdef eqsetdebug
		logFile << "------- Merging " << var.toString() << " and equivalence class with representative " << getEqSet().at(brindx).at(indx1).at(0)->toString() << " in branch " << brindx << endl;
#endif
#endif // debug
		getEqSet().at(brindx).at(indx1).insert(getEqSet().at(brindx).at(indx1).begin(), &var);

	};
Tableau::~Tableau() {};
/*
  End Tableau
*/




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
		else
		{
		  retrieveVarData(input.substr(0, found), &name, &level);
		  var1=createVarFromString(vec, vec2, &name, &level, new int(0), &startQuantVect.at(level), &typeformula);
          #ifdef debug  
            #ifdef debuginsertf		 
			  logFile << "-----Declaration Found: " << var1->toString() << endl;
              #endif
            #endif // debug
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
				if (!stackFormula.empty())
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
				if (!stackFormula.empty())
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
		case ' ': break; case '\f': break; case '\n': break;  case '\r': break;  case '\t': break; case '\v': break;
		default:	atom.append(string(1, c)); break;
		}
	}	

	
	if (!atom.empty())
	{ 		
		createAtom(vec, vec2, atom, &formula, startQuantVect, typeformula);		
		if (formula != NULL)
			stformula.push(formula);		
	}
    
	if (!stformula.empty())
	{
		*outformula = (stformula.top());
           #ifdef debug  
		         logFile << "-----Final Formula: " << (*outformula)->toString() << endl;
          #endif // debug
				 return 1;
	}
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

Formula* copyFormula(Formula* formula, Formula* father)
{

	if (formula == NULL)
		return NULL;
	Formula* fin = new Formula();
	fin->setOperand(formula->getOperand());
	if (formula->getAtom() == NULL)
		fin->setAtom(NULL);
	else
	{
		Atom* atomf = new Atom();
		copyAtom(formula->getAtom(), atomf);
		fin->setAtom(atomf);
	}
	fin->setPreviousFormula(father);
	fin->setLSubformula(copyFormula(formula->getLSubformula(), fin));
	fin->setRSubformula(copyFormula(formula->getRSubformula(), fin));
	return fin;
}

void convertToCNF(Formula* formula)
{
	vector<Formula*> stack;
	stack.push_back(formula);
#ifdef debug
#ifdef debugcnf
	logFile << "-----Converting formula in CNF:" << endl;
	logFile << "-----" << formula->toString() << endl;
#endif
#endif // debug
	while (!stack.empty())
	{
		Formula* current = stack.back();
		stack.pop_back();
		int lr = -1;
		if (current->getOperand() == 0)
		{
			Formula* fswitch = NULL;
			Formula* base = NULL;
			if (current->getLSubformula()->getOperand() == 1)
			{
				fswitch = current->getLSubformula();
				base = current->getRSubformula();
				lr = 0;
			}
			else if (current->getRSubformula()->getOperand() == 1)
			{
				fswitch = current->getRSubformula();
				base = current->getLSubformula();
				lr = 1;
			}
			if (fswitch != NULL)
			{
				current->setOperand(1);
				fswitch->setOperand(0);
				Formula* move = fswitch->getRSubformula();
				fswitch->setRSubformula(copyFormula(base, fswitch));

				Formula* child = new Formula();
				child->setOperand(0);
				if (lr == 0)
					current->setRSubformula(child);
				else
					current->setLSubformula(child);
				child->setPreviousFormula(current);

				child->setLSubformula(move);
				child->setRSubformula(base);

				move->setPreviousFormula(child);
				base->setPreviousFormula(child);

			}

		}
		if (lr > -1 && current->getPreviousformula() != NULL)
			stack.push_back(current->getPreviousformula());
		else
		{
			if (current->getLSubformula() != NULL)
				stack.push_back(current->getLSubformula());
			if (current->getRSubformula() != NULL)
				stack.push_back(current->getRSubformula());
		}
	}
#ifdef debug
#ifdef debugcnf
	logFile << "-----Resulting CNF formula:" << endl;
	logFile << "-----" << formula->toString() << endl;
#endif
#endif // debug
}

/*
Create an object of type Formula  from the given string representing a formula.
*/
int insertFormulaKB(int keepQ,vector<vector <Var>>& varset1, vector<vector <Var>>& varset2, string formula, Formula** ffinal, int* typeformula)
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
	if (keepQ == 0)
	{
		for (int i = 0; i < varset1.size(); i++)
		 vqlsize.push_back(0); //formulas share quantified variables
	}
	else
	{
		for (int i = 0; i < varset1.size(); i++)
			vqlsize.push_back((int)varset1.at(i).size());
	}
	int res=parseInternalFormula(varset1, varset2, &formula, ffinal, vqlsize, *typeformula);
	//vec.push_back(*ffinal);
#ifdef debug  
	logFile << "---Formula Ended " << endl;
#endif // debug
	//convertToCNF(*ffinal);
	return res;
}

int insertFormulaKB(int keepQ,vector<vector <Var>>& varset, vector<vector <Var>>& varset2, string formula, vector<Formula> &vec, int* typeformula)
{
	Formula* f = NULL;
	int res = insertFormulaKB(keepQ, varset, varset2, formula, &f, typeformula);
	if ( res == 1)
	{
		vec.push_back(*f);
		delete(f);
	}
	return res;
}

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

void renameQVariables(Formula* formula, vector<vector <Var>>& varset1)
{
	vector<int> npos;
	for (int i = 0; i < varset1.size(); i++)
	{
		npos.push_back(varset1.at(i).size());
		
	}
	
	vector<Formula*> stackformula;
	stackformula.push_back(formula);
	while (!stackformula.empty())
	{
		Formula* tmp = stackformula.back();
		stackformula.pop_back();
		if (tmp->getAtom() != NULL)
		{
			Atom* atm = tmp->getAtom();
			for (int i = 0; i < atm->getElements().size(); i++)
			{
				if (atm->getElementAt(i)->getVarType() == 1)
				{
					
					int vecpos = atm->getElementAt(i)->getType(); 
					int position = npos.at(vecpos);					
					int j = position; 
					for (; j < varset1.at(vecpos).size(); j++)
					{
						if ( (atm->getElementAt(i)->equal(&varset1.at(vecpos).at(j))) == 0)
						{							
							atm->setElementAt(i, &varset1.at(vecpos).at(j));
							break;
						}
					}
					if (j == varset1.at(vecpos).size())
					{ 						
						varset1.at(vecpos).push_back(*atm->getElementAt(i));
						varset1.at(vecpos).back().setIndex(j);
						atm->setElementAt(i, &varset1.at(vecpos).back());
					}
				}
			}
		}
		if( tmp->getLSubformula()!= NULL)
		  stackformula.push_back(tmp->getLSubformula());
		if(tmp->getRSubformula()!=NULL)
			stackformula.push_back(tmp->getRSubformula());
	}
}

void moveQuantifier(int qFlag, Formula* formula, vector<vector <Var>>& varset1, vector<Formula>& data)
{
#ifdef debug 
#ifdef debugmvq
	logFile << "---Moving quantifiers in formula: " << endl;
	logFile << "-----"<<formula->toString()<< endl;	
#endif
#endif // debug
	if (formula->getOperand() != 1)
	{
		data.push_back(*formula);
		return;
	}
	vector<Formula*> tmp;
	vector<Formula*> inside;
	tmp.push_back(formula);
	while (!tmp.empty())
	{
		Formula* current = tmp.back();
		tmp.pop_back();
		
		if (current->getOperand() == 1) //AND found
		{
			inside.push_back(current->getLSubformula());
			if (qFlag == 1)
			  renameQVariables(current->getRSubformula(), varset1);
			inside.push_back(current->getRSubformula());
			while (!inside.empty())
			{
				Formula* local = inside.back();
				inside.pop_back();
				local->setPreviousFormula(NULL);				
				tmp.push_back(local);
			}			
		}
		else
		{
			data.push_back(*current);
             #ifdef debug 
             #ifdef debugmvq			     
		         logFile << "-----Formula inserted: " << current->toString() << endl;
            #endif
            #endif // debug
		}
	}
	
	return;
}



void readKBFromStrings(int qflag, vector<string>&names, vector<Formula>& KB)
{	
	cout << "Knowledge Base" << endl;
	int typeformula = 0;
	for(string str: names)
	{
		if ((str.rfind("//", 0) == 0) || str.empty())
			continue;
		cout << str << endl;
		insertFormulaKB( qflag,varSet.getVQL(), varSet.getVVL(), str, KB, &typeformula);
	}
	//Converting to CNF;
	for (int i = 0; i < KB.size(); i++)
	{		
		convertToCNF(&KB.at(i));
	}
	vector<Formula> KBf;
	while(!KB.empty())
	{
	  moveQuantifier(qflag, &KB.back(), varSet.getVQL(), KBf); 		
	  KB.back().setLSubformula(NULL);
	  KB.back().setRSubformula(NULL);
	  KB.back().~Formula();
	  KB.pop_back();
	}
	KB = KBf;
}

void readKBFromFile(int qflag, string &name, vector<Formula>& KB)
{
	std::ifstream file(name);
	std::string str;
	cout << "Reading From Files" << endl;	
	vector<string> lines;
	while (std::getline(file, str))
	{
		lines.push_back(str);
	}
	readKBFromStrings(qflag, lines, KB);
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


/**
Start QueryManager
*/

QueryManager::QueryManager(int _nlevel, int _maxQQSize)
	{
		nlevel = _nlevel;
		maxQQSize = _maxQQSize;
		QVQL.reserve(nlevel);
		QVVL.reserve(nlevel);
		Match = pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>(vector<int>(), vector<vector<vector<pair<Var*, Var*>>>>());
		ynAnswer = vector<int>();
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

void QueryManager::extractLiterals(Formula& f, vector<Atom*> &atoms)
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


vector<vector<Var>>& QueryManager::getQVQL() { return QVQL; };
vector<vector<Var>>& QueryManager::getQVVL() { return QVVL; };
vector<int>& QueryManager::getAnswerSet() { return ynAnswer; };
pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& QueryManager::getMatchSet() { return Match; };
void QueryManager::setMatchSet(pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& input) { Match = input; };
void QueryManager::setAnswerSet(vector<int> input) { ynAnswer = input; };
int QueryManager::checkQueryLiteralMatchInBranch(Node* branch, Atom* query)
	{
		Node* iterator = branch;
		while (iterator != NULL)
		{
			for (Formula formula : iterator->getSetFormulae())
			{
				if (formula.getAtom() != NULL)
				{
					if (formula.getAtom()->equals( *(query))==0)
					{						
						return 1;
					}
				}
			}
			iterator = iterator->getFather();
		}
		return 0;	
};

int QueryManager::checkQueryVariableMatchInBranch(Node* branch, Atom* query, vector<pair<Var*, Var*>>& currentMatch, vector<vector<pair<Var*, Var*>>>& matches)
	{
		int res = 0;
		Node* iterator = branch;
		while (iterator != NULL)
		{
			for (Formula formula : iterator->getSetFormulae())
			{
				if (formula.getAtom() != NULL)
				{
					if (formula.getAtom()->getElements().size() == query->getElements().size() && formula.getAtom()->getAtomOp() == query->getAtomOp())
					{
						int matchN = 0;
						int noq = 0; //check if the query is a literal and there is a match
						vector<pair<Var*, Var*>> temp = vector<pair<Var*, Var*>>();
						for (int varIt = 0; varIt < formula.getAtom()->getElements().size(); varIt++)
						{
							if (query->getElementAt(varIt)->getVarType() == 0 && query->getElementAt(varIt)->equal(formula.getAtom()->getElements().at(varIt)) == 0)
							{
								// cout << query->toString() << "++" << formula.getAtom()->toString() << endl;
								matchN++;								
							}
							else if (query->getElementAt(varIt)->getVarType() == 1)
							{
								// cout << query->toString() << ".." << formula.getAtom()->toString() << endl;
								temp.push_back(pair<Var*, Var*>(query->getElementAt(varIt), formula.getAtom()->getElementAt(varIt)));
								matchN++;
								// cout<<temp.back().first->toString() << " pair " << temp.back().second->toString()<< " " <<endl;
							}
						}
						if (matchN == query->getElements().size())
						{
							//currentMatch.insert(currentMatch.end(), temp.begin(), temp.end());
							vector<pair<Var*, Var*>> c = currentMatch;
							for (pair<Var*, Var*> m : temp)
							{
								c.push_back(m);
							}
							matches.push_back(c);
							res = 1;
						}						
					}
				}
			}
			iterator = iterator->getFather();
		}
		return res;
};
	
Atom QueryManager::applySubstitution(Atom* result, Atom* query, const vector<pair<Var*,Var*>>& matches)
	{				
		copyAtom(query,result);
		//cout << "Q before:" << result->toString() << endl;		
		for (int sigIt = 0; sigIt < matches.size(); sigIt++)
		{
			for (int i = 0; i < query->getElements().size(); i++)
			{
				if (query->getElementAt(i)->equal(matches.at(sigIt).first)==0)
				{

					result->setElementAt(i, matches.at(sigIt).second);
					//cout << "replacing..." << result->getElementAt(i)->toString()<<endl;
				}
			}
		}
		//cout << "Query After: " << result->toString() << endl;
		return *result;
};

void QueryManager::executeQuery(Formula& f, Tableau& tableau, pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& result, int YN, vector<int>& ynAnswer)
	{
		vector<Atom*> qLits;
		formula = f;
		extractLiterals(formula, qLits);
		
		//Print Atoms from Query
		cout << "Literals from query: " << qLits.size() << endl;
		for (Atom* a : qLits)
			cout << a->toString() << endl;
		//---------------------------------
		for (int branchIt=0; branchIt<tableau.getOpenBranches().size();branchIt++)
		{
			int res=0;
			vector<vector<pair<Var*, Var*>>> matchSet(0, vector<pair<Var*,Var*>>(0)); //partial solutions set for the current branch
            int qIter=0; 
			for (; qIter < qLits.size(); qIter++)
			{
				res = 0;
				if (qLits.at(qIter)->containsQVariable() == 0)
					res = checkQueryLiteralMatchInBranch(tableau.getOpenBranches().at(branchIt), qLits.at(qIter));
				else
				{
				  if (matchSet.empty())
				   {
					res = checkQueryVariableMatchInBranch(tableau.getOpenBranches().at(branchIt), qLits.at(qIter), vector<pair<Var*, Var*>>(), matchSet);
				    }
				   else
				    {
					 vector <vector<pair<Var*, Var*>>> tmp(0);
					 for (int solIter = 0; solIter < matchSet.size(); solIter++) //iterate over partial solutions
					   {
						Atom sigq = Atom(-1, vector<Var*>(0));
						applySubstitution(&sigq, qLits.at(qIter), matchSet.at(solIter));
						if (sigq.containsQVariable() == 0)
						{
							if (checkQueryLiteralMatchInBranch(tableau.getOpenBranches().at(branchIt), &sigq) == 1)
								tmp.push_back(matchSet.at(solIter));
						}
						else 
							checkQueryVariableMatchInBranch(tableau.getOpenBranches().at(branchIt), &sigq, matchSet.at(solIter), tmp);
						// apply partial solution to q_i
						//then call checkQueryMatchInbranch wiht sigma(q_i)
					   }
					matchSet = tmp;
					if (matchSet.empty())
						res = 0;
					else res = 1;
				}
			}
				if (res == 0)
					break;
			}				
			if (!matchSet.empty())
			{
				result.first.push_back(branchIt);
				result.second.push_back(matchSet);
			}
			//cout << "Tableau Branch: " << branchIt << ", Answer: " << res << endl;
			if(YN==1)
			 ynAnswer.at(branchIt)=res;
		}
	};

/**
End QueryManager
*/


QueryManager* performQuery(string& str, Formula** formula, Tableau& tableau, int yn)
{
	QueryManager* queryManager = new QueryManager(5, 50);
	int typeformula = 1;
	insertFormulaKB(0,queryManager->getQVQL(), queryManager->getQVVL(), str, formula, &typeformula);	
	
	pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>> result(vector<int>(0), vector<vector<vector<pair<Var*, Var*>>>>(0));
	vector<int> ynanswer;
	if (yn == 0)
		ynanswer = vector<int>(0);
	else
		ynanswer = vector<int>(tableau.getOpenBranches().size());
	queryManager->executeQuery(**formula, tableau, result, yn, ynanswer); 
	queryManager->setMatchSet(result);
	queryManager->setAnswerSet(ynanswer);
	return queryManager;
	/*for (vector<Var> i : queryManager->getQVQL())
		for (Var j : i)
			cout << j.toString() << endl;*/
};

void performQuerySet(vector<QueryManager>& results,vector<string>& strings, vector<Formula>& formulae, Tableau& tableau)
{
#ifdef debug 
#ifdef debugquery
	logFile << "-----Parsing line:" << strings.at(0) << endl;
#endif
#endif // debug	
	for (string s : strings) // Manage multiple query
	{
		Formula *f = NULL;  
		QueryManager* manager=performQuery(s, &f, tableau, 1); 
		formulae.push_back(*f);
		results.push_back(*manager); //Multiple query to be managed
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
Optimize Atom management and creation
Test Cases
Optimize expandKB
remove recursion
A more efficient expansion function is required.
Ensure that a) Vectors on VariableSet are only-read b) coerence of pointer to elements of such vectors.
Done Remove the existence of two functions createQueryVarFromString and createKBVarFromString by generalizing the use of vectors in VarSet. Same thing with
DONE - Solve PBRule inefficiency
*/
