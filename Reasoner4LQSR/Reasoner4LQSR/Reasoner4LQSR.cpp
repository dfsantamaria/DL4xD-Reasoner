// Reasoner4LQSR.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>

//#define debug

std::ofstream out;

int main()
{
  std::ofstream out("LOG.log");
  #ifdef debug
   out << "input2";	
  #endif // debug

  out.close();	
  return 0;
}

