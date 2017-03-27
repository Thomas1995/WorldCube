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
	std::string GetName();
	std::unordered_map<std::string, std::string> GetAttrs();
	std::unordered_map<std::string, XMLNode> GetVectors();
	std::vector<XMLNode> GetChildren();
};


class XMLParser {
	xml_document<> doc;

	void makeTree(std::istream &fin);

public:
	XMLParser(const std::string filename);
	XMLNode GetRoot();
};
