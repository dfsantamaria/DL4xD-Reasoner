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
void parseEquivalentClassExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectIntersectionOf(string& entry, pugi::xml_node_iterator& it, string varz, int varcount);
void parseClassExpression(string& entry, pugi::xml_node_iterator& it, string varz, int varcount);

void parseObjectUnionOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectComplementOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectOneOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectSomeValuesFrom(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectAllValuesFrom(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectHasValue(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectHasSelf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectMinCardinality(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectMaxCardinality(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectExactCardinality(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseSubClassOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseEquivalentClasses(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseDisjointClasses(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseDisjointUnion(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectPropertyChain(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectPropertyDomain(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
void parseObjectPropertyRange(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize);
