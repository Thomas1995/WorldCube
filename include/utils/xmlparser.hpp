#pragma once

#include "rapidxml/rapidxml.hpp"
#include "utils/utils.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>

using namespace rapidxml;
using namespace std;

#ifdef VS2015
#define strdup _strdup
#endif

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

	unordered_map<string, string> GetAttrs() {
		unordered_map<string, string> ret;

		for (xml_attribute<> *attr = node->first_attribute();
			attr; attr = attr->next_attribute())
		{
			ret[attr->name()] = attr->value();
		}

		return ret;
	}

	unordered_map<string, XMLNode> GetVectors() {
		unordered_map<string, XMLNode> ret;

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
	XMLParser(const string filename) {
		Path p;
		(&p)->Add("..")->Add("..")->Add("data")->Add(filename);
		ifstream fin(p);
		makeTree(fin);
		fin.close();
	}

	XMLNode GetRoot() {
		return XMLNode(doc.first_node());
	}
};
