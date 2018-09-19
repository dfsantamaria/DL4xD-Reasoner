#pragma once
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <array>
#include <stack>
#include <sstream>
#include "thirdparts/pugixml/src/pugixml.hpp"

using namespace std;
string retrieveNameFromNode(pugi::xml_node_iterator const& it, string const& attribute);
string retrieveVarNameFromNode(pugi::xml_node_iterator const& it, string const& attribute, int const& var);
string retrieveZVariable(int num);
int parseDeclaration(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int readOWLXMLOntology(std::string filename, vector<pair<std::string, std::string>>& ontNamespaces, vector<string>& formulae, vector<int>& KBsize);
void buildNamespace(pugi::xml_document& xmldocument, vector<pair<std::string, std::string>>& ontNamespaces);
int parseClassAssertion(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseObjectPropertyAssertion(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseNegativeObjectPropertyAssertion(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseSameIndividual(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseDifferentIndividuals(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseFunctionalObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseInverseFunctionalObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseReflexiveObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseIrreflexiveObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseTransitiveObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseSymmetricObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseAsymmetricObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseInverseObjectProperties(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseEquivalentObjectProperties(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseDisjointObjectProperties(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseSubObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);

int parseSubClassOfExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseEquivalentClassExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseObjectIntersectionOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int& exprtype);
int parseClassExpression(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int& exprtype);
int parseObjectUnionOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int& exprtype);
int parseObjectPropertyDomain(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int &exprtype);
int parseObjectPropertyRange(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int& exprtype);
int parseObjectPropertyDomainExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseObjectPropertyRangeExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseObjectComplementOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount,  int& exprtype);
int parseDisjointClasses(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount,  int& exprtype);
int parseObjectOneOf(string& entry, pugi::xml_node_iterator& it, int varz);
int parseObjectHasSelf(string& entry, pugi::xml_node_iterator& it, int varz);
int parseObjectHasValue(string& entry, pugi::xml_node_iterator& it, int varz);
int parseObjectSomeValuesFrom(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int& exprtype);
int parseObjectAllValuesFrom(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int& exprtype);

int parseObjectExactCardinality(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
int parseObjectMinCardinality(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int& exprtype);
int parseObjectMaxCardinality(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int& exprtype);

int parseDisjointUnion(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount, int& exprtype);
int parseDisjointUnionExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);

int parseDLSafeBody(string& entry, pugi::xml_node_iterator& it, vector<string>& varSet);
int parseDLSafeRule(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
int parseDLAtom(string& atom, pugi::xml_node_iterator& node, vector<string>& varSet, string name, int vt);
 
