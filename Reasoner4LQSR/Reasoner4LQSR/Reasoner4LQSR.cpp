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
#define debug
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
			string name;  // Underscore is allowed to define name with underscore			
			//int isValidType(int _type) { return _type > minVarSize && _type <= maxVarSize; };
			//int isValidVar(int _var) { return _var >= 0 && _var <= 1; };
			
   public:
		   Var(string _name, int _type, int _var)
		     {
			   setName( _name);
			   setType(_type);
			   setVarType(_var);
		     };
		   ~Var() {};
		   //int isValidType() { return isValidType(type);};
		   //int isValidVar() { return isValidVar(var); };
		   int getType() { return type; };
		   string getName() { return name; };
		   int getVarType() { return var; };
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
   private :
          vector< vector <Var> > VVL;
          vector< vector <Var> > VQL;
		  int capacity;
		  int nlevel;
   public :
	     VariablesSet() {};
	     VariablesSet(int maxNVariable, int maxSize)
		  {
		   nlevel = maxNVariable;
		   capacity = maxSize;
		   VQL.reserve(nlevel);  //initialize vectors for variables
		   VVL.reserve(nlevel);
		   for (int i = 0; i <= nlevel; i++)
			 {
				 VQL.push_back(vector<Var>());
				 VVL.push_back(vector<Var>());
			 }
		   for (int i = 0; i <= nlevel; i++)
			 {
				 VQL.at(i).reserve(maxSize);
				 VVL.at(i).reserve(maxSize);
			 }
		  };

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

		int VVLPushBack(int inslevel, string name, int level, int vartype)
		 {
           #ifdef debug 
			logFile << "-----Inserting Data in Variable Set of level: " << inslevel << ". Name: " << name <<". Level: "<< level<<". Type: "<< vartype << endl;
			logFile << "-----Variable Set of level: " << inslevel << " has size: " << VVL.at(inslevel).size() <<" and capacity: "<< VVL.at(inslevel).capacity() << endl;
          #endif // debug

			if (VVL.at(inslevel).size() < VVL.at(inslevel).capacity())
			{
				VVL.at(inslevel).push_back(*new Var(name, level, vartype));
				return 0;
			}
			return 1;
		 }

		int VQLPushBack(int inslevel, string name, int level, int vartype)
		{
          #ifdef debug 
			logFile << "-----Inserting Data in Quantified Variable Set of level: " << inslevel << ". Name: " << name << ". Level: " << level << ". Type: " << vartype << endl;
			logFile << "-----Quantified Variable Set of level: " << inslevel << " has size: " << VQL.at(inslevel).size() << " and capacity: " << VQL.at(inslevel).capacity() << endl;
           #endif // debug
		  if (VQL.at(inslevel).size() < VQL.at(inslevel).capacity())
			{
				VQL.at(inslevel).push_back(*new Var(name, level, vartype));
				return 0;
			}
			return 1;
		}

		Var* VVLGetBack(int level) { return &VVL.at(level).back(); }
		Var* VQLGetBack(int level) { return &VQL.at(level).back(); }
		size_t VVLGetSize() { return VVL.size(); }
		size_t VQLGetSize() { return VVL.size(); }
		size_t VVLGetSizeAt(int level) { return VVL.at(level).size(); }		
		size_t VQLGetSizeAt(int level) { return VQL.at(level).size(); }
 };

 //Special chars of a formula
 class Operators
 {
  private :
            string logOp[4] = { "$OR","$AD","$RO","$DA" }; //logic operators
            string setOp[7] = { "$IN", "$EQ", "$NI", "$QE", "$OA","$AO", "$CO" }; //set operators
            string qutOp[1] = {"$FA"};	//quantifiers
  public :
	  Operators() {};
	  size_t getLogOpSize() { return logOp->size(); };
	  size_t getSetOpSize() { return logOp->size(); };
	  size_t getQuantiOpSize() { return qutOp->size(); };
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
	 for (int i = 0; i < getSetOpSize(); i++)
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
	      Atom(int op, vector<Var*> vec) 
           {
			  //components.reserve(maxVarSize);
			  setAtomOp(op);
			  components = vec;
		   };		  
		 
		  vector<Var*>* getElements()
		   {
			return &components;
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
			if ( index < components.size())			  
			  return components.at(index);
			return NULL;			  
		   }

		  ~Atom() {};

		  /*
		     Use Carefully. Remember the position of the left/right operand
		  */
		  void addElement(Var& element)
		   {
			 (*getElements()).push_back(&element);
		   };	

		  string toString()
		   {
			string out = "(";
			if (getElements()->size() > 2)
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
 };

 class Formula  //struttura pi� generica da cambiare eventualmente con strutture per CNF
 {
   private:
	 //vector<Var*> quantified;
	 Atom *atom;
	 int operand; 
	 Formula *lsubformula;
	 Formula *rsubformula;
	 Formula *pformula;
   public:
	 Formula()
	   {
		 setAtom(NULL);
		 setOperand(-1);
		 setLSubformula(NULL);
		 setRSubformula(NULL);
		 setPreviousFormula(NULL);
	   };	
	 Formula(Atom *at, int op)
	 {
	   setAtom(at);
	   setOperand(op);
	   setLSubformula (NULL);
	   setRSubformula(NULL);
	   setPreviousFormula(NULL);	  
	 };	
	 Formula(Atom *at, int op, Formula *lf, Formula *rf)
	 {
	   setAtom(at);
	   setOperand(op);
	   lsubformula = lf;
	   rsubformula = rf;	  
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
 public:
	 Node(int size) { setFormula.reserve(size); leftChild = rightChild = father = NULL; };
	 Node() { leftChild = rightChild = father = NULL; };
	 Node(vector<Formula>* formula) { setFormula = *formula; leftChild = rightChild = father = NULL; };
	 Node* getLeftChild() { return leftChild; };
	 Node* getRightChild() { return rightChild; };
	 Node* getFather() { return father; }
	 void setRightChild(Node* child) { rightChild = child; };
	 void setLeftChild(Node* child) { leftChild = child; };
	 void setFather(Node* f) { father = f; };
	 void insertFormula(Formula* f) { setFormula.push_back(*f); };
	 vector<Formula>* getSetFormulae() { return &setFormula; };
	 ~Node() {};
 };


 /*
    The Tableau.
 */
 class Tableau
 {
   private: Node* radix;
   public: 
	   Tableau(Node* initial) { radix = initial; };
	   Tableau(int size_radix) { radix = new Node(size_radix); };
	   Node* getTableau() { return radix; };
	   int insertFormula(Formula *f) { radix->insertFormula(f);  return 0; };
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
		cout << out.top()<<endl;
		out.pop();
	}
}

int containsVariableName(vector<Var>* vect, Var** found, const string *name, const int *start)
{		
 for (int s=*start; s < vect->size(); s++)
	{
      if( (name->compare( (vect->at(s)).getName())==0 ))
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
	logFile << "-----Extracting Data From Var. Input String: " << input << endl;
  #endif // debug
  *level = input.at(1) - '0';
  *name = input.substr(3, (input.find_last_of('}') - 3));
  #ifdef debug 
   logFile << "-----Data From Var Computed. Name: " << *name << ". Level:" << *level << endl;
  #endif // debug
  return 0;
}

Var* createVarFromString(string *name, int *level, int *vartype, int *start)
{ 
	Var* ret;
    #ifdef debug 
 	 logFile << "-----Creating  Variable.  Name:" << *name << ". Level: " << *level << ". Type: " << *vartype << endl;
    #endif // debug
	if (containsVariableName(varSet.getVVLAt(*level), &ret, name, start) == 0)
	{
      #ifdef debug 
		logFile << "-----Variable Found in Set. " << ret->toString() << endl;
      #endif // debug
	 return ret;
	}
	else if (containsVariableName(varSet.getVQLAt(*level), &ret, name, start) == 0)
	{
      #ifdef debug 
		logFile << "-----Variable Found in Quantified Set. " << ret->toString() << endl;
      #endif // debug
	 return ret;
	}
	else
	{     
	  varSet.VVLPushBack(*level, *name,*level,*vartype);
	  ret=varSet.VVLGetBack(*level);
      #ifdef debug 
	    logFile << "-----Adding Variable to Set. " << ret->toString() << endl;
      #endif // debug
	  return ret;
	}
}

Var* createQVarFromString(string *name, int *level, int *vartype, int *start)
{
 #ifdef debug 
	logFile << "-----Creating Quantified Variable.  Name:" << *name << ". Level: " << *level << ". Type: "<< *vartype << endl;
 #endif // debug
 Var* ret;
 if (containsVariableName(varSet.getVQLAt(*level), &ret, name, start) == 0)
  {
   #ifdef debug 
	 logFile << "-----Quantified Variable Found in Set."<< ret->toString()  << endl;
   #endif // debug
   return ret;
  }
 else
  {
   varSet.VQLPushBack(*level, *name,*level, *vartype);
   ret=varSet.VQLGetBack(*level);
   #ifdef debug 
    logFile << "-----Adding Quantified Variable to Set." << ret->toString() << endl;
   #endif // debug
   return ret;
 }
}

/*
  Create an Atom from the given string
*/
int createAtom(string input, Formula **formula, vector<int>* startQuantVect)
{	
  #ifdef debug  
	logFile << "-----Computing Atom: " <<  input << endl;
   #endif // debug
	Var* var1;
	Var* var2;
	Var* var3;
	string name = string();
	int level = -1;
	Atom* atom = NULL;
	if (input[0]=='$') //case pair or quantifier
	{
		string head = input.substr(0, 3); //cout << head << endl;	
		string match = input; 
		if (head.compare("$OA") == 0)  //case  pair
		{			
			match = input.substr(3, input.size() - 1);
			size_t found = match.find("$");  
			retrieveVarData(match.substr(0, found), &name, &level);
			var1 = createVarFromString(&name, &level, new int(0), &startQuantVect->at(level));
			match = match.substr(found+3, match.size()-1); //here the comma
			found = match.find("$");
			retrieveVarData(match.substr(0, found), &name, &level);
			var2 = createVarFromString(&name, &level, new int(0), &startQuantVect->at(level));			
			match = match.substr(found + 3, match.size() - 1); 
			int op = operators.getSetOpValue(match.substr(0, 3));
			match = match.substr(3, match.size() - 1);
			retrieveVarData(match, &name, &level);
            var3 = createVarFromString(&name, &level, new int(0), &startQuantVect->at(level));
            atom = new Atom(op, {var3, var1, var2 });
			//Atom* atom = new Atom(0, { new Var(name,level,0), new Var("b1",0,0), new Var("c1",0,0) });
			*formula = (new Formula(atom, -1 ));
			//cout << "Atom found: " << formula->print() << endl;			
		}
		else if (head.compare("$FA")==0)  //case quantified variable
		{				
			retrieveVarData(match.substr(3, match.size() - 1), &name, &level);
			var1 = createQVarFromString(&name, &level, new int(1), &startQuantVect->at(level));
			*formula =  NULL; //var
		}
	} 
	else if( input[0]=='V') //case single var
	{
      size_t found = input.find("$");//case no pair
	  if (found != string::npos)
		{   
		  retrieveVarData(input.substr(0, found), &name, &level);
		  var1= createVarFromString(&name,&level,new int(0), &startQuantVect->at(level));
		  int op =  operators.getSetOpValue(input.substr(found, 3)); 
		  retrieveVarData(input.substr(found + 3, input.size() - 1), &name, &level);
		  var2 = createVarFromString(&name, &level, new int(0), &startQuantVect->at(level));
		  atom =  new Atom(op, {var2, var1} );		  
		  *formula = (new Formula(atom, -1));		 	  
		}
	}	
    #ifdef debug  
	if(atom !=NULL)
	  logFile << "-----Atom created: " << atom->toString() << endl;
    #endif // debug
	return 0;
}

/*
Parse a string representing an internal formula and return the corresponding internal formula. 
*/
int parseInternalFormula(const string *inputformula, Formula **outformula, vector<int>* startQuantVect)
{
    #ifdef debug  
	   logFile << "-----Analizyng input formula" << endl;
    #endif // debug
    stack<string> stackFormula;
	stack<Formula*> stformula; //tracking subformulae
	stackFormula.push(*inputformula);	
	string top = stackFormula.top();
	stackFormula.pop();  
	string atom=string();
	string operand=string();
	Formula* formula= NULL;
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
					  logFile << "-----Candidate atom found: " << atom << endl;
                    #endif // debug
					createAtom(atom, &formula, startQuantVect); 
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
					  logFile << "-----Computing subformula: " << centerf->toString() << endl;
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
		cout << element.toString() << endl;
	} 

}

/*
  Create an object of type Formula  from the given string representing a formula.
*/
int insertFormula(string* formula, Formula **ffinal)
{
	/*
	  The following vector of int represents the position of the quantified variables of the current formula.
	  Initially the vector plainly coincides the the end of the vector modified in the previous execution.
	*/
 #ifdef debug  
	logFile << "---Formula Inserted: " << *formula << endl;
 #endif // debug
 vector<int> vqlsize;
 for (int i = 0; i < varSet.VQLGetSize(); i++)	
  vqlsize.push_back((int) varSet.VQLGetSizeAt(i));
 parseInternalFormula(formula, ffinal, &vqlsize); 
 #ifdef debug  
   logFile << "---Formula Ended "<< endl;
 #endif // debug
 return 0;
}

int insertFormulaKB(string s, Tableau* t)
{
	Formula* f;
	insertFormula(&s, &f);
	t->insertFormula(f);
	return 0;
}

int instantiateFormula(Formula f, vector<Formula> *destination)
{
   #ifdef debug  
	logFile << "------- Expanding Formula: " << f.toString() << endl;
   #endif // debug
	destination->push_back(f);
	return 0;
}

int expandKB(vector<Formula> *inpf)
{ 
  #ifdef debug  
		 logFile << "--- Applying Expansion Rule" <<endl;
  #endif // debug
	int or = operators.getLogOpValue("$OR");
	vector <Formula> tmp;
	vector <Formula> out;
	for(int i=0; i< inpf->size(); i++)
	  tmp.push_back(inpf->at(i));

	while(!tmp.empty())
	 {
       
		Formula f = tmp.back();
        #ifdef debug  
		 logFile << "----- Computing Formula: " << f.toString() << endl;
        #endif // debug
		tmp.pop_back();
		if (f.getAtom() != NULL || f.getOperand()== or)
		 {       
		   instantiateFormula(f,&out);			
		 }
		else
		{			
			tmp.push_back(*(f.getLSubformula()));
			tmp.push_back(*(f.getRSubformula()));			
		}
	 }
	
	for (int i = 0; i < out.size(); i++)
		cout<<out.at(i).toString()<<endl;

	return 0;
}



int main()
{    
  #ifdef debug  
	logFile << "Debug Started"<<endl;
  #endif // debug
   /*
      Initialization
   */
  varSet = VariablesSet(3, 100);
  operators = Operators();
  Tableau tab(new Node()); //empty tableau
  /*
     Inserting Knowledge Base
  */
  insertFormulaKB("($FA V0{z}) ($OA V0{z} $CO V0{z} $AO $NI V3{C333})",&tab);
  insertFormulaKB("($FA V0{z1}) ($FA V0{z2}) (V0{z1} $EQ V0{z2})", &tab);
  insertFormulaKB("($FA V0{z3}) ( (V0{z3} $NI V1{C1}) $AD (  (V0{b} $NI V1{C1}) $OR (V0{a} $NI V1{C1}) ) )", &tab);
  insertFormulaKB(" ($FA V0{z}) ($FA V0{z1}) ( ( (V0{k} $NI V1{l}) $AD  ( ( V0{z} $NI V1{C1})$OR ( V0{z1} $NI V1{C2}))$AD((  $OA V0{z1} $CO V0{z1} $AO $NI V1{C2})$OR (V0{z1} $IN V1{C2}))))", &tab);
  
  Node* radix = tab.getTableau();
  vector <Formula> *sta =radix->getSetFormulae();
  cout << "---Radix Content ---" << sta->size() << endl;
  for ( Formula s : *sta)
   {
	 cout << (s.toString()) << endl;	 
   } 

    
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

  /*
  
  */
  cout << "Expanding KB" << endl;
  cout << "First step of expansion" << endl;
  expandKB(sta);
  logFile.close(); 
  return 0;
}

/*
Mapping from 4LQSR formulae in semi-internal formulae
/FA -> ForAll
/IN -> IN
/NI -> NOTIN
/EQ -> =
/QE -> NOT =
/OA -> <
/AO -> >
/AD -> AND
/DA -> NOT AND
/OR -> OR
/RO -> NOT OR
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
insert logging
USE A MAP to MAP from string representing operator and integer representing operator.
check brackets; check format of a formula in general as preprocessing and Check if an atom is built correctly.
define special chars from a config file. Setting the size of the special chars and checking for correctness.
allowing change of the $ char from a config file.
creating a quantified variable for a formula does not check if it is yet present
Optimize Atom management and creation
Test Cases
*/

/*
Formula* ffinal;
Formula* ffinal2;
Formula* ffinal3;
Formula* ffinal4;
string formula = "($FA V0{z}) ($OA V0{z} $CO V0{z} $AO $NI V3{C333})";
string formula2 = "($FA V0{z1}) ($FA V0{z2}) (V0{z1} $EQ V0{z2})";
// string formula2 = "($FA V0{ z1 }) (V0{ z1 } $NI V1{ C1 })";
// string formula = "($FA V0{z})(( ( V0{z} $NI V1{C1})$OR ( V0{z1} $NI V1{C2}))$AD(( V0{z1} $NI V1{C2})$OR (V0{z1} $IN V1{C2})))";
// string formula2 = "($FA V0{z2}) ( (V0{z2} $NI V1{C1}) $AD (  (V0{b} $NI V1{C1}) $AD (V0{b} $NI V1{C1})  ) )";
string formula3 = "($FA V0{z3}) ( (V0{z3} $NI V1{C1}) $AD (  (V0{b} $NI V1{C1}) $OR (V0{a} $NI V1{C1}) ) )";
string formula4 = " ($FA V0{z}) ($FA V0{z1}) ( ( (V0{k} $NI V1{l}) $AD  ( ( V0{z} $NI V1{C1})$OR ( V0{z1} $NI V1{C2}))$AD((  $OA V0{z1} $CO V0{z1} $AO $NI V1{C2})$OR (V0{z1} $IN V1{C2})))) ";
// cout << "Current Formula is: " << formula << endl;
insertFormula(&formula, &ffinal);
insertFormula(&formula2, &ffinal2);
insertFormula(&formula3, &ffinal3);
insertFormula(&formula4, &ffinal4);
cout << "Parsed formula: " << ffinal->toString() << endl;
cout << "Parsed formula: " << ffinal2->toString() << endl;
cout << "Parsed formula: " << ffinal3->toString() << endl;
Tableau tab(new Node()); //empty tableau
Node* radix = tab.getTableau();
radix->insertFormula(ffinal);
radix->insertFormula(ffinal2);
radix->insertFormula(ffinal3);
radix->insertFormula(ffinal4);


*/







/*Var b ("monastero", 0, 0);
Var b1("monastero", 0, 0);
Var x("livello", 0, 0);
Var x1("livello", 0, 0);
Var g("Edificio", 1, 0);
Var g1("Edificio", 1, 0);
Var h("haLivello", 3, 0);
Var h1("haLivello", 3, 0);
Atom atom(0, {&h,&b,&x});
Atom atom2(0, {&g,&b});
Atom atom3(1, { &b,&x });
Formula af = Formula(NULL, 1, new Formula(&atom, -1), new Formula(&atom2, -1));
Formula af2 = Formula(NULL, 0, &af, new Formula(&atom3, -1));  */







/*
//cout << "Testing Add Var:" << endl;
//cout<<VVL.at(0).size()<<endl;
//cout << VVL.at(0).back().print()<<endl;
//cout << "----------------------" << endl;
cout << b.print()<<endl;
cout << "b is equal to x: "<< b.equal(&b) <<endl;
cout << b.getName() <<","<< b.getType() <<","<< b.getVarType() << endl;
cout << x.getName() << "," << x.getType() << "," << x.getVarType() << endl;
cout << g.getName() << "," << g.getType() << "," << g.getVarType() << endl;
cout << h.getName() << "," << h.getType() << "," << h.getVarType() << endl;
cout << "Testing print Var" << endl;
cout <<  b.print() << endl;
cout << x.print() << endl;
cout << g.print() << endl;
cout << h.print() << endl;
// cout<< "--------------------  "<< VQL.at(0).capacity()<<endl;
// cout << "-------------------- " << VQL.at(0).at(0).getName() << endl;
// cout<<addNewElement(x, VCL)<<endl;
//cout << VCL.at(0).at(0).getName() << endl;
// cout << VQL.capacity()<< endl;
*/

// cout << af2.print() << endl;
/*
cout << "Testing print atom:" << endl;
cout << atom.print() << endl;
cout << atom2.print() << endl;
cout << atom3.print() << endl;
cout << "Atom print: " <<  atom.getElementAt(0)->getName() << endl;
cout << "Atom print: " <<  atom.getElementAt(1)->getName() << endl;
if( ((atom.getElementAt(2)))==NULL)
cout << "Atom print: NULL " << endl;
*/
// Tableau tab( &Node() ); //empty tableau
// Node* radix=tab.getTableau();

/* cout << "------------" << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(0)->getVarType() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(0)->getType() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(0)->getName() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(1)->getVarType() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(1)->getType() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(1)->getName() << endl;*/

/* cout << "------------" << endl;
insertFormula(&formula2, &ffinal3);
cout << "------------" << endl; */
/* cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(0)->getVarType() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(0)->getType() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(0)->getName() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(1)->getVarType() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(1)->getType() << endl;
cout << "Parsed formula: " << ffinal->getAtom()->getElementAt(1)->getName() << endl;
cout << "------------" << endl;*/
/*cout << "------------" << endl;
cout << "Parsed formula: " << ffinal3->toString() << endl;
cout << "------------" << endl;*/
//insertFormula(&formula3, &ffinal3);

/*for (int i=0; i< sta->size(); i++)
{
cout<< (sta->at(i).toString())<< endl;
}*/