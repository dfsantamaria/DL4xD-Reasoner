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
	Initialization
	*/
	//If you don't read an OWL File, make sure that quantified and quantifier free var vectors are sufficiently large.
	//InitializeReasoner(3, 100, 20);
/*
	InitializeReasoner(maxLevNum, vector<int>{20,20,0,20,10,0,0,0});
*/
	/*
	Inserting Knowledge Base
	*/

	/*
	vector<Formula*> KB;
	vector<Formula*> KBnorm;
	vector<Formula*> KBcnf;
	vector<Formula*> KBmq;	
	string kbname = "Example/bg8.txt"; */

	/*Precomputing Space -- to be implemented*/
//  to be implemented
//	vector<int>KBsize(8, 0);
//	vector<int>Litsize(2, 0);
//	precomputeKBSpaceFromFile(kbname, KBsize, Litsize);
	/*  */

	/*
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

	/*
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
	
	*/

    auto started = std::chrono::high_resolution_clock::now();





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
	int chk=readOWLXMLOntology("Example/onto1.owl", ontNamespaces, formulae, KB2size);
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
	//printTRadix(KB2);
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
	//printTRadix(KB2mq); //print Tableau Radix
	
	//vector<Formula*> expKB2;
	//cout << "Expanding Quantifiers in KB" << endl;
	//expandKB(KB2mq, expKB2);
	Tableau tableau2 = Tableau(new Node(KB2mq));
	//Printing result of expansion
	//printTExpanded(tableau2);
	//Printing content of VVL and VQL
	
	cout << "Expanding Tableau" << endl;
	expandGammaTableau(tableau2);
	printVarSet();

	/*

	cout << "Building EqSet" << endl;
	buildEqSet(tableau2);

	//print EqSet
	printEqSet(tableau2);

	//print open branches
	printOpenBranches(tableau2);

	//print closed branches
	printClosedBranches(tableau2);	

	cout << "--End reading ontology--" << endl;                       */

	/* Query Reading*/




	/*
	cout << "---" << endl;
	cout << "Reading Query ..." << endl;
	string queryname = "Example/query.txt";
	vector<Formula> querySet;
	vector<string> stringSet = vector<string>(0);

	readQueryFromFile(queryname, stringSet);
	vector<QueryManager*> results;
	performQuerySet(results, stringSet, querySet, tableau2);

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

	*/
	auto done = std::chrono::high_resolution_clock::now();
	std::cout << "Milliseconds Execution: "<< std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count()<<endl;
	cout << "Open Branch: " << tableau2.getOpenBranches().size() << endl;
	cout << "Closed Branch: " << tableau2.getClosedBranches().size() << endl;
	printClosedBranches(tableau2);
	debugEnd();
}
