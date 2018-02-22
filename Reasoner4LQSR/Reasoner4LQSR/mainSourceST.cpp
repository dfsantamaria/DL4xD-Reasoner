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

	InitializeReasoner(maxLevNum, vector<int>{20,20,0,20,10,10,0,10});

	/*
	Inserting Knowledge Base
	*/

	
	vector<Formula*> KB;
	vector<Formula*> KBnorm;
	vector<Formula*> KBcnf;
	vector<Formula*> KBmq;	
	string kbname = "Example/ex1.txt"; 

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
	convertKBToCNF(KBnorm,KBcnf);
	KBnorm.clear();
	printTRadix(KBcnf); //print Tableau Radix
	cout << "--Move Quantifiers--" << endl;
	moveQuantifierKB(0, KBcnf, KBmq);	
	printTRadix(KBmq); //print Tableau Radix

	vector<Formula*> expKB;
	cout << "Expanding Quantifiers in KB" << endl;
	expandKB(KBmq, expKB); 	
	Tableau tableau = Tableau(new Node(expKB));
	//Printing result of expansion
	printTExpanded(tableau);
	//Printing content of VVL and VQL
	printVarSet();
	cout << "Expanding Tableau" << endl;
	expandTableau(tableau);
	cout << "Building EqSet" << endl;
	buildEqSet(tableau);

	//print EqSet
	printEqSet(tableau);

	//print open branches
	printOpenBranches(tableau);

	//print closed branches
	printClosedBranches(tableau);

	/* Query Reading*/

	
	cout << "---" << endl;
	cout << "Reading Query ..." << endl;
	string queryname = "Example/query.txt";
	vector<Formula> querySet;
	vector<string> stringSet = vector<string>(0);	

	readQueryFromFile(queryname, stringSet);
	vector<QueryManager*> results;
	performQuerySet(results, stringSet, querySet, tableau);

	cout << "Printing query results ..." << endl;
	for (int i = 0; i < results.at(0)->getMatchSet().second.size(); i++)
	{
		cout << "Tableau branch number: " << results.at(0)->getMatchSet().first.at(i) << endl;
		for (int j = 0; j < results.at(0)->getMatchSet().second.at(i).size(); j++)
		{
			cout << "Solution number: " << j << endl;
			for (int k = 0; k < results.at(0)->getMatchSet().second.at(i).at(j).size(); k++)
			{
				cout << results.at(0)->getMatchSet().second.at(i).at(j).at(k).first->toString();
				cout << ",";
				cout << results.at(0)->getMatchSet().second.at(i).at(j).at(k).second->toString() << "; ";
			}
			cout << endl;
		}
	}
	cout << "Printing Y/N results ..." << endl;
	for (int i = 0; i < results.at(0)->getAnswerSet().size(); i++)
	{
		cout << "Branch number: " << i << " Answer:" << results.at(0)->getAnswerSet().at(i) << endl;
	}
	
	

	debugEnd();
}
