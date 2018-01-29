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
#include "XMLparser.h"
#include "log.h"
#include "test.h"
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

void InitializeReasoner(int sizeV, vector<int>& KBsize)
{	
	varSet = VariablesSet(sizeV, KBsize); 
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
		//nlevel = maxNVariable;
		//capacity = maxVSize;
		VQL.reserve(maxNVariable);  //initialize vectors for variables
		//QVQL.reserve(maxNVariable);  //initialize vectors for variables
		VVL.reserve(maxNVariable);
		for (int i = 0; i <= maxNVariable; i++)
		{
			VQL.push_back(vector<Var>());
			VVL.push_back(vector<Var>());
			//	QVQL.push_back(vector<Var>());
		}
		for (int i = 0; i <= maxNVariable; i++)
		{
			VQL.at(i).reserve(maxQSize);
			VVL.at(i).reserve(maxVSize);
			//	QVQL.at(i).reserve(maxQQSize);
		}
	};

VariablesSet::VariablesSet(int maxNVariable, vector<int>& KBsize)
{
	//nlevel = maxNVariable;
	//capacity = maxVSize;
	VQL.reserve(maxNVariable);  //initialize vectors for variables
						  //QVQL.reserve(nlevel);  //initialize vectors for variables
	VVL.reserve(maxNVariable);

	for (int i = 0; i <= maxNVariable; i++)
	{
		VQL.push_back(vector<Var>());
		VVL.push_back(vector<Var>());
		//	QVQL.push_back(vector<Var>());
	}
	for (int i = 0; i <= maxNVariable; i++)
	{		
		VVL.at(i).reserve(KBsize.at(i));	
	}

	for (int i = 0; i <= maxNVariable; i++)
	{
		VQL.at(i).reserve(KBsize.at(1+maxNVariable+i)); 
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
  Start Literal
*/
Literal::Literal() {};

Literal::Literal(int op, vector<Var*> vec)
	{
		//components.reserve(maxVarSize);
		setLiteralOp(op);
		components = vec;
	};

vector<Var*>& Literal::getElements()
	{
		return components;
	};

int Literal::getLiteralOp()
	{
		return literalOp;
	};

void Literal::setLiteralOp(int value)
	{
		literalOp = value;
	};

Var* Literal::getElementAt(int index)
	{
		if (index < components.size())
			return components.at(index);
		return NULL;
	}

Literal::~Literal() {};

	/*
	Use Carefully. Remember the position of the left/right operand
	*/
void Literal::addElement(Var* element)
	{
		getElements().push_back(element);
	};

int Literal::setElementAt(int index, Var* element)
	{
		if (index < components.size())
		{
			components.at(index) = element;
			return 0;
		}
		return 1;
     };

string Literal::toString()
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
		out.append(operators.getSetOpElement(getLiteralOp()));
		out.append(" ");
		out.append(getElementAt(0)->toString());
		out.append(")");
		return out;
	};

int Literal::containsQVariable()
	{		
		for (Var* var: getElements())
		{
			if (var->getVarType() == 1)
				return 1;
		}
		return 0;
};

int Literal::equals(Literal &atom)
	{
		if ((this->getElements().size() == atom.getElements().size()) && (this->getLiteralOp() == atom.getLiteralOp()))
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
End Literal
*/


/**
  Start Formula
*/
Formula::Formula()
	{
		setLiteral(NULL);
		setOperand(-1);
		fulfilled = 1;
		setLSubformula(NULL);
		setRSubformula(NULL);
		setPreviousFormula(NULL);
	};

Formula::Formula(Literal *at, int op)
	{
		setLiteral(at);
		setOperand(op);
		fulfilled = 1;
		setLSubformula(NULL);
		setRSubformula(NULL);
		setPreviousFormula(NULL);
	};

Formula::Formula(Literal *at, int op, Formula *lf, Formula *rf)
	{
		setLiteral(at);
		setOperand(op);
		lsubformula = lf;
		rsubformula = rf;
		fulfilled = 1;
		(*lsubformula).setPreviousFormula(this);
		(*rsubformula).setPreviousFormula(this);
	};

// vector<Var*>* getQuantifiedSet() { return &quantified; };
Literal* Formula::getLiteral() { return atom; };
int Formula::getOperand() { return operand; };
Formula* Formula::getLSubformula() { return lsubformula; };
Formula* Formula::getRSubformula() { return rsubformula; };
Formula* Formula::getPreviousformula() { return pformula; };
void Formula::setOperand(int op) { operand = op; };
void Formula::setLiteral(Literal *at) { atom = at; };
void Formula::setLSubformula(Formula *sub) { lsubformula = sub; };
void Formula::setRSubformula(Formula *sub) { rsubformula = sub; };
void Formula::setPreviousFormula(Formula *prev) { pformula = prev; };
void Formula::setFulfillness(int val) { fulfilled = val; };
int Formula::getFulfillness() { return fulfilled; };
Formula::~Formula() { delete atom;  lsubformula = NULL; rsubformula = NULL; pformula = NULL; };
void Formula::toStringQVar(vector<Var*>& qu)
{
 if(getLiteral()!=NULL)
	for (int i = 0; i < getLiteral()->getElements().size(); i++)
	{
		int j = 0;
		for (; j < qu.size(); j++)
		{
			if (getLiteral()->getElementAt(i)->getVarType() == 1 && qu.at(j)->getVarType() == 1)
				if (!getLiteral()->getElementAt(i)->getName().compare(qu.at(j)->getName()))
					break;
		}

		if (j >= qu.size() && getLiteral()->getElementAt(i)->getVarType() == 1)
			qu.push_back(getLiteral()->getElementAt(i));
	}
 if (getLSubformula() != NULL)
	 getLSubformula()->toStringQVar(qu);
 if (getRSubformula() != NULL)
	 getRSubformula()->toStringQVar(qu);
};

string Formula::toString()
{
	vector<Var*> q;
	toStringQVar(q);
	string r=toStringNoQVar();
	string quanti = "";
	for (Var* v : q)
	{
		quanti.append("(");
		quanti.append("$FA ");
		quanti.append(v->toString());
		quanti.append(")");
	}	
	quanti.append(r);
	return quanti;
}
string Formula::toStringNoQVar()
	{
		if (getLiteral() != NULL)
		{		
			return (getLiteral()->toString().append(" "));
		}
		else if (getOperand()==4 && getRSubformula() != NULL)
		{
			string ret=("( ");
			ret.append(operators.getLogOpElement(getOperand()));			
			ret.append(getRSubformula()->toStringNoQVar());
			ret.append(")");
			return ret;
		}
		else if (getOperand() > -1 && getLSubformula() != NULL && getRSubformula() != NULL)
		{			
			string ret = "( ";			
            ret.append(getLSubformula()->toStringNoQVar());
			ret.append(" ");						
			ret.append(operators.getLogOpElement(getOperand()));
			ret.append(" ");
			ret.append(getRSubformula()->toStringNoQVar());
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

int Node::containsFormula(Formula* F) //to be completed and performed
	{
		for (int i = 0; i < this->getSetFormulae().size(); i++)
		{
			Literal* locAt = this->getSetFormulae().at(i)->getLiteral();
			if (locAt != NULL && F->getLiteral() != NULL)
			{
				if (locAt->equals(*(F->getLiteral())) == 0)
				{
					return 0;
				}
			}

		}
		return 1;
};

Node::Node(int size) { setFormula.reserve(size); leftChild = rightChild = father = NULL; };
Node::Node() { leftChild = rightChild = father = NULL; };
Node::Node(vector<Formula*> formula) { setFormula = formula; leftChild = rightChild = father = NULL; };
Node* Node::getLeftChild() { return leftChild; };
Node* Node::getRightChild() { return rightChild; };
Node* Node::getFather() { return father; };
void Node::setRightChild(Node* child) { rightChild = child; child->setFather(this); };
void Node::setLeftChild(Node* child) { leftChild = child; child->setFather(this); };
void Node::setFather(Node* f) { father = f; };
void Node::insertFormula(Formula* f)
	{
		setFormula.push_back(f);
	};
void Node::insertCheckFormula(Formula* f)
	{
		if (this->containsFormula(f) == 1)
			setFormula.push_back(f);
	};
vector<Formula*> Node::getSetFormulae() { return setFormula; };
void Node::setSetFormulae(vector<Formula*> input) { setFormula = input; };
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
int Tableau::insertFormula(Formula* f) { radix->insertFormula(f);  return 0; };
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
Create an Literal from the given string
*/
int createLiteral(vector<vector <Var>>& vec, vector<vector <Var>>& vec2, string input, Formula **formula, vector<int>& startQuantVect, int typeformula)
{	
#ifdef debug  
#ifdef debuginsertf
	logFile << "-----Computing Literal: " << input << endl;
#endif
#endif // debug
	Var* var1;
	Var* var2;
	Var* var3;
	string name = string();
	int level = -1;
	Literal* atom = NULL;
	if (input[0] == '$') //case pair or quantifier
	{
		string head = input.substr(0, 3); 	
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
			atom = new Literal(op, { var3, var1, var2 });
			//Literal* atom = new Literal(0, { new Var(name,level,0), new Var("b1",0,0), new Var("c1",0,0) });
			*formula = (new Formula(atom, -1));						
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
			atom = new Literal(op, { var2, var1 });
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
		logFile << "-----Literal created: " << atom->toString() << endl;
#endif
#endif // debug
	return 0;
}

/*
Parse a string representing an internal formula and return the corresponding internal formula.
Return 1 if a formula is created, 0 in case of declarations.
*/
int parseInternalFormula(vector<vector <Var>>& vec, vector<vector <Var>>& vec2, const string *inputformula, Formula **outformula, vector<int>& startQuantVect, int typeformula)
{

#ifdef debug  
	logFile << "-----Analizyng input formula" << endl;
#endif // debug
	stack<string> stackFormula;
	stack<Formula*> stformula; //tracking subformulae
//	stackFormula.push(*inputformula);
//	string top = stackFormula.top();
	string top = *inputformula;
//	stackFormula.pop();
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
				createLiteral(vec, vec2, atom, &formula, startQuantVect, typeformula);				
				if (formula != NULL) // creation of the formula 
				{						
					stformula.push(formula); 
				}				
				atom.clear();
				
			}
			else if(stformula.size()>1)       //if(stformula.size()>1)
			{				
				Formula *rightf = stformula.top(); 
				stformula.pop();					
				Formula *centerf = stformula.top(); 
				stformula.pop();			
				centerf->setRSubformula(rightf);
				rightf->setPreviousFormula(centerf);

				if (centerf->getLiteral()==NULL && centerf->getOperand()==4) //$NG has one branch
				{					
					centerf->setLSubformula(NULL); 
				}
				else
				{
					Formula *leftf = stformula.top(); 
					stformula.pop();
					centerf->setLSubformula(leftf);
					leftf->setPreviousFormula(centerf);
				}
				
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
				int isNg = operators.getLogOpValue(operand);
				stformula.push(new Formula(NULL, isNg));
				
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
		createLiteral(vec, vec2, atom, &formula, startQuantVect, typeformula);		
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

void copyLiteral(Literal* source, Literal* dest)
{
	//Literal(int op, vector<Var*> vec)
	dest->setLiteralOp(source->getLiteralOp());
	for (Var* var : source->getElements())
		dest->addElement(var);
}

Literal* copyLiteral(Literal* atom, const string *qvar, Var* dest)
{
	Literal* fin = new Literal();
	fin->setLiteralOp(atom->getLiteralOp());
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
	if (formula->getLiteral() == NULL)
		fin->setLiteral(NULL);
	else
	{
		fin->setLiteral(copyLiteral(formula->getLiteral(), qvar, dest));
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
	if (formula->getLiteral() == NULL)
		fin->setLiteral(NULL);
	else
	{
		Literal* atomf = new Literal();
		copyLiteral(formula->getLiteral(), atomf);
		fin->setLiteral(atomf);
	}
	fin->setPreviousFormula(father);
	fin->setLSubformula(copyFormula(formula->getLSubformula(), fin));
	fin->setRSubformula(copyFormula(formula->getRSubformula(), fin));
	return fin;
}

Formula* convertFormulaToCNF(Formula* formula)
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
		else {
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
	return formula;
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

//if insertFormulaKB returns 1, the parsed formula should be inserted in KB, otherwise is a declaration
int insertFormulaKB(int keepQ,vector<vector <Var>>& varset, vector<vector <Var>>& varset2, string formula, vector<Formula*> &vec, int* typeformula)
{
	Formula* f = NULL;
	int res = insertFormulaKB(keepQ, varset, varset2, formula, &f, typeformula);
	if ( res == 1)	
	  vec.push_back(f);	
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
			if (f->getLiteral() != NULL)
			{
				for (Var* var : (f->getLiteral()->getElements()))
				{
					if (var->getVarType() == 1)
					{
						s = (var->getName());
						return var->getType();
					}
				}
			}
			q.push_back(f->getLSubformula());
			q.push_back(f->getRSubformula());
		}
	}
	return -1;
}

int instantiateFormula(Formula* f, vector<Formula*> &destination)
{

#ifdef test
    counterFormula++;
	cout << "Formula Num" << counterFormula << endl;
	testFile << counterFormula << endl;
#endif // couterformula

#ifdef test
 counterFormulaIn = 0;
 testFile << f->toString() << endl;
#endif // counterFormulaIn

	
	
	
	


#ifdef debug  
	logFile << "------- Expanding Formula: " << f->toString() << endl;
#endif // debug
	string s;
	vector<Formula*> tmp;
	tmp.push_back(f);
	while (!tmp.empty())
	{		

		Formula* top = tmp.back(); 		
		tmp.pop_back();
		int type = containsQVar(top, s);
		if (type>-1)
		{       
			for (int i = 0; i < varSet.getVVLAt(type)->size(); i++)
			{				
				tmp.push_back((copyFormula(top, NULL, &s, &varSet.getVVLAt(type)->at(i))));					
			}
		}
		else
		{
#ifdef debug 
#ifdef debugexpand
			logFile << "------- Expanded Formula: " << top->toString() << endl;
#endif
#endif // debug	
			
#ifdef test
counterTest++;
counterFormulaIn++;
#endif // counterFormulaIn

			
			//cout<<"------- Expanded Formula: " << counterTest << endl;

			if (top->getLiteral() != NULL)
				top->setFulfillness(0);
			destination.push_back(top); 
		}
	}

  #ifdef test
	testFile << "In Formula: " << counterFormulaIn << ". Total:" << counterTest << endl;
	cout << "------- In Formula: " << counterFormulaIn<<". Total:"<<counterTest << endl;
#endif // counterFormulaIn

	return 0;
}



int expandKB(const vector<Formula*> &inpf, vector <Formula*> &out)
{
#ifdef debug  
	logFile << "--- Applying Expansion Rule" << endl;
#endif // debug

	if (varSet.getVVLAt(0)->size() == 0) //no individual;
	{ 		
		varSet.getVVLAt(0)->push_back(*new Var("TOPIND", 0, 0,0));		
		/*for (Formula* f : inpf)
		{
			out.push_back(f);
		}
		return 0;*/
	}


	int or = operators.getLogOpValue("$OR");
	vector <Formula*> tmp;
	for (int i = 0; i < inpf.size(); i++)
		tmp.push_back(inpf.at(i));

	
	while (!tmp.empty())
	{
		Formula* f = tmp.back();
#ifdef debug  
#ifdef debugexpand
		logFile << "----- Computing Formula: " << f->toString() << endl;
#endif
#endif // debug
		tmp.pop_back();
		if (f->getLiteral() != NULL || f->getOperand()== or)
		{			
			instantiateFormula(f, out); 
		}	
		/*else if (f.getOperand() == 4)
		{			
			f.getRSubformula()->setPreviousFormula(NULL);
#ifdef debug  
#ifdef debugexpand			
			logFile << "----- Obtaining Formula: " << f.getRSubformula()->toString() << endl;
#endif
#endif // debug			
			tmp.push_back(*(f.getRSubformula()));
		}*/
		/*else
		{
			
			f->getLSubformula()->setPreviousFormula(NULL);
			f->getRSubformula()->setPreviousFormula(NULL);
#ifdef debug  
#ifdef debugexpand
			logFile << "----- Obtaining Formula: " << f.getLSubformula()->toString() << endl;
			logFile << "----- Obtaining Formula: " << f.getRSubformula()->toString() << endl;
#endif
#endif // debug
			tmp.push_back(*(f.getLSubformula()));
			tmp.push_back(*(f.getRSubformula()));

			//cout << (f.getOperand()) << endl;
		}*/
	}
#ifdef debug  
	logFile << "--- End Expansion Rule" << endl;
#endif // debug
	return 0;
}

int checkLiteralOpClash(int op1, int op2)
{
	if (abs(op1 - op2) == 2)
		return 0;
	return 1;
}

int checkLiteralClash(Literal &atom1, Literal &atom2)
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
			if (atom1.getElementAt(i)->equal(atom2.getElementAt(i)) != 0)
				return 1;
		}
		return checkLiteralOpClash(atom1.getLiteralOp(), atom2.getLiteralOp());
	}
	return 1;
}

int checkLiteralClash(Literal &atom) //0 for clash 1 for no-clash
{
#ifdef debug
#ifdef debugclash
	logFile << "-----Checking for Clash: " << atom.toString() << endl;
#endif
#endif // debug
	if (atom.getElements().size() == 2 && atom.getLiteralOp() == 3 && atom.getElementAt(0)->equal(atom.getElementAt(1)) == 0)
		return 0;
	return 1;
}

int checkLiteralTautology(Literal &atom) //0 for tautology 1 for no-tautology
{
#ifdef debug
#ifdef debugclash
	logFile << "-----Checking for Tautology: " << atom.toString() << endl;
#endif
#endif // debug
	if (atom.getElements().size() == 2 && atom.getLiteralOp() == 1 && atom.getElementAt(0)->equal(atom.getElementAt(1)) == 0)
		return 0;
	return 1;
}

int checkLiteralsClash(vector<Literal*> &vec)
{

	for (int i = 0; i < vec.size() - 1; i++)
	{
#ifdef debug 
#ifdef debugclash
		logFile << "-----Checking for Clash: " << vec.at(i)->toString() << " and " << vec.at(i + 1)->toString() << endl;
#endif // debug
#endif
		if (checkLiteralClash(*vec.at(i), *vec.at(i + 1)))
			return 0;
	}
	return 1;
}


/*
Check for Clash between the given candidate and a vector of formulae starting from the given index
*/
int checkVectorClash(Literal* candidate, vector<Formula*> &formset, int start)
{
#ifdef debug  
	logFile << "---Checking for Clash in Vector of Formulae" << endl;
#endif // debug
	
	if (candidate != NULL) //atomic formula
	{
		if (checkLiteralClash(*candidate) == 0)  // type a != a
		{
#ifdef debug  
			logFile << "-----Clash at: " << candidate->toString() << endl;
#endif // debug
			return 0;
		}
		for (int j = start; j < formset.size(); j++)
		{
			if (formset.at(j)->getLiteral() != NULL)
			{
				if (checkLiteralClash(*formset.at(j)->getLiteral()) == 0)
				{
#ifdef debug  
					logFile << "-----Clash at: " << formset.at(j)->getLiteral()->toString() << endl;
#endif // debug
					return 0;
				}
				if (checkLiteralClash(*candidate, *formset.at(j)->getLiteral()) == 0)
				{
#ifdef debug  
					logFile << "-----Clash at: " << candidate->toString() << "," << formset.at(j)->getLiteral()->toString() << endl;
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
int checkNodeClash(vector<Formula*> &formset)
{
	if (formset.size() < 2)
	{
		return 1;
	}
	for (int i = 0; i < formset.size() - 1; i++)
	{		
		if (checkVectorClash(formset.at(i)->getLiteral(), formset, i + 1) == 0)
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
	vector<Formula*>local = node->getSetFormulae();
	Node* iterator = node->getFather();
	while (iterator != NULL)
	{
		for (int i = 0; i < local.size(); i++)
		{
			Literal* at = local.at(i)->getLiteral();
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

int getLiteralSet(Formula* f, vector<Literal*> &outf)
{
	if (f->getLiteral() != NULL)
		return -1;
	stack <Formula*> st;
	st.push(f->getRSubformula());
	st.push(f->getLSubformula());
	while (!st.empty())
	{
		Formula* tmp = st.top();
		if (tmp->getLiteral() != NULL &&checkLiteralClash(*tmp->getLiteral()) != 0 && checkLiteralTautology(*tmp->getLiteral()) != 0)
		{
		  int stop = 0;
				int i = 0;
				for (; i < outf.size() && stop == 0; i++)
				{
					if (outf.at(i)->equals(*tmp->getLiteral()) == 0)
						stop = 1;
					else if (checkLiteralClash(*outf.at(i), *tmp->getLiteral()) == 0)	// case A V \negA						
						stop = 2; 					
				}
				if (stop == 0)
					outf.push_back(tmp->getLiteral());//There should be only OR because of CNF formula
				else if (stop == 2) // remove the element
				{

					outf.at(i - 1) = outf.back(); 
					outf.pop_back();
				}						
		}                      	
		st.pop();
		if (tmp->getRSubformula() != NULL)
			st.push(tmp->getRSubformula());
		if (tmp->getLSubformula() != NULL)
			st.push(tmp->getLSubformula());
	}
	return 0;
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


Literal* negatedLiteral(Literal *input)
{
	Literal* out = copyLiteral(input, NULL, NULL);
	int neg = (out->getLiteralOp() >= 2 ? -2 : 2);
	out->setLiteralOp(out->getLiteralOp() + neg);
	return out;
}


int checkBranchClash(Literal* atom, Node* node)
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

void ERule(Literal* atom, Node* node)
{
#ifdef debug  
	logFile << "---Applying E-RULE" << endl;
#endif // debug		

#ifdef debug  
#ifdef debugexpand
	logFile << "----- Computing Literal from E-Rule: " << copyLiteral(atom, NULL, NULL)->toString() << endl;
#endif
#endif // debug

	Node* tmp = node;
	vector<Node*> newNodeSet;
	Literal* neg = negatedLiteral(atom);
	//	if (checkBranchClash(neg, tmp) == 1)		  //solve thi inefficiency
	node->insertFormula((new Formula(copyLiteral(atom, NULL, NULL), -1)));
}

void PBRule(vector<Literal*> atoms, Node* node, vector<Node*> &nodeSet)
{
#ifdef debug  
	logFile << "---Applying PB-RULE" << endl;
#endif // debug		

#ifdef debug  
#ifdef debugexpand
	logFile << "----- Computing Literal from PB-Rule. " << endl;
#endif
#endif // debug
	Node* tmp = node;
	vector<Node*> newNodeSet;
	/*	for (int i = 0; i < atoms.size(); i++)           //solve this inefficiency. Looking for positive in the branch.
		{
			Literal* neg = negatedLiteral(atoms.at(i));
			if (checkBranchClash(neg, tmp) == 0)
			{
				newNodeSet.push_back(tmp);
				nodeSet.insert(nodeSet.end(), newNodeSet.begin(), newNodeSet.end());
				return;
			}
		} //15/01/18 inserted 
	tmp = node;*/
	for (int i = 0; i < atoms.size() - 1; i++)
	{
#ifdef debug  
#ifdef debugexpand
		logFile << "------ Computing Literal from PB-Rule. " << copyLiteral(atoms.at(i), NULL, NULL)->toString() << endl;
		logFile << "------ Computing Literal from PB-Rule. " << (negatedLiteral(atoms.at(i)))->toString() << endl;
#endif
#endif // debug
		Literal* neg = negatedLiteral(atoms.at(i));
		tmp->setLeftChild(new Node(vector<Formula*> {(new Formula(copyLiteral(atoms.at(i), NULL, NULL), -1))}));
		newNodeSet.push_back(tmp->getLeftChild());
		tmp->setRightChild(new Node(vector<Formula*> {(new Formula(negatedLiteral(atoms.at(i)), -1))}));
		tmp = tmp->getRightChild();
	}
#ifdef debug  
#ifdef debugexpand
	logFile << "------ Computing Literal from PB-Rule. " << (copyLiteral(atoms.at(atoms.size() - 1), NULL, NULL))->toString() << endl;
#endif
#endif // debug
	tmp->insertFormula((new Formula(copyLiteral(atoms.at(atoms.size() - 1), NULL, NULL), -1)));
	newNodeSet.push_back(tmp);
	nodeSet.insert(nodeSet.end(), newNodeSet.begin(), newNodeSet.end());
}




int checkLiterals(Literal* atom, Node* node)
{
	Node* iterator = node;
	while (iterator != NULL)
	{
		vector<Formula*> vec = iterator->getSetFormulae();
		for (int i = 0; i < vec.size(); i++)
		{
			if (vec.at(i)->getLiteral() != NULL)
			{
				if ((checkLiteralClash(*atom, *(vec.at(i)->getLiteral())) == 0))
					return 0;
				if (atom->equals(*(vec.at(i)->getLiteral())) == 0)
					return 2;
			}
		}
		iterator = iterator->getFather();
	}
	return 1;
}


void chooseRule(Tableau &T, vector<Node*> &nodeSet, Formula* f)
{
	vector<Node*> newNodeSet;
	for (int b = 0; b < nodeSet.size(); b++)
	{		
		vector<Literal*> atoms;
		vector<Literal*> atomset;
		int val = getLiteralSet(f, atomset); 
		if (val == 0 && atomset.size() == 0)
		{
		  newNodeSet.push_back(nodeSet.at(b));
          continue;
		}
		
		/*	if (checkLiteralsClash(atomset))
			{
				newNodeSet.push_back(nodeSet.at(b));
				//T.getClosedBranches().push_back(nodeSet.at(b));
				break;
			}*/
		int check = 0;
		for (int j = 0; j < atomset.size(); j++)
		{
			check = checkLiterals(atomset.at(j), nodeSet.at(b));
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
	
	if ( (checkNodeClash(T.getTableau()->getSetFormulae())==0) )
	{		
		T.getClosedBranches().push_back(T.getTableau()); 
		return;
	}
	vector<Node*> nonComBranches = vector<Node*>();
	nonComBranches.push_back(T.getTableau()); //initially only the root node
	vector<Formula*> fset = T.getTableau()->getSetFormulae();
	
	for (int i = 0; i < fset.size(); i++)
	{		
		if (fset.at(i)->getLiteral() == NULL)  //OR found
		{
#ifdef debug  
#ifdef debugexpand
			logFile << "------Fulfilling formula: " << fset.at(i)->toString() << endl;
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
	vector<Literal*> atomset;
	getLiteralSet(T.getTableau()->getSetFormulae().at(i), atomset);
	if (checkLiteralsClash(atomset))
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
				Formula* localf = node->getSetFormulae().at(fcount);
				if (localf->getLiteral() != NULL && localf->getLiteral()->getLiteralOp() == operators.getSetOpValue("$EQ")) //find equivalences
					if (localf->getLiteral()->getElementAt(0)->equal(localf->getLiteral()->getElementAt(1)) == 1)  // ignore the case a=a
					{
#ifdef debug 
#ifdef eqsetdebug
						logFile << "----- Computing equivalence classes for " << localf->getLiteral()->toString() << " in branch " << i << endl;
#endif
#endif // debug

						Var* var1 = localf->getLiteral()->getElements().at(0);
						Var* var2 = localf->getLiteral()->getElements().at(1);
						insertVarsEqClass(*var1, *var2, tab, i);
					}
			}
			node = node->getFather();
		}
	}
}


int checkLiteralClashEqSet(Literal &atom1, Literal &atom2, Tableau& t, int& brindx)
{
#ifdef debug 
#ifdef debugclash
	logFile << "-----Checking for Clash: " << atom1.toString() << " and " << atom2.toString() << " with Equivalence Classes" << endl;
#endif // debug
#endif


	if (atom1.getElements().size() == atom2.getElements().size())
	{
		if (checkLiteralOpClash(atom1.getLiteralOp(), atom2.getLiteralOp()) == 1)
			return 1;

		if (atom1.getLiteralOp() == 3 || atom1.getLiteralOp() == 1) // case equivalence		
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



int checkLiteralClashEqSet(Literal &atom, Tableau& T, int& brindx) //0 for clash 1 for no-clash
{
#ifdef debug
#ifdef debugclash
	logFile << "-----Checking for Clash: " << atom.toString() << " in EqSet" << endl;
#endif
#endif // debug
	if (atom.getElements().size() == 2 && atom.getLiteralOp() == 3)
	{		
		return (T.sameEqClass(*(atom.getElementAt(0)), *(atom.getElementAt(1)), brindx));
	}
	return 1;
}

int checkNodeClashEqSet(Literal& atom, Node& node, Tableau& T, int& brind)
{
	Node* tmp = &node;
	while (tmp != NULL)
	{
		for (int j = 0; j < tmp->getSetFormulae().size(); j++)
		{
			if (tmp->getSetFormulae().at(j)->getLiteral() != NULL)
			{
				if (checkLiteralClashEqSet(atom, *(tmp->getSetFormulae().at(j)->getLiteral()), T, brind) == 0)
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
			if (currentNode->getSetFormulae().at(j)->getLiteral() != NULL)
			{

				if (checkLiteralClashEqSet(*(currentNode->getSetFormulae().at(j)->getLiteral()), T, i) == 1 &&
					checkLiteralClashEqSet(*(currentNode->getSetFormulae().at(j)->getLiteral()), *(currentNode->getSetFormulae().at(j + 1)->getLiteral()), T, i) == 1)
				{
					tobreak = checkNodeClashEqSet(*(currentNode->getSetFormulae().at(j)->getLiteral()), *(currentNode->getFather()), T, i);
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
		if (tmp->getLiteral() != NULL)
		{
			Literal* atm = tmp->getLiteral();
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

void moveQuantifier(int qFlag, Formula* formula, vector<vector <Var>>& varset1, vector<Formula*>& data)
{
#ifdef debug 
#ifdef debugmvq
	logFile << "---Moving quantifiers in formula: " << endl;
	logFile << "-----"<<formula->toString()<< endl;	
#endif
#endif // debug
	if (formula->getOperand() != 1)
	{
		data.push_back(formula);
		return;
	}
	vector<Formula*> tmp;	
	tmp.push_back(formula);
	while (!tmp.empty())
	{
		
	 Formula* current = tmp.back(); 
	  tmp.pop_back();	 
	  if (current->getOperand() == 1) //AND found
		{
			Formula* fL = current->getLSubformula();
			Formula* fR = current->getRSubformula();
			fL->setPreviousFormula(NULL);
			fR->setPreviousFormula(NULL);
			
			delete(current);
			tmp.push_back(fL);
			if (qFlag == 1)
			  renameQVariables(fR, varset1);
			tmp.push_back(fR);	
			
		}
		else 
		{
			data.push_back(current);
             #ifdef debug 
             #ifdef debugmvq			     
		         logFile << "-----Formula inserted: " << current->toString() << endl;
            #endif
            #endif // debug
				 
		}
	}	
}



void convertKBToNNF(vector<Formula*>& KB, vector<Formula*>& KBnorm)
{
#ifdef debug
#ifdef debugnorm
	logFile << "-----Converting KB to NNF-----" << endl;
#endif
#endif // debug
  for (int i = 0; i < KB.size(); i++)
	{		
		Formula* newf=convertFormulaToNNF(KB.at(i)); 
		KBnorm.push_back(newf);
	}
#ifdef debug
#ifdef debugnorm
  logFile << "-----End converting KB to NNF-----" << endl;
#endif
#endif // debug
}

void convertKBToCNF(vector<Formula*>& KB, vector<Formula*>& KBcnf)
{
#ifdef debug
#ifdef debugcnf
	logFile << "-----Converting KB to CNF-----" << endl;	
#endif
#endif // debug

	for (int i = 0; i < KB.size(); i++)
	{
	  Formula* newf=convertFormulaToCNF(KB.at(i));
	  KBcnf.push_back(newf);
	}
#ifdef debug
#ifdef debugcnf
	logFile << "-----End converting KB to CNF:" << endl;
#endif
#endif // debug
}

void moveQuantifierKB(int qflag, vector<Formula*>& KB, vector<Formula*>& KBout)
{	
	for(int i=0; i<KB.size();i++)
	{		
		moveQuantifier(qflag, KB.at(i) , varSet.getVQL(), KBout);		
	}
}

void readKBFromStrings(int qflag, vector<string>&names, vector<Formula*>& KB)
{	
	cout << "Knowledge Base" << endl;
	int typeformula = 0;
	for(string str: names)
	{
		if ((str.rfind("//", 0) == 0) || str.empty())
			continue;			
		insertFormulaKB(qflag,varSet.getVQL(), varSet.getVVL(), str, KB, &typeformula);
	}	
}

void readKBFromFile(int qflag, string &name, vector<Formula*>& KB)
{
	std::ifstream file(name);
	std::string str;		
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

void QueryManager::extractLiterals(Formula& f, vector<Literal*> &atoms)
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
				if (back->getLiteral() != NULL)
				{
					atoms.push_back(back->getLiteral());
#ifdef debug 
#ifdef debugquery
					logFile << "-----Literal computed:" << atoms.back()->toString() << endl;
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
int QueryManager::checkQueryLiteralMatchInBranch(Node* branch, Literal* query)
	{
		Node* iterator = branch;
		while (iterator != NULL)
		{
			for (Formula* formula : iterator->getSetFormulae())
			{
				if (formula->getLiteral() != NULL)
				{
					if (formula->getLiteral()->equals( *(query))==0)
					{						
						return 1;
					}
				}
			}
			iterator = iterator->getFather();
		}
		return 0;	
};

int QueryManager::checkQueryVariableMatchInBranch(Node* branch, Literal* query, vector<pair<Var*, Var*>>& currentMatch, vector<vector<pair<Var*, Var*>>>& matches)
	{
		int res = 0;
		Node* iterator = branch;
		while (iterator != NULL)
		{
			for (Formula* formula : iterator->getSetFormulae())
			{
				if (formula->getLiteral() != NULL)
				{
					if (formula->getLiteral()->getElements().size() == query->getElements().size() && formula->getLiteral()->getLiteralOp() == query->getLiteralOp())
					{
						int matchN = 0;
						int noq = 0; //check if the query is a literal and there is a match
						vector<pair<Var*, Var*>> temp = vector<pair<Var*, Var*>>();
						for (int varIt = 0; varIt < formula->getLiteral()->getElements().size(); varIt++)
						{
							if (query->getElementAt(varIt)->getVarType() == 0 && query->getElementAt(varIt)->equal(formula->getLiteral()->getElements().at(varIt)) == 0)
							{
								// cout << query->toString() << "++" << formula.getLiteral()->toString() << endl;
								matchN++;								
							}
							else if (query->getElementAt(varIt)->getVarType() == 1)
							{
								// cout << query->toString() << ".." << formula.getLiteral()->toString() << endl;
								temp.push_back(pair<Var*, Var*>(query->getElementAt(varIt), formula->getLiteral()->getElementAt(varIt)));
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
	
Literal QueryManager::applySubstitution(Literal* result, Literal* query, const vector<pair<Var*,Var*>>& matches)
	{				
		copyLiteral(query,result);				
		for (int sigIt = 0; sigIt < matches.size(); sigIt++)
		{
			for (int i = 0; i < query->getElements().size(); i++)
			{
				if (query->getElementAt(i)->equal(matches.at(sigIt).first)==0)
				{
					result->setElementAt(i, matches.at(sigIt).second);					
				}
			}
		}		
		return *result;
};

int QueryManager::executeQuery(Formula& f, Tableau& tableau, pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>>& result, int YN, vector<int>& ynAnswer)
	{
		vector<Literal*> qLits;
		formula = f;
		extractLiterals(formula, qLits);
		int matchFound = -1; //use -1 for errors
		//Print Literals from Query
		/*cout << "Literals from query: " << qLits.size() << endl;
		for (Literal* a : qLits)
			cout << a->toString() << endl;
		//---------------------------------
		*/
		for (int branchIt=0; branchIt<tableau.getOpenBranches().size();branchIt++)
		{
			int res=0;
			vector<vector<pair<Var*, Var*>>> matchSet(0, vector<pair<Var*,Var*>>(0)); //partial solutions set for the current branch
            int qIter=0; 
			for (; qIter < qLits.size(); qIter++)
			{
				res = 0;
				if (qLits.at(qIter)->containsQVariable() == 0)
				{
					res = checkQueryLiteralMatchInBranch(tableau.getOpenBranches().at(branchIt), qLits.at(qIter)); 
				}
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
						Literal sigq = Literal(-1, vector<Var*>(0));
						applySubstitution(&sigq, qLits.at(qIter), matchSet.at(solIter));
						if (sigq.containsQVariable() == 0)
						{
							if (checkQueryLiteralMatchInBranch(tableau.getOpenBranches().at(branchIt), &sigq) == 1)
								tmp.push_back(matchSet.at(solIter));
							else { tmp.clear(); break; }

						}
						else 
							checkQueryVariableMatchInBranch(tableau.getOpenBranches().at(branchIt), &sigq, matchSet.at(solIter), tmp);
						// apply partial solution to q_i
						//then call checkQueryMatchInbranch wiht sigma(q_i)
					   }
					matchSet = tmp;
					if (matchSet.empty())
					  res = 0;
					else 
					  res = 1;					
				}
			}				
				if (res == 0)
					break;
			}				
			if (!matchSet.empty())
			{
				result.first.push_back(branchIt);
				result.second.push_back(matchSet);
				matchFound = 1;
				if (YN == 1)
					ynAnswer.at(branchIt) = 1;
			}
			else if (YN == 1)
			{
				ynAnswer.at(branchIt) = res;
				matchFound = (res?res:0);
			}
		}
		return matchFound;
	};

/**
End QueryManager
*/





int performQuery(QueryManager*& queryManager, string& str, Formula** formula, Tableau& tableau, int yn)
{
	
	queryManager = (new QueryManager(5, 50)); 
	int typeformula = 1; 
	insertFormulaKB(0, queryManager->getQVQL(), queryManager->getQVVL(), str, formula, &typeformula);	
	
	pair <vector<int>, vector<vector<vector<pair<Var*, Var*>>>>> result(vector<int>(0), vector<vector<vector<pair<Var*, Var*>>>>(0));
	vector<int> ynanswer;
	if (yn == 0)
		ynanswer = vector<int>(0);
	else
		ynanswer = vector<int>(tableau.getOpenBranches().size());
	int matchFound=queryManager->executeQuery(**formula, tableau, result, yn, ynanswer); 
	queryManager->setMatchSet(result);
	queryManager->setAnswerSet(ynanswer);	
	return matchFound;	
};

void performQuerySet(vector<QueryManager*>& results,vector<string>& strings, vector<Formula>& formulae, Tableau& tableau)
{
#ifdef debug 
#ifdef debugquery
	logFile << "-----Parsing line:" << strings.at(0) << endl;
#endif
#endif // debug		
	for (string s : strings) // Manage multiple query
	{
		Formula *f = NULL;  
		QueryManager* manager=NULL; 
		int result = performQuery(manager, s, &f, tableau, 1);       
		formulae.push_back(*f);
		results.push_back(manager); //Multiple query to be managed
	}	
};


void propagateNegation(Formula* current, vector<bool>& isNegated, vector<Formula*>& stackF, bool valLeft, bool valRight)
{
#ifdef debug
#ifdef debugnorm		
	logFile << "------ Dropping Negation boleans to subformulae of:-----" << endl;	
	logFile << "-------"<<current->toString() << endl;
#endif
#endif // debug

	if (current->getLSubformula() != NULL)
	{
		stackF.push_back(current->getLSubformula());
		isNegated.push_back(valLeft);
	}
	if (current->getRSubformula() != NULL)
	{
		stackF.push_back(current->getRSubformula());
		isNegated.push_back(valRight);
	}
}

void dropLRImplication(Formula* f)
{	
#ifdef debug
#ifdef debugnorm		
	logFile << "------ Dropping II from:" << endl;
	logFile << f->toString() << endl;
#endif
#endif // debug
	Formula* right = new Formula(NULL, 5);
	right->setLSubformula(copyFormula(f->getLSubformula(), right));
	right->setRSubformula(copyFormula(f->getRSubformula(), right));

	Formula* left = new Formula(NULL, 5);
	left->setLSubformula(f->getRSubformula());
	left->getLSubformula()->setPreviousFormula(left);

	left->setRSubformula(f->getLSubformula());
	left->getRSubformula()->setPreviousFormula(left);

	f->setLSubformula(left);
	f->setRSubformula(right);
	left->setPreviousFormula(f);
	right->setPreviousFormula(f);
}

Formula* dropNegation(Formula *f, Formula **topform)
{	
#ifdef debug
#ifdef debugnorm		
	logFile << "------ Dropping Negation from:" << endl;
	logFile << "-------" << f->toString() << endl;
#endif
#endif // debug
	Formula* father = f->getPreviousformula(); 
	Formula* tmp = f->getRSubformula();	
	if (tmp == NULL)
		tmp = f->getLSubformula();	
	if (father == NULL)
	{		
	  (*topform) = tmp;	
	}
	else 
	{
		if (father->getLSubformula() == f)
			father->setLSubformula(tmp);
		else
			father->setRSubformula(tmp);
		tmp->setPreviousFormula(father);
	}	
	delete(f);	
	return(tmp); 	
}

Formula* convertFormulaToNNF(Formula* formula)
{
	
#ifdef debug
#ifdef debugnorm
	logFile << "-----Converting Formula to NNF:------" << endl;
	logFile << "------" << formula->toString() << endl;
#endif
#endif // debug
	if (formula->getLiteral() != NULL)
	{
#ifdef debug
#ifdef debugnorm		
		logFile << "------ Obtained:" << endl;
		logFile << "-------" << formula->toString() << endl;
#endif
#endif // debug
		return formula;
	}
	Formula* startFormula = formula;
	vector<bool> isNegated;
	vector<Formula*> stackF;
	stackF.push_back(startFormula);	
	isNegated.push_back(false);	
	
	while (!stackF.empty())
	{
		Formula* current = stackF.back();
		bool neg = isNegated.back();
		stackF.pop_back();
		isNegated.pop_back();		
		if (current->getLiteral() != NULL )
		{
			
			if (neg)
			{	;
#ifdef debug
#ifdef debugnorm		
				logFile << "------ Found Negation on Literal:" << endl;
				logFile << "-------" << current->toString() << endl;
#endif
#endif // debug
			  int  newOp = (current->getLiteral()->getLiteralOp() >= 2 ? -2 : 2);			  
			  current->getLiteral()->setLiteralOp(current->getLiteral()->getLiteralOp() + newOp);
#ifdef debug
#ifdef debugnorm		
			  logFile << "------ Computing:" << endl;
			  logFile << "-------" << current->getLiteral()->toString() << endl;
#endif
#endif // debug
			 }
		}
		else
		{			
		 switch (current->getOperand())
		  {
			case 0:
				if (neg)
				{					
					current->setOperand(1);
#ifdef debug
#ifdef debugnorm		
					logFile << "------ Found Negation on \"OR\":" << endl;
					
#endif
#endif // debug
				}
				propagateNegation(current, isNegated, stackF, neg, neg);
				break;

			case 1:
				if (neg)
				{
					current->setOperand(0);
#ifdef debug
#ifdef debugnorm		
					logFile << "------ Found Negation on \"AND\":" << endl;

#endif
#endif // debug
				}
				propagateNegation(current, isNegated, stackF, neg, neg);
				break;

			case 2:
				if (neg)
				{
					current->setOperand(0);
#ifdef debug
#ifdef debugnorm		
					logFile << "------ Found Negation on \"NOR\":" << endl;

#endif
#endif // debug
				}
				propagateNegation(current, isNegated, stackF, !neg, !neg);
				break;

			case 3:
				if (neg)
				{
					current->setOperand(1);
#ifdef debug
#ifdef debugnorm		
					logFile << "------ Found Negation on \"NAND\":" << endl;

#endif
#endif // debug
				}
				propagateNegation(current, isNegated, stackF, !neg, !neg);
				break;

			case 4: //case negation formula				
#ifdef debug
#ifdef debugnorm		
				logFile << "------ Found Negation on \"NEG\":" << endl;

#endif
#endif // debug			
				stackF.push_back(dropNegation(current, &startFormula));					
				isNegated.push_back(!neg);
				break;

			case 5: //case ->					
				if (neg) //case \neg ->			  
				{
					current->setOperand(1);
#ifdef debug
#ifdef debugnorm		
					logFile << "------ Found Negation on \"IF\":" << endl;

#endif
#endif // debug
				}
				else
					current->setOperand(0);
				propagateNegation(current, isNegated, stackF, !neg, neg);
				break;

			case 6: //case <-
				if (neg) //case \neg ->			  
				{
					current->setOperand(1);
#ifdef debug
#ifdef debugnorm		
					logFile << "------ Found Negation on \"FI\":" << endl;

#endif
#endif // debug
				}
				else
					current->setOperand(0);
				propagateNegation(current, isNegated, stackF, neg, !neg);
				break;

			case 7:
				if (neg) //case \neg <-->
				{
					current->setOperand(0);
#ifdef debug
#ifdef debugnorm		
					logFile << "------ Found Negation on \"II\":" << endl;

#endif
#endif // debug
				}
				else
					current->setOperand(1);
				dropLRImplication(current);
				propagateNegation(current, isNegated, stackF, neg, neg);
				break;
			}
		}
	}
#ifdef debug
#ifdef debugnorm	
	logFile << "------ End converting formula to NNF" << endl;
	logFile << "------ Formula Computed:" << endl;
	logFile << "-------" << startFormula->toString() << endl;
#endif
#endif // debug
	return startFormula;		
}



int precomputeKBSpaceFromString(string& inputformula, vector<int>& KBsize, vector<int>& Litsize)
{
	throw new exception("Function to be implemented");
	return -1;
}

void precomputeKBSpaceFromFile(string &name, vector<int>& KBsize, vector<int>& Litsize)
{
	std::ifstream file(name);
	std::string str;	
	while (std::getline(file, str))
	{		
		if ((str.rfind("//", 0) != 0) || !str.empty())
		{
		  precomputeKBSpaceFromString(str, KBsize, Litsize);
		}
	}
	
}


void generateConst(int r, int n, vector<int>& b)
{
	int i;
	if (r < 2 || n < 1)          /* parameter check */
		return;
	r -= 1;                    /* r = max digit value */
	do
	{
		i = n - 1;
		while (b.at(i) < r)
		{        /* increment last digit */
			b[i]++;
			//show(b, n);
		}
		/* find next digit to increment */
		while (i >= 0 && b.at(i) == r)
			i--;
		if (i < 0)
			break;         /* return if done */
		b.at(i)++;
		while (++i < n)          /* zero following digits */
			b.at(i) = 0;
		//show(b, n);
	} while (1);
	//free(a);
}

void retrieveQVarSet(vector<Literal*>&atomset, vector<vector<Var*>>&varset)
{
	for (Literal* lit : atomset)
	{
		for (Var* var : lit->getElements())
		{
			if (var->getVarType() == 1)
			{
				int i = 0;
				for (; i < varset.at(var->getType()).size(); i++)
				{
					if (var->equal(varset.at(var->getType()).at(i)))
						break;
				}
				if (i < varset.at(var->getType()).size())
					varset.at(var->getType()).push_back(var);
			}
		}
	}
}


void expandGammaTableau(Tableau& T)
{
	//throw new exception("Unsupported");
	cout << "GAMMA" << endl;
	vector<Node*> newNodeSet;
	newNodeSet.push_back(T.getTableau());
	for(int i=0; i<T.getTableau()->getSetFormulae().size();i++)
	{
		Formula* currUnfulFormula = T.getTableau()->getSetFormulae().at(i);
		currUnfulFormula->setFulfillness(0);		
		vector<Literal*> atomset;		
		int val = getLiteralSet(currUnfulFormula, atomset);		
		if (val == 0 && atomset.size() == 0)			
			continue;
		vector<vector<Var*>> varset(4);
		retrieveQVarSet(atomset, varset);
		cout << "----" << endl;
		for (Literal* at : atomset)
		{
			cout << at->toString() << endl;
		}
		cout << "----" << endl;
		
	

		//instantiate and apply tableau rule
        /*
		vector<int>b = {1,1 };
		generateConst(3, 2, b);
		*/
	}

}



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
				cout << tmp->getSetFormulae().at(j)->toString() << endl;
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
				cout << tmp->getSetFormulae().at(j)->toString() << endl;
			cout << "----" << endl;
			tmp = tmp->getFather();
		}
	}
}

void printTRadix(vector<Formula*>& KB)
{
	cout << "---Radix Content ---" << endl;
	for (int i = 0; i < KB.size(); i++)
	{		
		cout << KB.at(i)->toString() << endl;
	}
	
}

void printTExpanded(Tableau& tableau)
{
	cout << "Content of Expansion:" << endl;
	for (int i = 0; i < tableau.getTableau()->getSetFormulae().size(); i++)
	{
		cout << (tableau.getTableau()->getSetFormulae().at(i)->toString()) << "," << tableau.getTableau()->getSetFormulae().at(i)->getFulfillness() << endl;
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
check brackets; check format of a formula in general as preprocessing and Check if a Literal is built correctly.
define special chars from a config file. Setting the size of the special chars and checking for correctness.
allowing change of the $ char from a config file.
Optimize Literal management and creation
Test Cases
Optimize expandKB
remove recursion
A more efficient expansion function is required.
Ensure that a) Vectors on VariableSet are only-read b) coerence of pointer to elements of such vectors.
Done Remove the existence of two functions createQueryVarFromString and createKBVarFromString by generalizing the use of vectors in VarSet. Same thing with
DONE - Solve PBRule inefficiency
*/
