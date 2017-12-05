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
#include "log.h"

using namespace std;

void buildNamespace(pugi::xml_document& xmldocument, vector<pair<string, string>>& ontNamespaces)
{	
 for (pugi::xml_node_iterator it = xmldocument.begin(); it != xmldocument.end(); ++it)
	{
	 for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
		{
			string name = ait->name();
			string value = ait->value();
#ifdef debug 
#ifdef debugparseXML
			logFile << "-----Namespace found: " << name << " " << value << "." << endl;
#endif
#endif // debug
			if (name == "xml:base")
				ontNamespaces.insert(ontNamespaces.begin(), make_pair(name, value));
			else
				ontNamespaces.push_back(make_pair(name, value));
		}		
	}	
};

void parseDeclaration(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found declaration. " << endl;
#endif
#endif // debug
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

void parseClassAssertion(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found class assertion. " << endl;
#endif
#endif // debug
	for (pugi::xml_node_iterator node = it->begin(); node != it->end(); ++node)
	{
      string name = node->name();
	  string irival = node->attribute("IRI").as_string();
	  if (irival.empty())
		  return;
	  if (name == "Class")
	    entry = "V1{" + irival.substr(irival.find("#") + 1) + "}";
	  else if (name == "NamedIndividual")
	    entry = "V0{" + irival.substr(irival.find("#") + 1) + "} $IN "+entry;
	}	
};

void parseObjectPropertyAssertion(std::string& entry, pugi::xml_node_iterator& it)
{	
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found object-property assertion. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	string irival = node.attribute("IRI").as_string();
	entry = "} $AO $IN V3{" + irival.substr(irival.find("#") + 1) + "}";
	node = node.next_sibling();
	string acc = "$OA V0{";
	irival = node.attribute("IRI").as_string();
	acc.append(irival.substr(irival.find("#") + 1)).append("} $CO V0{");
	node = node.next_sibling();
	irival = node.attribute("IRI").as_string();
	acc.append(irival.substr(irival.find("#") + 1));
	entry = acc + entry;	
};

void parseNegativeObjectPropertyAssertion(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found negative object-property assertion. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	string irival = node.attribute("IRI").as_string();
	entry = "} $AO $NI V3{" + irival.substr(irival.find("#") + 1) + "}";
	node = node.next_sibling();
	string acc = "$OA V0{";
	irival = node.attribute("IRI").as_string();
	acc.append(irival.substr(irival.find("#") + 1)).append("} $CO V0{");
	node = node.next_sibling();
	irival = node.attribute("IRI").as_string();
	acc.append(irival.substr(irival.find("#") + 1));
	entry = acc + entry;
};

void parseSameIndividual(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found same individuals. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	string irival = node.attribute("IRI").as_string();
	entry = "V0{" + irival.substr(irival.find("#") + 1) + "} $EQ";
	node = node.next_sibling();
	irival = node.attribute("IRI").as_string();
	entry = entry + " V0{" + irival.substr(irival.find("#") + 1) + "}";
};

void parseDifferentIndividuals(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found different individuals. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	string irival = node.attribute("IRI").as_string();
	entry = "V0{" + irival.substr(irival.find("#") + 1) + "} $QE";
	node = node.next_sibling();
	irival = node.attribute("IRI").as_string();
	entry = entry + " V0{" + irival.substr(irival.find("#") + 1) + "}";
};

void parseFunctionalObjectProperty(std::string& entry, pugi::xml_node_iterator& it)
{	
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found functional object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();	
	if ( string(node.name()) == "ObjectProperty")
	{	
		string irival = node.attribute("IRI").as_string();        
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z})($FA V0{z1})($FA V0{z2})";
		entry.append("((($OA V0{z} $CO V0{z1} $AO $NI ");
		entry.append(irival);
		entry.append(")$OR($OA V0{z} $CO V0{z2} $AO $NI ");
		entry.append(irival);
		entry.append("))$OR( V0{z1} $EQ V0{z2}))");
	}	
};

void parseInverseFunctionalObjectProperty(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found inverse functional object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{

		string irival = node.attribute("IRI").as_string();		
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z})($FA V0{z1})($FA V0{z2})";
		entry.append("((($OA V0{z} $CO V0{z1} $AO $NI ");
		entry.append(irival);
		entry.append(")$OR($OA V0{z2} $CO V0{z1} $AO $NI ");
		entry.append(irival);
		entry.append("))$OR( V0{z} $EQ V0{z2}))");
	}
};

void parseReflexiveObjectProperty(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found reflexive object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{

		string irival = node.attribute("IRI").as_string();		
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z})";
		entry.append("($OA V0{z} $CO V0{z} $AO $IN ");
		entry.append(irival);
		entry.append(")");
	}
};

void parseIrreflexiveObjectProperty(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found irreflexive object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{

		string irival = node.attribute("IRI").as_string();
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z})";
		entry.append("($OA V0{z} $CO V0{z} $AO $NI ");
		entry.append(irival);
		entry.append(")");
	}
};



void parseTransitiveObjectProperty(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found transitive object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{

		string irival = node.attribute("IRI").as_string();
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z}) ($FA V0{z1}) ($FA V0{z2}) ( (";
		entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
		entry.append(irival);
		entry.append(") $OR ");

		entry.append("($OA V0{z1} $CO V0{z2} $AO $NI ");
		entry.append(irival);
		entry.append(") ) $OR ");

		entry.append("($OA V0{z} $CO V0{z2} $AO $IN ");
		entry.append(irival);
		entry.append("))");
	}
};

void parseSymmetricObjectProperty(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found symmetric object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{ 

		string irival = node.attribute("IRI").as_string();
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z})($FA V0{z1})(";
		entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
		entry.append(irival);
		entry.append(") $OR");
		entry.append("($OA V0{z1} $CO V0{z} $AO $IN ");
		entry.append(irival);
		entry.append("))");
	}
};

void parseAsymmetricObjectProperty(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found asymmetric object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{

		string irival = node.attribute("IRI").as_string();
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z})($FA V0{z1})(";
		entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
		entry.append(irival);
		entry.append(") $OR");
		entry.append("($OA V0{z1} $CO V0{z} $AO $NI ");
		entry.append(irival);
		entry.append("))");

	}
};

void parseInverseObjectProperties(std::string& entry, pugi::xml_node_iterator& it)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found inverse object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{		
		string irival = node.attribute("IRI").as_string();
		irival = irival.substr(irival.find("#") + 1);
		string prop1 = "V3{" + irival + "}";
		node = node.next_sibling();			
		irival = "";  
		if (string(node.name()) == "ObjectProperty")
		{   
			irival = node.attribute("IRI").as_string();
			irival = irival.substr(irival.find("#") + 1);
			string prop2 = "V3{" + irival + "}";
			entry = "($FA V0{z})($FA V0{z1})((";
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop1);
			entry.append(") $OR");
			entry.append("($OA V0{z1} $CO V0{z} $AO $NI ");
			entry.append(prop2);
			entry.append(")) $AD (");
			entry.append("($OA V0{z1} $CO V0{z} $AO $NI ");
			entry.append(prop2);
			entry.append(") $OR");
			entry.append("($OA V0{z} $CO V0{z1} $AO $IN ");
			entry.append(prop2);
			entry.append("))"); 
		}
	}
};
/*(8z1)(8z2)((:(hz1; z2i 2 XR3 1) _ hz2; z1i 2 XR3 2) ^ (:(hz2; z1i 2
	X3
	R2) _ hz1; z2i 2 XR3 1))*/

void readOWLXMLOntology(string filename, vector<pair<string, string>>& ontNamespaces, vector<string>& formulae)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "----Reading OWL/XML file: " << filename << "." << endl;
#endif
#endif // debug
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
			parseDeclaration(entry, it);
		else if (name == "ClassAssertion")
			parseClassAssertion(entry, it);
		else if (name == "ObjectPropertyAssertion")
			parseObjectPropertyAssertion(entry, it);
		else if (name == "NegativeObjectPropertyAssertion")
			parseNegativeObjectPropertyAssertion(entry, it);
		else if (name == "SameIndividual")
			parseSameIndividual(entry, it);
		else if (name == "DifferentIndividuals")
			parseDifferentIndividuals(entry, it);
		else if (name == "FunctionalObjectProperty")
		  parseFunctionalObjectProperty(entry, it);
		else if (name == "InverseFunctionalObjectProperty")
			parseInverseFunctionalObjectProperty(entry, it);
		else if (name == "ReflexiveObjectProperty")
			parseReflexiveObjectProperty(entry, it);
		else if (name == "IrreflexiveObjectProperty")
			parseIrreflexiveObjectProperty(entry, it);	
		else if (name == "TransitiveObjectProperty")
			parseTransitiveObjectProperty(entry, it);
		else if (name == "SymmetricObjectProperty")
			parseSymmetricObjectProperty(entry, it);
		else if (name == "AsymmetricObjectProperty")
			parseAsymmetricObjectProperty(entry, it);
		else if (name == "InverseObjectProperties")
			parseInverseObjectProperties(entry, it);
		if (!entry.empty())
			formulae.push_back(entry);	

		//cout << irival << endl;
		//cout << irival.substr(irival.find("#") + 1) << endl;
	}

#ifdef debug 
#ifdef debugparseXML
	logFile << "----End Reading OWL/XML file: " << filename << "." << endl;
#endif
#endif // debug
};