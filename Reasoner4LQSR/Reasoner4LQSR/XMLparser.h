#pragma once
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <array>
#include <stack>
#include <sstream>
#include "thirdparts/pugixml/src/pugixml.hpp"

void readOWLXMLOntology(std::string filename);
void buildNamespace(pugi::xml_document& xmldocument);