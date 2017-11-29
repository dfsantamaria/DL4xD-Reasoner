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
#include "XMLparser.h"
#include "thirdparts/pugixml/src/pugixml.hpp"

using namespace std;


void buildNamespace(pugi::xml_document& xmldocument, vector<pair<string, string>>& ontNamespaces)
{	
 for (pugi::xml_node_iterator it = xmldocument.begin(); it != xmldocument.end(); ++it)
	{
	 for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
		{
			string name = ait->name();
			string value = ait->value();
			if (name == "xml:base")
				ontNamespaces.insert(ontNamespaces.begin(), make_pair(name, value));
			else
				ontNamespaces.push_back(make_pair(name, value));
		}		
	}	
};

void parseDeclaration(std::string& entry, pugi::xml_node_iterator& it)
{
	pugi::xml_node node = it->first_child();
	string name = node.name();
	string irival = node.attribute("IRI").as_string();
	if (name=="Class")	
	 entry = "V1{" + irival.substr(irival.find("#") + 1) + "}";
	else if (name=="ObjectProperty")
	 entry = "V3{" + irival.substr(irival.find("#") + 1) + "}";
	else if (name=="NamedIndividual")
	 entry = "V0{" + irival.substr(irival.find("#") + 1) + "}";
};

void readOWLXMLOntology(string filename, vector<pair<string, string>>& ontNamespaces, vector<string>& formulae)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.data());
	//building namespace
	buildNamespace(doc, ontNamespaces);
	//std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
	pugi::xml_node tools = doc.first_child();
	
	for (pugi::xml_node_iterator it = tools.begin(); it != tools.end(); ++it)
	{
		string entry = "";		
		string name = it->name();
		if (name == "Declaration")
		{
			parseDeclaration(entry, it);
		}
		 
		if (!entry.empty())
			formulae.push_back(entry);
		
		//cout << irival << endl;
		//cout << irival.substr(irival.find("#") + 1) << endl;
	}

};

