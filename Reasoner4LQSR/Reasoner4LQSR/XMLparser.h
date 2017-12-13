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

void parseDeclaration(vector<std::string>& entry, pugi::xml_node_iterator& it);
void readOWLXMLOntology(std::string filename, vector<pair<std::string, std::string>>& ontNamespaces, vector<string>& formulae);
void buildNamespace(pugi::xml_document& xmldocument, vector<pair<std::string, std::string>>& ontNamespaces);
void parseClassAssertion(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseObjectPropertyAssertion(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseNegativeObjectPropertyAssertion(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseSameIndividual(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseDifferentIndividuals(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseFunctionalObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseInverseFunctionalObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseReflexiveObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseIrreflexiveObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseTransitiveObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseSymmetricObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseAsymmetricObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseInverseObjectProperties(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseEquivalentObjectProperties(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseDisjointObjectProperties(vector<std::string>& entry, pugi::xml_node_iterator& it);
void parseSubObjectProperty(vector<std::string>& entry, pugi::xml_node_iterator& it);

void parseObjectInserctionOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectUnionOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectComplementOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectOneOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectSomeValuesFrom(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectAllValuesFrom(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectHasValue(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectHasSelf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz); 
void parseObjectMinCardinality(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectMaxCardinality(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectExactCardinality(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseSubClassOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseEquivalentClasses(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseDisjointClasses(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseDisjointUnion(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectPropertyChain(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectPropertyDomain(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseObjectPropertyRange(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz);
void parseClassExpression(vector<std::string>& entry, pugi::xml_node_iterator& it);