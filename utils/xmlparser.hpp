#ifndef __XML_PARSER_H
#define __XML_PARSER_H

#include "rapidxml/rapidxml.hpp"

#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <iostream>

using namespace rapidxml;
using namespace std;


class XMLNode {
	xml_node<> *node;

public:
	XMLNode(xml_node<> *node) {
		this->node = node;
	}
	XMLNode() {}

	bool IsArray() {
		string ret = node->name();
		return ret.substr(0, 5) == "this.";
	}
	bool IsChild() { return !IsArray(); }

	string GetName() {
		string ret = node->name();
		if(IsArray()) 
			return ret.substr(5, ret.size());
		return ret;
	}

	map<string, string> GetAttrs() {
		map<string, string> ret;

		for (xml_attribute<> *attr = node->first_attribute(); 
			attr; attr = attr->next_attribute()) 
		{
			ret[attr->name()] = attr->value();
		}

		return ret;
	}

	map<string, XMLNode> GetVectors() {
		map<string, XMLNode> ret;

		for (xml_node<> *attr = node->first_node(); 
			attr; attr = attr->next_sibling()) 
		{
			XMLNode node(attr);
			if (node.IsArray())
				ret[node.GetName()] = node;
		}

		return ret;
	}

	vector<XMLNode> GetChildren() {
		vector<XMLNode> ret;

		for (xml_node<> *attr = node->first_node(); 
			attr; attr = attr->next_sibling()) 
		{
			XMLNode node(attr);
			if (node.IsChild())
				ret.push_back(node);
		}
		
		return ret;
	}
};


class XMLParser {
	xml_document<> doc;

	void makeTree(istream &fin) {
		string currentLine, text;
		while (getline(fin, currentLine))
			text += currentLine + "\n";
		doc.parse<0>(strdup(text.c_str()));
	}

public:
	XMLParser(string filename) {
		ifstream fin(filename);
		makeTree(fin);
		fin.close();
	}

	XMLNode GetRoot() {
		return XMLNode(doc.first_node());
	}
};

#endif