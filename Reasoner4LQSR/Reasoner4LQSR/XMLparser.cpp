
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
#pragma once
using namespace std;

#define propertyindex 3
#define qvar0 (propertyindex+1)

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

void parseDeclaration(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found declaration. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	string name = node.name();
	string irival = node.attribute("IRI").as_string();
	string entry = "";
	if (name == "Class")
	{
		entry = "V1{" + irival.substr(irival.find("#") + 1) + "}";
		KBsize.at(1)++;
	}
	else if (name == "ObjectProperty")
	{
		entry = "V3{" + irival.substr(irival.find("#") + 1) + "}";
		KBsize.at(propertyindex)++;
	}
	else if (name == "NamedIndividual")
	{
		entry = "V0{" + irival.substr(irival.find("#") + 1) + "}";
		KBsize.at(0)++;
	}
	out.push_back(entry);
};

void parseClassAssertion(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found class assertion. " << endl;
#endif
#endif // debug
	string entry = "";
	for (pugi::xml_node_iterator node = it->begin(); node != it->end(); ++node)
	{
      string name = node->name();
	  
	  string irival = node->attribute("IRI").as_string();
	  if (irival.empty())
		  return;
	  if (name == "Class")
	  {
		  entry = "V1{" + irival.substr(irival.find("#") + 1) + "}";		  
	  }
	  else if (name == "NamedIndividual")
	  {
		  entry = "V0{" + irival.substr(irival.find("#") + 1) + "} $IN " + entry;		 
	  }

	}
	out.push_back(entry);
};

void parseObjectPropertyAssertion(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{	
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found object-property assertion. " << endl;
#endif
#endif // debug
	string entry = "";
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
	out.push_back(entry);
};

void parseNegativeObjectPropertyAssertion(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found negative object-property assertion. " << endl;
#endif
#endif // debug
	string entry = "";
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
	out.push_back(entry);
};

void parseSameIndividual(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found same individuals. " << endl;
#endif
#endif // debug
	string entry = "";
	pugi::xml_node node = it->first_child();
	string irival = node.attribute("IRI").as_string();
	entry = "V0{" + irival.substr(irival.find("#") + 1) + "} $EQ";
	node = node.next_sibling();
	irival = node.attribute("IRI").as_string();
	entry = entry + " V0{" + irival.substr(irival.find("#") + 1) + "}";
	out.push_back(entry);
};

void parseDifferentIndividuals(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found different individuals. " << endl;
#endif
#endif // debug
	string entry = "";
	pugi::xml_node node = it->first_child();
	string irival = node.attribute("IRI").as_string();
	entry = "V0{" + irival.substr(irival.find("#") + 1) + "} $QE";
	node = node.next_sibling();
	irival = node.attribute("IRI").as_string();
	entry = entry + " V0{" + irival.substr(irival.find("#") + 1) + "}";
	out.push_back(entry);
};

void parseFunctionalObjectProperty(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{	
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found functional object-property. " << endl;
#endif
#endif // debug
	string entry = "";
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
		if(KBsize.at(qvar0)<3)
		    KBsize.at(qvar0)=3;
		out.push_back(entry);
	}	
};

void parseInverseFunctionalObjectProperty(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found inverse functional object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{
		string entry = "";
		string irival = node.attribute("IRI").as_string();		
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z})($FA V0{z1})($FA V0{z2})";
		entry.append("((($OA V0{z} $CO V0{z1} $AO $NI ");
		entry.append(irival);
		entry.append(")$OR($OA V0{z2} $CO V0{z1} $AO $NI ");
		entry.append(irival);
		entry.append("))$OR( V0{z} $EQ V0{z2}))");
		if (KBsize.at(qvar0)<3)
			KBsize.at(qvar0) = 3;
		out.push_back(entry);
	}
};

void parseReflexiveObjectProperty(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found reflexive object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{
		string entry = "";
		string irival = node.attribute("IRI").as_string();		
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z})";
		entry.append("($OA V0{z} $CO V0{z} $AO $IN ");
		entry.append(irival);
		entry.append(")");
		out.push_back(entry);
		if (KBsize.at(qvar0)<1)
			KBsize.at(qvar0) = 1;
	}
};

void parseIrreflexiveObjectProperty(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found irreflexive object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{
		string entry = "";
		string irival = node.attribute("IRI").as_string();
		irival = irival.substr(irival.find("#") + 1);
		irival = "V3{" + irival + "}";
		entry = "($FA V0{z})";
		entry.append("($OA V0{z} $CO V0{z} $AO $NI ");
		entry.append(irival);
		entry.append(")");
		out.push_back(entry);
		if (KBsize.at(qvar0)<1)
			KBsize.at(qvar0) = 1;
	}
};



void parseTransitiveObjectProperty(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found transitive object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{
		string entry = "";
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
		out.push_back(entry);
		if (KBsize.at(qvar0)<3)
		 KBsize.at(qvar0) = 3;
	}
};

void parseSymmetricObjectProperty(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found symmetric object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{ 
		string entry = "";
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
		out.push_back(entry);
		if (KBsize.at(qvar0)<2)
			KBsize.at(qvar0) = 2;
	}
};

void parseAsymmetricObjectProperty(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found asymmetric object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{
		string entry = "";
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
		out.push_back(entry);
		if (KBsize.at(qvar0)<2)
			KBsize.at(qvar0) = 2;
	}
};

void parseInverseObjectProperties(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found inverse object-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{
		string entry = "";
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
			entry.append("($OA V0{z1} $CO V0{z} $AO $IN ");
			entry.append(prop2);
			entry.append(")) $AD (");
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop2);
			entry.append(") $OR");
			entry.append("($OA V0{z} $CO V0{z1} $AO $IN ");
			entry.append(prop1);
			entry.append(")))");
			out.push_back(entry);
			if (KBsize.at(qvar0)<2)
				KBsize.at(qvar0) = 2;
		}
	}
};

void parseEquivalentObjectProperties(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found equivalent object-properties. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{
		string entry = "";
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
			entry.append("($OA V0{z} $CO V0{z1} $AO $IN ");
			entry.append(prop2);
			entry.append(")) $AD (");
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop1);
			entry.append(") $OR");
			entry.append("($OA V0{z} $CO V0{z1} $AO $IN ");
			entry.append(prop2);
			entry.append(")))");
			out.push_back(entry);
			if (KBsize.at(qvar0)<2)
				KBsize.at(qvar0) = 2;
		}
		else if (string(node.name()) == "ObjectInverseOf")			
			{
				irival = (node.first_child()).attribute("IRI").as_string();
				irival = irival.substr(irival.find("#") + 1);
				string prop2 = "V3{" + irival + "}";
				entry = "($FA V0{z})($FA V0{z1})((";
				entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
				entry.append(prop1);
				entry.append(") $OR");
				entry.append("($OA V0{z1} $CO V0{z} $AO $IN ");
				entry.append(prop2);
				entry.append(")) $AD (");
				entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
				entry.append(prop1);
				entry.append(") $OR");
				entry.append("($OA V0{z1} $CO V0{z} $AO $IN ");
				entry.append(prop2);
				entry.append(")))");
				out.push_back(entry);
				if (KBsize.at(qvar0)<2)
					KBsize.at(qvar0) = 2;
			}
	}

};

void parseDisjointObjectProperties(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found disjoint object-properties. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{
		string entry = "";
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
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop2);
			entry.append(")) $AD (");
			entry.append("($OA V0{z} $CO V0{z1} $AO $IN ");
			entry.append(prop2);
			entry.append(") $OR");
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop1);
			entry.append(")))");
			out.push_back(entry);
			if (KBsize.at(qvar0)<2)
				KBsize.at(qvar0) = 2;
		}
		else if (string(node.name()) == "ObjectInverseOf")
		{
			irival = (node.first_child()).attribute("IRI").as_string();
			irival = irival.substr(irival.find("#") + 1);
			string prop2 = "V3{" + irival + "}";
			entry = "($FA V0{z})($FA V0{z1})((";
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop1);
			entry.append(") $OR");
			entry.append("($OA V0{z1} $CO V0{z} $AO $NI ");
			entry.append(prop2);
			entry.append(")) $AD (");
			entry.append("($OA V0{z1} $CO V0{z} $AO $IN ");
			entry.append(prop2);
			entry.append(") $OR");
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop1);
			entry.append(")))");
			out.push_back(entry);
			if (KBsize.at(qvar0)< 2)
				KBsize.at(qvar0) = 2;
		}
	}

};


void parseSubObjectProperty(vector<std::string>& out, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found subobject-property. " << endl;
#endif
#endif // debug
	pugi::xml_node node = it->first_child();
	if (string(node.name()) == "ObjectProperty")
	{
		string entry = "";
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
			entry = "($FA V0{z})($FA V0{z1})(";
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop1);
			entry.append(") $OR");
			entry.append("($OA V0{z} $CO V0{z1} $AO $IN ");
			entry.append(prop2);	
			entry.append("))");
			if (KBsize.at(qvar0)< 2)
				KBsize.at(qvar0) = 2;
		}
		else if (string(node.name()) == "ObjectInverseOf")
		{
			irival = (node.first_child()).attribute("IRI").as_string();
			irival = irival.substr(irival.find("#") + 1);
			string prop2 = "V3{" + irival + "}";
			entry = "($FA V0{z})($FA V0{z1})(";
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop1);
			entry.append(") $OR");
			entry.append("($OA V0{z1} $CO V0{z} $AO $IN ");
			entry.append(prop2);
			entry.append("))");
			if (KBsize.at(qvar0)<2)
				KBsize.at(qvar0) = 2;
		}
	}

};

void parseObjectInserctionOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectUnionOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectComplementOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectOneOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectSomeValuesFrom(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectAllValuesFrom(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectHasValue(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectHasSelf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz,vector<int>& KBsize) { throw new exception(); };
void parseObjectMinCardinality(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectMaxCardinality(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectExactCardinality(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseSubClassOf(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseEquivalentClasses(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseDisjointClasses(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseDisjointUnion(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectPropertyChain(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectPropertyDomain(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseObjectPropertyRange(vector<std::string>& entry, pugi::xml_node_iterator& it, string varz, vector<int>& KBsize) { throw new exception(); };
void parseClassExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize) { throw new exception; }








void readOWLXMLOntology(string filename, vector<pair<string, string>>& ontNamespaces, vector<string>& formulae, vector<int>& KBsize)
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
		string name = it->name();
		if (name == "Declaration")
			parseDeclaration(formulae, it, KBsize);
		else if (name == "ClassAssertion")
			parseClassAssertion(formulae, it, KBsize);
		else if (name == "ObjectPropertyAssertion")
			parseObjectPropertyAssertion(formulae, it, KBsize);
		else if (name == "NegativeObjectPropertyAssertion")
			parseNegativeObjectPropertyAssertion(formulae, it, KBsize);
		else if (name == "SameIndividual")
			parseSameIndividual(formulae, it, KBsize);
		else if (name == "DifferentIndividuals")
			parseDifferentIndividuals(formulae, it, KBsize);
		else if (name == "FunctionalObjectProperty")
		  parseFunctionalObjectProperty(formulae, it, KBsize);
		else if (name == "InverseFunctionalObjectProperty")
			parseInverseFunctionalObjectProperty(formulae, it, KBsize);
		else if (name == "ReflexiveObjectProperty")
			parseReflexiveObjectProperty(formulae, it, KBsize);
		else if (name == "IrreflexiveObjectProperty")
			parseIrreflexiveObjectProperty(formulae, it, KBsize);
		else if (name == "TransitiveObjectProperty")
			parseTransitiveObjectProperty(formulae, it, KBsize);
		else if (name == "SymmetricObjectProperty")
			parseSymmetricObjectProperty(formulae, it, KBsize);
		else if (name == "AsymmetricObjectProperty")
			parseAsymmetricObjectProperty(formulae, it, KBsize);
		else if (name == "InverseObjectProperties")
			parseInverseObjectProperties(formulae, it, KBsize);
		else if (name == "EquivalentObjectProperties")
			parseEquivalentObjectProperties(formulae, it, KBsize);
		else if (name == "DisjointObjectProperties")
			parseDisjointObjectProperties(formulae, it, KBsize);
		else if (name == "SubObjectProperty")
			parseSubObjectProperty(formulae, it, KBsize);
		// if (!entry.empty())
		//	formulae.push_back(entry);	

		//cout << irival << endl;
		//cout << irival.substr(irival.find("#") + 1) << endl;
	}

#ifdef debug 
#ifdef debugparseXML
	logFile << "----The Ontology Contains: " << endl;
	logFile << "----- Individuals: " << KBsize.at(0) << "." << endl;
	logFile << "----- Classes: " << KBsize.at(1) << "." << endl;
	logFile << "----- Properties: " << KBsize.at(propertyindex) << "." << endl;
	logFile << "----- Quantified Variables of level 0: " << KBsize.at(qvar0) << "." << endl;
	logFile << "----End Reading OWL/XML file: " << filename << "." << endl;
#endif
#endif // debug

};