
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
		string prop1 = retrieveVarNameFromNode(node, "IRI", 3);
		node = node.next_sibling();		
		if (string(node.name()) == "ObjectProperty")
		{			
			string prop2 = retrieveVarNameFromNode(node, "IRI", 3);
			entry = "($FA V0{z})($FA V0{z1})(";
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop1);
			entry.append(") $OR");
			entry.append("($OA V0{z} $CO V0{z1} $AO $IN ");
			entry.append(prop2);	
			entry.append("))");
			out.push_back(entry);
			if (KBsize.at(qvar0)< 2)
				KBsize.at(qvar0) = 2;
		}
		else if (string(node.name()) == "ObjectInverseOf")
		{			
			string prop2 = retrieveVarNameFromNode(node.first_child(), "IRI", 3);
			entry = "($FA V0{z})($FA V0{z1})(";
			entry.append("($OA V0{z} $CO V0{z1} $AO $NI ");
			entry.append(prop1);
			entry.append(") $OR");
			entry.append("($OA V0{z1} $CO V0{z} $AO $IN ");
			entry.append(prop2);
			entry.append("))");
			out.push_back(entry);
			if (KBsize.at(qvar0)<2)
				KBsize.at(qvar0) = 2;
		}
		
	}
     else if (string(node.name()) == "ObjectPropertyChain")
		{		
		  string entry = "";
		  string quant = "($FA V0{z})";
		  int i = 1;
			for (pugi::xml_node_iterator ch = node.begin(); ch != node.end(); ++ch,++i)
			{
				if (string(ch->name()) == "ObjectProperty")
				{
					cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
					string propc = retrieveVarNameFromNode(ch, "IRI", 3);
					quant += "($FA V0{z" + to_string(i) + "})";
					string prevVar = "z";
					if (i > 1)
						prevVar = prevVar + to_string(i-1);
					if (i % 2 && ch != node.last_child())
						entry += "(";
					entry += "($OA V0{"+prevVar+"} $CO "+ "V0{ z" + to_string(i) + " } $OA $IN "+propc + ")";
					if (i % 2==0)
						entry += ")";
					if (ch != node.last_child())
						entry += "$AD ";
					
				}
                    
			} 
			entry= quant+ "( ("+entry+") $IF ($OA V0{z} $CO " + "V0{ z" + to_string(i-1) + " } $OA $IN " + retrieveVarNameFromNode(node.next_sibling(), "IRI", 3) + "))";
			out.push_back(entry);
			if (KBsize.at(qvar0)< i)
			  KBsize.at(qvar0) = i;
		}
};






string retrieveNameFromNode(pugi::xml_node_iterator const& it, string const& attribute)
{
	string irival = it->attribute("IRI").as_string();
	irival = irival.substr(irival.find("#") + 1);
	return irival;
}

string retrieveVarNameFromNode(pugi::xml_node_iterator const& it, string const& attribute, int const& var)
{
	return "V" + to_string(var) + "{" + retrieveNameFromNode(it, attribute) + "}";
}

void parseSubClassOfExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize)
{	
	int varz = 0;
	string formula = "";	
	string res = "(V0{z} $IN " + retrieveVarNameFromNode(it->first_child(), "IRI", 1) + ")";
	pugi::xml_node_iterator node = it->first_child().next_sibling();
	int varcount = 1;
	int val = -1;
	if (node->name() == "Class")
	{		
		formula = "(" + res + "$IF" + "(V0{z} $IN " + retrieveVarNameFromNode(node, "IRI", 1) + "))";
	}
	else
	{
		val=parseClassExpression(formula, node, varz, varcount);
		if (val == 0)			
		  formula = "(" + res + "$II" + "(" + formula + ")" + ")";
		else if(val==1)
			formula += "$IF " + res + ")";
		else if (val==2)
			formula = "(" + res + "$IF" + "(" + formula + ")" + ")";
	}
	string quantifier = "($FA V0{z})";
	if (varcount > 1)
	{
		for (int i = 1; i < varcount; i++)
			quantifier.append("($FA V0{z").append(to_string(i)).append("})");
	}
	formula = quantifier.append(formula);
	entry.push_back(formula);
	if (KBsize.at(qvar0)< varcount)
		KBsize.at(qvar0) = varcount;
	
}

void parseDisjointUnionExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
	throw new exception();
}

void parseDisjointClassesExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
	int varz = 0;
	string formula = "";
	int varcount = 1;
	parseDisjointClasses(formula, it, varz, varcount);
	string quantifier = "($FA V0{z})";
	if (varcount > 1)
	{
		for (int i = 1; i < varcount; i++)
			quantifier.append("($FA V0{z").append(to_string(i)).append("})");
	}
	formula = quantifier.append(formula);
	entry.push_back(formula);
	if (KBsize.at(qvar0)< varcount)
		KBsize.at(qvar0) = varcount;
}


void parseObjectPropertyRangeExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
	int varz = 0;
	string formula = "";
	int varcount = 2;
	parseObjectPropertyRange(formula, it, varz, varcount);
	string quantifier = "($FA V0{z})($FA V0{z1})";
	if (varcount > 2)
	{
		for (int i = 2; i < varcount; i++)
			quantifier.append("($FA V0{z").append(to_string(i)).append("})");
	}
	formula = quantifier.append(formula);
	entry.push_back(formula);
	if (KBsize.at(qvar0)< varcount)
		KBsize.at(qvar0) = varcount;
}

void parseObjectPropertyDomainExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize)
{
	int varz = 0;
	string formula = "";
	int varcount = 2;
	parseObjectPropertyDomain(formula, it, varz, varcount);		
	string quantifier = "($FA V0{z})($FA V0{z1})";
	if (varcount > 2)
	{
		for (int i = 2; i < varcount; i++)
			quantifier.append("($FA V0{z").append(to_string(i)).append("})");
	}
	formula = quantifier.append(formula);
	entry.push_back(formula);
	if (KBsize.at(qvar0)< varcount)
		KBsize.at(qvar0) = varcount;
}

void parseEquivalentClassExpression(vector<std::string>& entry, pugi::xml_node_iterator& it, vector<int>& KBsize)
{	
	int varz = 0;
	string formula = "";
	string irival = it->first_child().attribute("IRI").as_string();	
	string res = "(V0{z} $IN " +  retrieveVarNameFromNode(it->first_child(), "IRI", 1) + ")";
	pugi::xml_node_iterator node = it->first_child().next_sibling();
	int varcount = 1;
	int val = -1;
	if (node->name() == "Class")
	{		
		formula = "(" + res + "$II" + "(V0{z} $IN " + retrieveVarNameFromNode(node, "IRI", 1) + "))";
	}
	else
	{
		val=parseClassExpression(formula, node, varz, varcount);
		if(val==0)
		  formula = "(" + res + "$II" + "(" + formula + ")" + ")";
		else if (val == 1)	
		     formula += "$IF " + res + ")";
		else if (val == 2)
			formula = "(" + res + "$IF" + "(" + formula + ")" + ")";
	}
	
	string quantifier = "($FA V0{z})";
	if (varcount > 1)
	{
	  for (int i = 1; i < varcount; i++)
				quantifier.append("($FA V0{z").append(to_string(i)).append("})");
	}
		formula = quantifier.append(formula);
	
	
	entry.push_back(formula);
	if (KBsize.at(qvar0)< varcount)
		KBsize.at(qvar0) = varcount;	
}

int parseClassExpression(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{		
	if (string(it->name()) == "ObjectIntersectionOf")
	{
	  parseObjectIntersectionOf(entry, it, varz, varcount);
	}
	else if (string(it->name()) == "ObjectUnionOf")
	{
	 parseObjectUnionOf(entry, it, varz, varcount);
	}
	else if (string(it->name()) == "ObjectComplementOf")
	{
	 parseObjectComplementOf(entry, it, varz, varcount);
	}
	else if (string(it->name()) == "ObjectOneOf")
	{
		parseObjectOneOf(entry, it, varz, varcount);
	}
	else if (string(it->name()) == "ObjectHasSelf")
	{
		parseObjectHasSelf(entry, it, varz, varcount);
	}
	else if (string(it->name()) == "ObjectHasValue")
	{
		parseObjectHasValue(entry, it, varz, varcount);
	}
	else if (string(it->name()) == "ObjectSomeValuesFrom")
	{
		parseObjectSomeValuesFrom(entry, it, varz, varcount);
		return 1;
	}
	else if (string(it->name()) == "ObjectAllValuesFrom")
	{
		parseObjectAllValuesFrom(entry, it, varz, varcount);
		return 2;
	}
	else if (string(it->name()) == "ObjectMinCardinality")
	{
		parseObjectMinCardinality(entry, it, varz, varcount);
		return 2;
	}
	else if (string(it->name()) == "ObjectMaxCardinality")
	{
		parseObjectMaxCardinality(entry, it, varz, varcount);
		return 1;
	}
	return 0;
}
void parseObjectIntersectionOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{
	
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found ObjectIntersectionOf class expression. " << endl;
#endif
#endif // debug            
	
	for (pugi::xml_node_iterator node = it->begin(); node != it->end(); ++node)
		{			
		  if (string(node->name()) == "Class")
		  {	  
			
			string var = "z";
			if (varz > 0)
				var += to_string(varz);
			string res = "(V0{" + var + "} $IN " + retrieveVarNameFromNode(node, "IRI", 1) +")";
			entry += res;
			if (node != it->begin())
				entry = "(" + entry + ")";
		  }
		  else
		  {
			  string intformula = "";
			  parseClassExpression(intformula, node, varz, varcount);
			  entry +=  intformula ;
		  }					  
		
		  if( node!=it->last_child())
			  entry += "$AD ";		  
		}	
};

void parseObjectUnionOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{

#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found ObjectUnionOf class expression. " << endl;
#endif
#endif // debug

	for (pugi::xml_node_iterator node = it->begin(); node != it->end(); ++node)
	{
		if (string(node->name()) == "Class")
		{			
			string var = "z";
			if (varz > 0)
				var += to_string(varz);
			string res = "(V0{" + var + "} $IN " + retrieveVarNameFromNode(node, "IRI", 1) + ")";
			entry += res;
			if (node != it->begin())
				entry = "(" + entry + ")";
		}
		else
		{
			string intformula = "";
			parseClassExpression(intformula, node, varz, varcount);
			entry += intformula;
		}

		if (node != it->last_child())
			entry += "$OR ";
	}
};

void parseObjectComplementOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found ObjectComplementOf class expression. " << endl;
#endif
#endif // debug

	for (pugi::xml_node_iterator node = it->begin(); node != it->end(); ++node)
	{
		if (string(node->name()) == "Class")
		{			
			string var = "z";
			if (varz > 0)
				var += to_string(varz);
			string res = "($NG (V0{" + var + "} $IN " + retrieveVarNameFromNode(node, "IRI", 1) + "))";
			entry += res;
			if (node != it->begin())
				entry = "(" + entry + ")";
		}
		else
		{
			string intformula = "($NG ( ";
			parseClassExpression(intformula, node, varz, varcount);
			entry += intformula + "))";
		}
	}
	
};

void parseObjectPropertyDomain(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{ 
	//(∀z1)(∀z2)(hz1, z2i ∈ XP31 ↔(hz1, z2i ∈ XP32 ∧ z1 ∈ XC1 1))
#ifdef debug 
#ifdef debugparseXML
		logFile << "-----Found ObjectPropertyDomain class expression. " << endl;
#endif
#endif // debug            
	   if (varcount < 2)
		 varcount = 2;

	    pugi::xml_node_iterator node = it->begin(); 
	
		entry += "( ( $OA V0{z} $CO V0{z1} $AO $IN V3{";		
		entry+=retrieveNameFromNode(node, "IRI")+ "}) $II";
		node = node->next_sibling();

		if (string(node->name()) == "Class")
		{			
			string var = "z";
			if (varz > 0)
				var += varz;
			string res = "(V0{" + var + "} $IN " + retrieveVarNameFromNode(node, "IRI", 1) + ") )";
			entry += res;
			
		}
		else
		{
			string intformula = "(";
			parseClassExpression(intformula, node, varz, varcount);
			entry += intformula+")";
		}		
};

void parseObjectPropertyRange(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{	
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found ObjectPropertyRange class expression. " << endl;
#endif
#endif // debug            
	if (varcount < 2)
		varcount = 2;

	pugi::xml_node_iterator node = it->begin();

	entry += "( ( $OA V0{z} $CO V0{z1} $AO $IN V3{";	
	entry += retrieveNameFromNode(node, "IRI") + "}) $II";
	node = node->next_sibling();

	if (string(node->name()) == "Class")
	{		
		string var = "z";
		if (varz > 0)
			var += to_string(varz);
		string res = "(V0{" + var + "} $IN " + retrieveVarNameFromNode(node, "IRI", 1) + ") )";
		entry += res;

	}
	else
	{
		string intformula = "(";
		parseClassExpression(intformula, node, varz+1, varcount);
		entry += intformula + ")";
	}
};
void parseDisjointClasses(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found DisjointClasses class expression. " << endl;
#endif
#endif // debug

	pugi::xml_node_iterator node = it->begin();	
	string var = "z";
	if (varz > 0)
		var += to_string(varz);
	string res = "(V0{" + var + "} $IN " + retrieveVarNameFromNode(node, "IRI", 1) + ") $II ";
	entry += res;
	

	for (pugi::xml_node_iterator nd = node->next_sibling(); nd != it->end(); ++nd)
	{
		if (string(nd->name()) == "Class")
		{
			
			string var = "z";
			if (varz > 0)
				var += to_string(varz);
			string res = "($NG (V0{" + var + "} $IN " + retrieveVarNameFromNode(nd, "IRI", 1) + "))";
			entry += res;
			if (nd != node)
				entry = "(" + entry + ")";
		}
		else
		{
			string intformula = "($NG ( ";
			parseClassExpression(intformula, nd, varz, varcount);
			entry += intformula + "))";
		}
		if (nd != it->last_child())
			entry += "$AD ";
	}

};


void parseObjectOneOf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{

#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found ObjectOneOf class expression. " << endl;
#endif
#endif // debug            

	for (pugi::xml_node_iterator node = it->begin(); node != it->end(); ++node)
	{
		if (string(node->name()) == "NamedIndividual")
		{			
			string var = "z";
			if (varz > 0)
				var += to_string(varz);
			string res = "(V0{" + var + "} $EQ " + retrieveVarNameFromNode(node, "IRI", 0) + ")";
			entry += res;
			if (node != it->begin())
				entry = "(" + entry + ")";
		}
		if (node != it->last_child())
			entry += "$OR ";
	}
};

void parseObjectHasSelf(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found ObjectHasSelf class expression. " << endl;
#endif
#endif // debug            
	        pugi::xml_node_iterator node = it->begin();		
			string var = "z";
			if (varz > 0)
				var += to_string(varz);
			string res = "($OA V0{" + var + "} $CO  V0{" + var + "} $OA $IN "+ retrieveVarNameFromNode(node, "IRI", 3) + ")";
			entry += res;	
	
};

void parseObjectHasValue(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found ObjectHasSelf class expression. " << endl;
#endif
#endif // debug            
	pugi::xml_node_iterator node = it->begin();	
	string classn = retrieveVarNameFromNode(node, "IRI", 3);
	
	node = node->next_sibling();
	string var = "z";
	if (varz > 0)
		var += to_string(varz);
	string res = "($OA V0{" + var + "} $CO "+ retrieveVarNameFromNode(node, "IRI", 0)+   " $OA $IN " + classn + ")";
	entry += res;
};

void parseObjectSomeValuesFrom(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount)
{
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found ObjectSomeValuesFrom class expression. " << endl;
#endif
#endif // debug  
	if (varcount < 2)
		varcount = 2;
	pugi::xml_node_iterator node = it->begin();
	string var = "z";
	if (varz > 0)
		var += to_string(varz);
	string res =  retrieveVarNameFromNode(node, "IRI", 3);
	entry += "( ($OA V0{"+var+"} $CO V0{z"+ to_string(varz + 1)+"} $AO $IN "+res+") $AD";
	string intformula ="";
	node = node->next_sibling(); 	
	if (string(node->name()) == "Class")
	{	
		intformula = "( V0{z" + to_string(varz + 1) + "} $IN " + retrieveVarNameFromNode(node, "IRI", 1) + ")";		
	}
	else
	{		
		parseClassExpression(intformula, node, varz + 1, varcount);
	}
	entry += intformula+")";	
};

void parseObjectAllValuesFrom(string& entry, pugi::xml_node_iterator& it, int varz, int &varcount)
{ 
#ifdef debug 
#ifdef debugparseXML
	logFile << "-----Found ObjectAllValuessFrom class expression. " << endl;
#endif
#endif // debug  
	if (varcount < 2)
		varcount = 2;
	pugi::xml_node_iterator node = it->begin();
	string var = "z";
	if (varz > 0)
		var += to_string(varz);
	string res = retrieveVarNameFromNode(node, "IRI", 3);
	entry += "( ($OA V0{" + var + "} $CO V0{z" + to_string(varz + 1) + "} $AO $IN " + res + ") $IF";
	string intformula = "";
	node = node->next_sibling();
	if (string(node->name()) == "Class")
	{
		intformula = "( V0{z" + to_string(varz + 1) + "} $IN " + retrieveVarNameFromNode(node, "IRI", 1) + ")";
	}
	else
	{
		parseClassExpression(intformula, node, varz + 1, varcount);
	}
	entry += intformula + ")";

};


void parseObjectMinCardinality(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount) { throw new exception(); };
void parseObjectMaxCardinality(string& entry, pugi::xml_node_iterator& it, int varz, int&varcount) { throw new exception(); };
void parseDisjointUnion(string& entry, pugi::xml_node_iterator& it, int varz, int& varcount) { throw new exception(); };


void parseObjectExactCardinality(string& entry, pugi::xml_node_iterator& it, int varz) { throw new exception(); };











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
		else if (name == "SubObjectPropertyOf")
			parseSubObjectProperty(formulae, it, KBsize);
		else if (name == "SubClassOf")
			parseSubClassOfExpression(formulae, it, KBsize);
		else if (name == "EquivalentClasses")
			parseEquivalentClassExpression(formulae, it, KBsize);
		else if (name == "ObjectPropertyDomain")
			parseObjectPropertyDomainExpression(formulae, it, KBsize);
		else if (name == "ObjectPropertyRange")
			parseObjectPropertyRangeExpression(formulae, it, KBsize);
		else if (name == "DisjointClasses")		
			parseDisjointClassesExpression(formulae, it, KBsize);
		else if (name == "DisjointUnion")
			parseDisjointUnionExpression(formulae, it, KBsize);
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