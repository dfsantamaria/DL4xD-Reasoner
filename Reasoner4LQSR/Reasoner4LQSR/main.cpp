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
	InitializeReasoner(3, 100, 20);
	/*
	Inserting Knowledge Base
	*/
	vector<Formula*> KB;
	vector<Formula*> KBnorm;
	vector<Formula*> KBcnf;
	//	insertFormulaKB("( ($OA V0{l} $CO V0{j} $AO $IN V3{C333})  $AD (  ($OA V0{k} $CO V0{t} $AO $IN V3{C333}) $OR ($OA V0{s} $CO V0{v} $AO $IN V3{C333}) ) )", KB);
	string kbname = "Example/bg5.txt";
	cout << "--Reading KB From File--" << endl;
	readKBFromFile(0, kbname, KB);
	printTRadix(KB); //print Tableau Radix
	cout << "--Normalizing KB--" << endl;
	normalizeKB(KB, KBnorm);
	KB.clear();
	printTRadix(KBnorm); //print Tableau Radix	
	cout << "--Convert Formulae in CNF--" << endl;
	converKBToCNF(KBnorm,KBcnf);
	KBnorm.clear();
	printTRadix(KBcnf); //print Tableau Radix

	cout << "Testing" << endl;
	cout << KBcnf.at(0)->toString() << endl;
	cout << KBcnf.at(0)->getRSubformula()->getRSubformula()->toString() << endl;
	cout << KBcnf.at(0)->getRSubformula()->getRSubformula()->getPreviousformula()->getPreviousformula()->toString() << endl;


	/*vector<Formula> expKB;
	cout << "Expanding Quantifiers in KB" << endl;
	expandKB(KB, expKB); 
	Tableau tableau = Tableau(new Node(expKB));

	//Printing result of expansion
	printTExpanded(tableau);

	//Printing content of VVL and VQL
	printVarSet();

	//cout << "Clash before tableau expansion:" << checkNodeClash(tableau.getTableau()->getSetFormulae()) << endl;

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

*/
	/* Query Reading*/
	/*
	cout << "---" << endl;
	cout << "Reading Query ..." << endl;
	string queryname = "Example/query.txt";
	vector<Formula> querySet;
	vector<string> stringSet = vector<string>(0);
	readQueryFromFile(queryname, stringSet);
	//vector<Atom*> qAtoms;  
	//cout << stringSet.at(0) << endl;
	vector<QueryManager> results;
	performQuerySet(results, stringSet, querySet, tableau);
	cout << "Printing query results ..." << endl;
	for (int i = 0; i < results.at(0).getMatchSet().second.size(); i++)
	{
		cout << "Tableau branch number: " << results.at(0).getMatchSet().first.at(i) << endl;
		for (int j = 0; j < results.at(0).getMatchSet().second.at(i).size(); j++)
		{
			cout << "Solution number: " << j << endl;
			for (int k = 0; k < results.at(0).getMatchSet().second.at(i).at(j).size(); k++)
			{
				cout << results.at(0).getMatchSet().second.at(i).at(j).at(k).first->toString();
				cout << ",";
				cout << results.at(0).getMatchSet().second.at(i).at(j).at(k).second->toString() << "; ";
			}
			cout << endl;
		}
	}
	cout << "Printing Y/N results ..." << endl;
	for (int i = 0; i < results.at(0).getAnswerSet().size(); i++)
	{
		cout << "Branch number: " << i << " Answer:" << results.at(0).getAnswerSet().at(i) << endl;
	}
	*/

	
	vector<pair<string, string>> ontNamespaces;
	vector<string> formulae;
	vector<Formula*> KB2;
	vector<Formula*> KB2norm;
	cout << "Reading OWL File" << endl;
	readOWLXMLOntology("Example/y2.owl", ontNamespaces, formulae);
	cout << "Printing ontology namespaces" << endl;
	for (pair<string, string> p : ontNamespaces)
		cout << p.first << " " << p.second << endl;
	cout << "Printing formulae: " << formulae.size() << endl;
	for (string p : formulae)
		cout << p << endl;
	cout << "--Reading Ontology--" << endl;
	readKBFromStrings(0, formulae, KB2);
	printTRadix(KB2);
	cout << "--Normalizing KB2--" << endl;
	normalizeKB(KB2, KB2norm);
	KB.clear();
	printTRadix(KB2norm); //print Tableau Radix	

	debugEnd();
	return 0;
}
