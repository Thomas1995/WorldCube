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
	XMLNode(xml_node<> *node);
	XMLNode();
	bool IsArray();
	bool IsChild();
	string GetName();
	unordered_map<string, string> GetAttrs();
	unordered_map<string, XMLNode> GetVectors();
	vector<XMLNode> GetChildren();
};


class XMLParser {
	xml_document<> doc;

	void makeTree(istream &fin);

public:
	XMLParser(const string filename);
	XMLNode GetRoot();
};
