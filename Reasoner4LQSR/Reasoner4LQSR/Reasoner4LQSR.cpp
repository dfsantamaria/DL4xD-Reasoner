// Reasoner4LQSR.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <array>
#include <stack>
#include <sstream>

using namespace std;
//#define debug
std::ofstream logFile;   //log file

/*
L0 has type 0
L1 has type 1
L2 has type 2
L3 as ObjectProperty has type 3
L3 as DataProperty has type 4
*/
const int minVarSize = -1;
const int maxVarSize = 3;   //size of variable
const int maxAtLen = 3;
const int maxOpLen = 4;

//Class of the single Var
 class Var
  {
   private: 
		    int type;
			int var;
			string name;  // Underscore is allowed to define name with underscore			
			int isValidType(int _type) { return _type > minVarSize && _type <= maxVarSize; };
			int isValidVar(int _var) { return _var >= 0 && _var <= 1; };
			
   public:
		   Var(string _name, int _type, int _var)
		     {
			  setName( _name);
			   setType(_type);
			   setVarType(_var);
		   };
		   ~Var() {};
		   int isValidType() { return isValidType(type);};
		   int isValidVar() { return isValidVar(var); };
		   int getType() { return type; };
		   string getName() { return name; };
		   int getVarType() { return var; };
		   int setName(string _name) 
		      { 
			    name = _name; 
			    return type; 
		      };
		   int setType(int _type)
		      { 
			   if (isValidType(_type))
			     type = _type;			    
			   else			     
				  type = -1; 
			   return type;			   
		      };
		   int setVarType(int _var)
		    {
			   if (isValidVar(_var))
				  var = _var;
			   else 
				 var = -1;	
			   return var;
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
		   string print()
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
 vector< vector <Var> > VVL;
 vector< vector <Var> > VQL;
 //Special chars of a formula
 string logOp[] = { "$OR","$AD","$RO","$DA" }; //logic operators
 string setOp[] = { "$IN", "$EQ", "$NI", "$QE", "$OA","$AO", "$CO" }; //set operators
 string qutOP[] = {"$FA"};	//quantifiers

 int getSetOpValue(string s)
 {
	 for (int i = 0; i < setOp->size(); i++)
	 {
		 if (setOp[i].compare(s) == 0)
			 return i;
	 }
	 return -1;
 }

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
			  //components.reserve(size);
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
		  /*
		     Use Carefully. Remember the position of the left/right operand
		  */
		  void addElement(Var& element)
		   {
			 (*getElements()).push_back(&element);
		   };	

		  string print()
		  {
			  string out = "(";
			  if (getElements()->size() > 2)
			  {
				out.append("$OA ");
				out.append(getElementAt(1)->print());
				out.append(" $CO "); 
				out.append(getElementAt(2)->print());
				out.append("$AO");
			  }
			  else
				out.append(getElementAt(1)->print());
			  out.append(" ");			  
			  out.append(setOp[getAtomOp()]);	
			  out.append(" ");
			  out.append(getElementAt(0)->print());
			  out.append(")");
			  return out;
		  };
 };

 class Formula  //struttura pi� generica da cambiare eventualmente con strutture per CNF
 {
   private:
	 vector<Var*> quantified;
	 Atom *atom;
	 int operand; //0 forAND, 1 for OR , 2 for NAND, 3 for NOR
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
	 vector<Var*>* getQuantifiedSet() { return &quantified; };
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
	 void print()
	 { 
		 /*if (getAtom() != NULL)
			 cout << getAtom().print();
		 else
			 cout << getOperand();
			 */
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
	 Node() { leftChild = rightChild = father = NULL; };
	 Node(vector<Formula>* formula) { setFormula = *formula; leftChild = rightChild = father = NULL; };
	 Node* getLeftChild() { return leftChild; };
	 Node* getRightChild() { return rightChild; };
	 Node* getFather() { return father; }
	 void setRightChild(Node* child) { rightChild = child; };
	 void setLeftChild(Node* child) { leftChild = child; };
	 void setFather(Node* f) { father = f; };
	 void insertFormula(Formula* f) { setFormula.push_back(*f); };
	 vector<Formula>* getSetFormulae() { return &setFormula; }
 };


 /*
    The Tableau.
 */
 class Tableau
 {
   private: Node* radix;
   public: 
	   Tableau(Node* initial) { radix = initial; };
	   Node* getTableau() { return radix; };	   
 };

 /*
   Init the vectors of variables.
 */
int init() //used to inizialize elements.
 {
  VQL.reserve(maxVarSize);  //initialize vectors for variables
  VVL.reserve(maxVarSize);		
  for (int i = 0; i <= maxVarSize; i++)
   {
	VQL.push_back(vector<Var>());
	VVL.push_back(vector<Var>());
   }
  return 0;
}


/*
Create and insert var from the given var name and the var type in the given vector of vector of var
*/

Var* insertSetVar(string *name, int *type, int vartype, vector<vector<Var>>& vec)
{  //VVL 
	vector<Var> *v = &(vec.at(*type)); 
	for (int i = 0; i< v->size(); i++)
	{
		if (v->at(i).equal(*name, *type, vartype) == 0)
			return &v->at(i); 
	}
	string outn = *name;
	int outt = *type;	
	Var* res = new Var( outn, outt, vartype);
	v->push_back(*res);
	return res;
}


/*
Create and insert a quatified var from the given var name and the var type
*/

Var* insertQVar(string *name, int *type)
{
return insertSetVar(name, type, 1 , VQL);
}

/*
   Create and insert a unquatified var from the given var name and the var type
*/

Var* insertVar(string *name, int *type)
{
	return insertSetVar(name, type, 0, VVL);
}

/*
  Return the value of the given string representing a logic operator
*/
int checkLogOp(string *s)
{
	for (const string &op : logOp)
	{
		if ( op.compare(*s) ==0)
			return 1;
	}
	return 0;
}

/*
   print a stack of string in inverted order
*/
void printStack(stack<string> st)
{
	stack<string> out;
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

void visitFormula(Formula *formula)
{
	//cout << formula->print()<<endl;

}


/*
   Create a Var given a string and an int (1 if it is quantified, 0 otherwhise) 
*/
Var* createVarFromString(string input, int vartype)
{
	int level = input.at(1)-'0';
	string name = input.substr(3, (input.find_last_of('}') - 3));
	if (vartype == 0)
		return insertVar(&name, &level);
	else if (vartype==1)
		return insertQVar(&name, &level);
	else return NULL;
}

/*
  Create an Atom from the given string
*/

int createAtom(string input)
{	
	Var* var1;
	Var* var2;
	Var* var3;
	if (input[0]=='$') //case pair or quantifier
	{
		string head = input.substr(0, 3); cout << head << endl;	
		string match = input;
		if (head.compare("$OA") == 0)  //case  pair
		{			
			match = input.substr(3, input.size() - 1);					
			size_t found = match.find("$");  
			var1 = createVarFromString(match.substr(0, found), 0);
			//cout << var1->getName() << endl;
			match = match.substr(found+3, match.size()-1); //here the comma
			found = match.find("$");
			var2 = createVarFromString(match.substr(0, found), 0);
			match = match.substr(found + 3, match.size() - 1); 
			int op = getSetOpValue(match.substr(0, 3));
			match = match.substr(3, match.size() - 1);
            var3 = createVarFromString(match, 0);
            Atom atom = Atom(op, {var3, var1, var2 });
			cout << "Atom found: " << atom.print() << endl;			
		}
	} 
	else if( input[0]=='V') //case single var
	{
      size_t found = input.find("$");//case no pair
	  if (found != string::npos)
		{   
		  
		  var1= createVarFromString(input.substr(0, found),0);
		  int op =  getSetOpValue(input.substr(found, 3)); 
		  var2 = createVarFromString(input.substr(found + 3, input.size() - 1),0);
		  Atom atom =  Atom(op, {var2,var1});
		  cout << "Atom found: " << atom.print()<<endl;	 
		  
		}
	}
	
	return 0;
}

/*
Parse a string representing an internal formula and return the corresponding internal formula
*/
int parseInternalFormula(string *inputformula, Formula *outformula)
{
	string strformu = *inputformula;
	stack<string> stackFormula;
	stackFormula.push(strformu);
	//while (!stackFormula.empty())
	{
		string top = stackFormula.top();
		stackFormula.pop();  //cout << top << endl;
		string atom=string();
		string operand=string();	
		for (int i = 0; i < top.length(); i++)
		{
			char c = top.at(i);					 
			switch (c)
			{
			case '(': stackFormula.push(string(1, c)); break;
			case ')': 
				if (!atom.empty())
				{					
					stackFormula.push(atom);
					createAtom(atom); //-----------------------------
					atom.clear();
				}
				stackFormula.push(string(1, c));
				break; 
			case '$': operand = string(1, c) + string(1, top.at(i + 1)) + string(1, top.at(i + 2));
				i = i + 2;
				if (checkLogOp(&operand) != 0)
				{
					stackFormula.push(operand);
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
	}
	printStack(stackFormula);
	return 0;
	
		
}

void printVector(vector<Var>& v)
{
	for (Var element : v)
	{
		cout << element.print() << endl;
	} 

}



int main()
{  
  int res=init();
  std::ofstream logFile("LOG.log");
  #ifdef debug  
  #endif // debug
  Var b ("monastero", 0, 0);
  Var b1("monastero", 0, 0);
  Var x("livello", 0, 0);
  Var x1("livello", 0, 0);
  Var g("Edificio", 1, 0);
  Var g1("Edificio", 1, 0);
  Var h("haLivello", 3, 0);
  Var h1("haLivello", 3, 0);
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
  Atom atom(0, {&h,&b,&x});
  Atom atom2(0, {&g,&b});
  Atom atom3(1, { &b,&x });
  cout << "Testing pring atom:" << endl;
  cout << atom.print() << endl;
  cout << atom2.print() << endl;
  cout << atom3.print() << endl;
  cout << "Atom print: " <<  atom.getElementAt(0)->getName() << endl;
  cout << "Atom print: " <<  atom.getElementAt(1)->getName() << endl;
  if( ((atom.getElementAt(2)))==NULL)
       cout << "Atom print: NULL " << endl;
  Tableau tab( &Node() ); //empty tableau
  Node* radix=tab.getTableau();
  cout << "Stack" << endl;  
  Formula final(NULL,5);
  string formula = " ($FA V0{z})( (V0{k} $NI V1{l}) $AD  ( ( V0{z} $NI V1{C1})$OR ( V0{z1} $NI V1{C2}))$AD((  $OA V1{z} $CO V1{z1} $AO $NI V1{C2})$OR (V0{z1} $IN V1{C2}))) ";
	 // ($OA V0{yyy} $CO V0{xxx} $AO $NI V3{C333})";  
  parseInternalFormula(&formula, &final);
  logFile.close();	
  cout << "Check insertSetVar" << endl;
  
 /* Var* ddd = insertVar("ciao", 0, 0);
  Var* daa = insertVar("CAAAAAAo", 0, 0);
  Var* dad = insertVar("ChhhhhhAo", 0, 0);
  Atom at = Atom(1, { ddd,daa,dad });
  cout << ddd->print() << endl; */
  cout << "Check VVL" << endl;  
  //insertVar( *createNewVar("V0{CIAO}"));
  //insertVar(*createNewVar("V0{test}"));
  //insertVar(*createNewVar("V1{1CIAO}"));
  //insertVar(*createNewVar("V3{3CIAO}"));
  cout << "Vector 0" << endl;
  printVector(VVL.at(0));
  cout << "Vector 1" << endl;
  printVector(VVL.at(1));
  cout << "Vector 3" << endl;
  printVector(VVL.at(3));

  cout << "Check VQL" << endl;  
  cout << "Vector 0" << endl;
  printVector(VQL.at(0));
  cout << "Vector 1" << endl;
  printVector(VQL.at(1));
  cout << "Vector 3" << endl;
  printVector(VQL.at(3));


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

USE A MAP to MAP from string representing operator and integer representing operator.
Check if an atom is built correctly.
check brackets; check format of a formula in general as preprocessing
define special chars from a config file. Setting the size of the special chars and checking for correctness.
allowing change of the $ char from a config file.

Optimize Atom managment and creation

*/
