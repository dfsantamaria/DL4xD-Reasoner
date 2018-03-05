#pragma once
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
#include <chrono>


#define propertyindex 3
#define qvar0 (propertyindex+1)
#define maxLevNum 3
#define sizeofVVector (maxLevNum+maxLevNum+2) 

std::fstream logFile;

void debugStart()
{
	logFile.open("LOG.log", fstream::out);
#ifdef debug  
	logFile << "Debug Started" << endl;
#endif // debug
};

void debugEnd()
{
	logFile.close();
};

int main()
{
	debugStart();

	/*
	Initialization
	*/
	//If you don't read an OWL File, make sure that quantified and quantifier free var vectors are sufficiently large.
	//InitializeReasoner(3, 100, 20);

	InitializeReasoner(maxLevNum, vector<int>{5, 5, 0, 5, 5, 5, 0, 5});

	/*
	Inserting Knowledge Base
	*/


	vector<Formula*> KB;
	vector<Formula*> KBnorm;
	vector<Formula*> KBcnf;
	vector<Formula*> KBmq;
	string kbname = "Tester/tester9.txt";

	/*Precomputing Space -- to be implemented*/
	//  to be implemented
	//	vector<int>KBsize(8, 0);
	//	vector<int>Litsize(2, 0);
	//	precomputeKBSpaceFromFile(kbname, KBsize, Litsize);
	/*  */


	cout << "--Reading KB From File--" << endl;
	readKBFromFile(0, kbname, KB);
	printTRadix(KB); //print Tableau Radix
	cout << "--Converting the KB to NNF --" << endl;
	convertKBToNNF(KB, KBnorm);
	KB.clear();
	printTRadix(KBnorm); //print Tableau Radix	
	cout << "--Converting Formulae to CNF--" << endl;
	convertKBToCNF(KBnorm, KBcnf);
	KBnorm.clear();
	printTRadix(KBcnf); //print Tableau Radix
	cout << "--Move Quantifiers--" << endl;
	moveQuantifierKB(0, KBcnf, KBmq);
	//printTRadix(KBmq); //print Tableau Radix

	vector<Formula*> expKB;
	cout << "Expanding Quantifiers in KB" << endl;

	auto started = std::chrono::high_resolution_clock::now();

	expandKB(KBmq, expKB);
	Tableau tableau = Tableau(new Node(expKB));
	//Printing result of expansion
	printTExpanded(tableau);
	//Printing content of VVL and VQL
	
	cout << "Expanding Tableau" << endl;
	expandTableau(tableau);
	cout << "Building EqSet" << endl;	

	auto done = std::chrono::high_resolution_clock::now();
	std::cout << "Milliseconds Execution: " << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << endl;

    //printVarSet();
	//print EqSet
	//printEqSet(tableau);

	//print open branches
	//printOpenBranches(tableau);

	//print closed branches
	//printClosedBranches(tableau);


	cout << "Open branches: " << tableau.getOpenBranches().size() << endl;
	cout << "Closed branches: " << tableau.getClosedBranches().size() << endl;
	cout << "Total branches: " << tableau.getOpenBranches().size() + tableau.getClosedBranches().size() << endl;

	debugEnd();
}
