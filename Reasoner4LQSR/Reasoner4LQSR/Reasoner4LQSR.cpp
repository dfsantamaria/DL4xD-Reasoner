// Reasoner4LQSR.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>

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
int min = -1;
int max = 5;

 class Var
  {
   private: 
		    int type;
			
			string name;
			int isValid(int _type) { return _type > min && _type < max; };
   public:
		   Var(string _name, int _type)
		    {
			   name = _name;
			   if (isValid(_type))
				   type = _type;
			   else
				   type = -1;
		    }
		   int getType() { return type; };
		   string getName() { return name; };
		   int setName(string _name) 
		      { 
			    name = _name; 
			    return type; 
		      };
		   int setType(int _type)
		      { 
			   if (isValid(_type))
			     type = _type;			    
			   else			     
				  type = -1; 
			   return type;			   
		      };
		   
  };

 // vector of 4LQSR Variables. V stays for variable, C for constant.
 vector<Var> VCL0;
 vector<Var> VCL1;
 vector<Var> VCL2;
 vector<Var> VCL3;
 vector<Var> VVL0;
 vector<Var> VVL1;
 vector<Var> VVL2;
 vector<Var> VVL3;

 class Formula
 {
 
 };
 class Formulae
 {
 private: vector<Formula> SetFormula;

 };

// Implementing Tableau as vector. Use index*2 for left child and index*2+1 for right
vector<Formulae> Tableau;

int main()
{
  std::ofstream logFile("LOG.log");
  #ifdef debug  
  #endif // debug
  Var b ("monastero", 0);
  cout << b.getName() << b.getType(); 
  logFile.close();	
  return 0;
}

