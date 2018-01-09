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
void parseDeclaration(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void readOWLXMLOntology(std::string filename, vector<pair<std::string, std::string>>& ontNamespaces, vector<string>& formulae, vector<int>& KBsize);
void buildNamespace(pugi::xml_document& xmldocument, vector<pair<std::string, std::string>>& ontNamespaces);
void parseClassAssertion(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseObjectPropertyAssertion(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseNegativeObjectPropertyAssertion(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseSameIndividual(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseDifferentIndividuals(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseFunctionalObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseInverseFunctionalObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseReflexiveObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseIrreflexiveObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseTransitiveObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseSymmetricObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseAsymmetricObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseInverseObjectProperties(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseEquivalentObjectProperties(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseDisjointObjectProperties(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseSubObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);

void parseSubClassOfExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseEquivalentClassExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseObjectIntersectionOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
void parseClassExpression(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
void parseObjectUnionOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
void parseObjectPropertyDomain(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
void parseObjectPropertyRange(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
void parseObjectPropertyDomainExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseObjectPropertyRangeExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize);
void parseObjectComplementOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
void parseDisjointClasses(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
void parseObjectOneOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
void parseObjectHasSelf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount);
void parseObjectHasValue(string& entry, pugi::xml_node_iterator& it, int varz,int& varcount);


void parseObjectSomeValuesFrom(string& entry, pugi::xml_node_iterator& it, int varz);
void parseObjectAllValuesFrom(string& entry, pugi::xml_node_iterator& it, int varz);
void parseObjectMinCardinality(string& entry, pugi::xml_node_iterator& it, int varz);
void parseObjectMaxCardinality(string& entry, pugi::xml_node_iterator& it, int varz);
void parseObjectExactCardinality(string& entry, pugi::xml_node_iterator& it, int varz);
void parseDisjointUnion(string& entry, pugi::xml_node_iterator& it, int varz);

