// Reasoner4LQSR.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <typeinfo>

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
 class Var
  {
   private: 
		    int type;
			int min = -1;
			int max = 5;
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



int main()
{
  std::ofstream logFile("LOG.log");
  #ifdef debug  
  #endif // debug
  Var b ("ciao", 0);
  cout << b.getName() << b.getType(); 
  logFile.close();	
  return 0;
}

