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
void parseSameIndividual(std::string& entry, pugi::xml_node_iterator& it);