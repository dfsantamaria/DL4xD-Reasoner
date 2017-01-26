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

using namespace std;
//#define debug
std::ofstream logFile;   //log file

/*
-1 is unused
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
  };

 // vector of 4LQSR Variables. Q stays for quantified.
 vector< vector <Var> > VVL;
 vector< vector <Var> > VQL;

 class Atom
 {
   private:  	       
		   int atomOp;		  
		    /*
			   -1 unsetted; 0 \in ; 1 \notin; 2 \equals; 3 \notequals 
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
 };

 class Formula  //struttura più generica da cambiare eventualmente con strutture per CNF
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
  for (int i = 0; i < maxVarSize; i++)
   {
	VQL.push_back(vector<Var>());
	VVL.push_back(vector<Var>());
   }
  return 0;
}
/*
  Add an element to the given vector of the vars
*/
int addNewElement(Var& element, vector < vector <Var>>& evector)
 {	
  int type = element.getType();
  if (element.isValidType()&&element.isValidVar())
	 (evector.at(type)).push_back(element);
  return type;
 }

/*
Add an element to the vector of quantified variable
*/

int addNewQuantifiedVar(Var& element)
 {
  return (addNewElement(element, VQL));
 }

/*
Add an element to the vector of (unquantified variables
*/
int addNewVar(Var& element)
 {
  return (addNewElement(element, VVL));
 }
/*
Parse a string representing an internal formula and return the corresponding internal formula
*/
int parseInternalFormula(string *inputformula, Formula *outformula)
{
	string strformu = *inputformula;
	stack<string> stackFormula;
	stackFormula.push(strformu);
	outformula->setOperand(32);
	cout << stackFormula.top() << endl;
	return 0;
}

int main()
{  
  int res=init();
  std::ofstream logFile("LOG.log");
  #ifdef debug  
  #endif // debug
  Var b ("monastero", 0, 0);
  Var x("cortile", 1, 0);
  Var g("boh", 2, 1);
  Var h("haLivello", 3, 0);
  cout << b.getName() <<","<< b.getType() <<","<< b.getVarType() << endl; 
  cout << x.getName() << "," << x.getType() << "," << x.getVarType() << endl;
  cout << g.getName() << "," << g.getType() << "," << g.getVarType() << endl;
  cout << h.getName() << "," << h.getType() << "," << h.getVarType() << endl;  
  cout << "Testing addNewConst: " << addNewQuantifiedVar(b)<<endl;
  cout<< "--------------------  "<< VQL.at(0).capacity()<<endl;
  cout << "-------------------- " << VQL.at(0).at(0).getName() << endl;
 // cout<<addNewElement(x, VCL)<<endl;
  //cout << VCL.at(0).at(0).getName() << endl;
  cout << VQL.capacity()<< endl;
  Atom atom(1, {&b,&h});
  cout << "Atom print: " <<  atom.getElementAt(0)->getName() << endl;
  cout << "Atom print: " <<  atom.getElementAt(1)->getName() << endl;
  if( ((atom.getElementAt(2)))==NULL)
       cout << "Atom print: NULL " << endl;
  Tableau tab( &Node() ); //empty tableau
  Node* radix=tab.getTableau();
  cout << "Stack" << endl;
  Formula final(NULL,5);
  string formula = "formula";
  
  parseInternalFormula(&formula, &final);
  cout << "Formula after: " << final.getOperand() << endl;
  logFile.close();	
  return 0;
}

/*
Mapping from 4LQSR formulae in semi-internal formulae
\FA -> ForAll
\IN -> IN
\NI -> NOTIN
\EQ -> =
\QE -> NOT =
\OA -> <
\AO -> >
\AD -> AND
\DA -> NOT AND
\OR -> OR
\RO -> NOT OR
( ) -> ( )
Vi{name} -> X^i_{name} variable

Example of formula

(\FA V0{z})(( ( V0{z} \NI V1{C1})\OR ( V0{z} \NI V1{C2}))\AN(( V0{z} \NI V1{C2})\OR (\V0{z} \IN V1{C2}))) 

Is also allowed the following name convention V1{x_b_3}
that stays for X^1_{ {x_{b_3} }
*/