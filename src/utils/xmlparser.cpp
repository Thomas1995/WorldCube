#include "utils/xmlparser.hpp"

// XMLNode

XMLNode::XMLNode(xml_node<> *node) {
	this->node = node;
}

XMLNode::XMLNode() {}

bool XMLNode::IsArray() {
	std::string ret = node->name();
	return ret.substr(0, 5) == "this.";
}

bool XMLNode::IsChild() {
	return !IsArray();
}

std::string XMLNode::GetName() {
	std::string ret = node->name();
	if(IsArray())
	return ret.substr(5, ret.size());
	return ret;
}

std::unordered_map<std::string, std::string> XMLNode::GetAttrs() {
	std::unordered_map<std::string, std::string> ret;

	for (xml_attribute<> *attr = node->first_attribute();
	attr; attr = attr->next_attribute()) {
		ret[attr->name()] = attr->value();
	}

	return ret;
}

std::unordered_map<std::string, XMLNode> XMLNode::GetVectors() {
	std::unordered_map<std::string, XMLNode> ret;

	for (xml_node<> *attr = node->first_node();
	attr; attr = attr->next_sibling()) {
		XMLNode node(attr);
		if (node.IsArray())
			ret[node.GetName()] = node;
	}

	return ret;
}

std::vector<XMLNode> XMLNode::GetChildren() {
	std::vector<XMLNode> ret;

	for (xml_node<> *attr = node->first_node();
	attr; attr = attr->next_sibling()) {
	XMLNode node(attr);
	if (node.IsChild())
		ret.push_back(node);
	}

	return ret;
}

// XMLParser

void XMLParser::makeTree(std::istream &fin) {
	std::string currentLine, text;
	while (getline(fin, currentLine))
		text += currentLine + "\n";
	doc.parse<0>(strdup(text.c_str()));
}

XMLParser::XMLParser(const std::string filename) {
	Path p;
	(&p)->Add("..")->Add("..")->Add("data")->Add(filename);
	std::ifstream fin(p);
	makeTree(fin);
	fin.close();
}

XMLNode XMLParser::GetRoot() {
  return XMLNode(doc.first_node());
}
