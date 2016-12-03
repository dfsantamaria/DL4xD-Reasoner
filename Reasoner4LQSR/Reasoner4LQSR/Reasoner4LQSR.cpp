// Reasoner4LQSR.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
//#define debug
std::ofstream logFile;   //log file

class Entity
 {
 protected: string name; int *priority;
   public: virtual int getPriority()=0;
   Entity(string inname) { name = inname; };
   void setName(char *inname) { name=inname; }
   string getName() { return name; }
 };

class L0Var : Entity {};


class L0VarConst : Entity
{
  public: int getPriority() { return 1; }; 
  L0VarConst(string inname): Entity(inname){}
};

class L0VarBound : Entity
{
  public: int getPriority() { return 2; };
  L0VarBound(string inname) : Entity(inname) {}
};

int main()
{
  std::ofstream logFile("LOG.log");
  #ifdef debug  
  #endif // debug
    L0VarBound b ("ciao");
	
  cout << b.getPriority();
  logFile.close();	
  return 0;
}

