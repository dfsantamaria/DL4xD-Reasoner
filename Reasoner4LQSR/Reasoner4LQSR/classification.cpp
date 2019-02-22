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
std::fstream testFile;

void debugStart()
{
	logFile.open("LOG/LOG.log", fstream::out);
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
	    OWL/XML Reader
	*/

	vector<pair<string, string>> ontNamespaces;
	vector<string> formulae;
	vector<Formula*> KB2;
	vector<Formula*> KB2norm;
	vector<Formula*> KB2cnf;
	vector<Formula*> KB2mq;
	vector<int> KB2size(sizeofVVector,0);
	cout << "Reading OWL File" << endl; 
	
	int chk=readOWLXMLOntology("Example/ceramic.owl", ontNamespaces, formulae, KB2size);
	if (chk == -1)
	{
		cout << "Ontology not supported" << endl;
		return -1;
	}
	InitializeReasoner(maxLevNum, KB2size);
	//InitializeReasoner(3, 100, 20);
	cout << "Printing ontology namespaces" << endl;
	/*for (pair<string, string> p : ontNamespaces)
		cout << p.first << " " << p.second << endl;
	cout << "Printing formulae: " << formulae.size() << endl;
	for (string p : formulae)
		cout << p << endl;*/
	cout << "Metrics of the Ontology: " << endl;
	cout << "Number of formulae: " << formulae.size() << endl;
	cout << "Individuals: " << KB2size.at(0) << "." << endl;
	cout << "Classes: " << KB2size.at(1) << "." << endl;
	cout << "Properties: " << KB2size.at(propertyindex) << "." << endl;
	cout << "Quantified Variables of level 0: " << KB2size.at(qvar0) << "." << endl;
	cout << "--Reading Ontology--" << endl;	
	
	readKBFromStrings(0, formulae, KB2);
//	printTRadix(KB2);
	cout << "--Converting KB2 to NNF--" << endl;
	convertKBToNNF(KB2, KB2norm);
	KB2.clear();
	//printTRadix(KB2norm); //print Tableau Radix	
	cout << "--Convert Formulae in CNF--" << endl;
	convertKBToCNF(KB2norm, KB2cnf);
	KB2norm.clear();
	//printTRadix(KB2cnf); //print Tableau Radix
	cout << "--Move Quantifiers--" << endl;
	moveQuantifierKB(0, KB2cnf, KB2mq);
//	printTRadix(KB2mq); //print Tableau Radix
	
	//vector<Formula*> expKB2;
	//cout << "Expanding Quantifiers in KB" << endl;
	//expandKB(KB2mq, expKB2);
	Tableau tableau2 = Tableau(new Node(KB2mq));
	//Printing result of expansion
	//printTExpanded(tableau2);
	//Printing content of VVL and VQL
	
	cout << "Computing Taxonomy " << endl;
	auto started = std::chrono::high_resolution_clock::now();

	vector<vector<int>> chierarchySUB = vector<vector<int>>(KB2size.at(1));
	vector<vector<int>> rhierarchySUB = vector<vector<int>>(KB2size.at(propertyindex));
	
	vector<vector<int>> chierarchyOUTSUB = vector<vector<int>>(KB2size.at(1));
	vector<vector<int>> rhierarchyOUTSUB = vector<vector<int>>(KB2size.at(propertyindex));
		

	computeSubsumptionGraph(chierarchySUB, rhierarchySUB, KB2mq);    
	computeSubHierarchy(chierarchySUB, chierarchyOUTSUB);
	computeSubHierarchy(rhierarchySUB, rhierarchyOUTSUB);

	vector<vector<int>> chierarchySUP = vector<vector<int>>(KB2size.at(1));
	vector<vector<int>> rhierarchySUP = vector<vector<int>>(KB2size.at(propertyindex));

	vector<vector<int>> chierarchyOUTSUP = vector<vector<int>>(KB2size.at(1));
	vector<vector<int>> rhierarchyOUTSUP = vector<vector<int>>(KB2size.at(propertyindex));

	computeSubsumptionGraph(chierarchySUP, rhierarchySUP, KB2mq);
	computeSuperHierarchy(chierarchySUP, chierarchyOUTSUP);
	computeSuperHierarchy(rhierarchySUP, rhierarchyOUTSUP);

	auto done = std::chrono::high_resolution_clock::now();
	
	printHierarchy(chierarchyOUTSUB, 1, "LOG/exampleSUB.txt");

	printHierarchy(rhierarchyOUTSUB, 3, "LOG/exampleSUB.txt");

	printHierarchy(chierarchyOUTSUP, 1, "LOG/exampleSUP.txt");

	printHierarchy(rhierarchyOUTSUP, 3, "LOG/exampleSUP.txt");

	

	//printClassGraph(chierarchy);
	//printRoleGraph(rhierarchy);
	std::cout << "Milliseconds Execution: " << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << endl;
	
	
	debugEnd();
}
