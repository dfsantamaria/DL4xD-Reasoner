// Reasoner4LQSR.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <array>

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
const int maxVarSize = 3;
const int maxAtLen = 3;
const int maxOpLen = 4;

 class Var
  {
   private: 
		    int type;
			int var;
			string name;
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

 // vector of 4LQSR Variables. V stays for variable, C for constant.
 vector< vector <Var> > VCL;
 vector< vector <Var> > VVL;

 class Formula
  {
 
  };

 class Node
  {
   private: vector<Formula> setFormula;
  };

 class Atom
 {
   private:  
	       
		   int atomOp;		  
		    /*
			   -1 unsetted; 0 \in ; 1 \notin; 2 \equals; 3 \notequals 
			*/
		   vector<Var*> components;		   
   public: 
	      Atom(int size, int op, vector<Var*> vec) 
           {
			  components.reserve(size);
			  setAtomOp(op);
			  components = vec;
		  };		  
		 
		  vector<Var*> getElements()
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
			if ( index < components.size())			  
			 return components.at(index);
			return NULL;
			  
		  }
		 /* void addElement(Var& element)
		  {
			  getElements().push_back(&element);
		  };
		  */
 };
// Implementing Tableau as vector. Use index*2 for left child and index*2+1 for right
vector<Node> Tableau;

int init()
 {
  VCL.reserve(maxVarSize);
  VVL.reserve(maxVarSize);		
  for (int i = 0; i < maxVarSize; i++)
   {
	VCL.push_back(vector<Var>());
	VVL.push_back(vector<Var>());
   }
  return 0;
}

int addNewElement(Var& element, vector < vector <Var>>& evector)
 {	
  int type = element.getType();
  if (element.isValidType()&&element.isValidVar())
	 (evector.at(type)).push_back(element);
  return type;
 }

int addNewConst(Var& element)
 {
  return (addNewElement(element, VCL));
 }

int addNewVar(Var& element)
 {
  return (addNewElement(element, VVL));
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
  
  cout << "Testing addNewConst: " << addNewConst(b)<<endl;
  cout<< "--------------------  "<< VCL.at(0).capacity()<<endl;
  cout << "-------------------- " << VCL.at(0).at(0).getName() << endl;
 // cout<<addNewElement(x, VCL)<<endl;
  //cout << VCL.at(0).at(0).getName() << endl;
  cout << VCL.capacity()<< endl;
  Atom atom(2, 0, {&b,&h});
  cout << "Atom print: " <<  (*(atom.getElementAt(0))).getName() << endl;
  cout << "Atom print: " << (*(atom.getElementAt(1))).getName() << endl;
  if( ((atom.getElementAt(2)))==NULL)
       cout << "Atom print: NULL " << endl;

  logFile.close();	
  return 0;
}

