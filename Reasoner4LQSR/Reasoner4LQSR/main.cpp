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
	vector<Formula> KB;
	vector<Formula> expKB;
	//	insertFormulaKB("( ($OA V0{l} $CO V0{j} $AO $IN V3{C333})  $AD (  ($OA V0{k} $CO V0{t} $AO $IN V3{C333}) $OR ($OA V0{s} $CO V0{v} $AO $IN V3{C333}) ) )", KB);
	string kbname = "Example/bg2.txt";
	readKBFromFile(0, kbname, KB);

	//print Tableau Radix	
	printTRadix(KB);

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


	/* Query Reading*/
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
	/*
	for (Formula f : querySet)
	cout << f.toString() << endl;
	*/

	/*
	vector<Formula> KB2; int typeformula = 0;
	//insertFormulaKB(varSet.getVQL(), varSet.getVVL(),
	//	"($FA V0{k}) ( (V0{k} $NI V1{C2})  $OR (  (  (V0{k} $NI V1{l}) $AD ( V0{k} $NI V1{C1}) ) $OR (V0{k} $NI V1{C3})))", KB2, &typeformula);

	insertFormulaKB(varSet.getVQL(), varSet.getVVL(),
	"($FA V0{z}) ($FA V1{z1})(( (V0{Annetta} $IN V1{Person}) $AD (V0{z} $IN V1{VeryYoung})) $OR (V0{z} $IN V1{z1}))", KB2, &typeformula);

	cout << KB2.at(0).toString() << endl;
	Formula* out= copyFormula(&KB2.at(0), NULL);
	KB2.clear();
	convertToCNF(out);
	cout << out->toString() << endl;
	*/
	vector<pair<string, string>> ontNamespaces;	
	vector<string> formulae;
	vector<Formula> KB2;
	cout << "Reading OWL File" << endl;
	readOWLXMLOntology("Example/yy.owl", ontNamespaces, formulae);
	cout << "Printing ontology namespaces" << endl;
	for (pair<string, string> p : ontNamespaces)
		cout << p.first << " " << p.second << endl;
	cout << "Printing formulae: " << formulae.size()<< endl;
	for (string p : formulae)
		cout << p << endl;
	readKBFromStrings(0, formulae, KB2);
	printTRadix(KB2);
	debugEnd();
	return 0;
}
