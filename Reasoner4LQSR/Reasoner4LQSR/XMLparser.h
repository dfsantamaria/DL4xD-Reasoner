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

void parseDeclaration(std::string& entry, pugi::xml_node_iterator& it);
void readOWLXMLOntology(std::string filename, vector<pair<std::string, std::string>>& ontNamespaces, vector<string>& formulae);
void buildNamespace(pugi::xml_document& xmldocument, vector<pair<std::string, std::string>>& ontNamespaces);
void parseClassAssertion(std::string& entry, pugi::xml_node_iterator& it);
void parseObjectPropertyAssertion(std::string& entry, pugi::xml_node_iterator& it);
void parseNegativeObjectPropertyAssertion(std::string& entry, pugi::xml_node_iterator& it);
void parseSameIndividual(std::string& entry, pugi::xml_node_iterator& it);
void parseDifferentIndividuals(std::string& entry, pugi::xml_node_iterator& it);
void parseFunctionalObjectProperty(std::string& entry, pugi::xml_node_iterator& it);
void parseInverseFunctionalObjectProperty(std::string& entry, pugi::xml_node_iterator& it);
void parseReflexiveObjectProperty(std::string& entry, pugi::xml_node_iterator& it);
void parseIrreflexiveObjectProperty(std::string& entry, pugi::xml_node_iterator& it);
void parseTransitiveObjectProperty(std::string& entry, pugi::xml_node_iterator& it);
void parseSymmetricObjectProperty(std::string& entry, pugi::xml_node_iterator& it);
void parseAsymmetricObjectProperty(std::string& entry, pugi::xml_node_iterator& it);
void parseInverseObjectProperties(std::string& entry, pugi::xml_node_iterator& it);