#include "utils/xmlparser.hpp"

// XMLNode

XMLNode::XMLNode(xml_node<> *node) {
  this->node = node;
}

XMLNode::XMLNode() {}

bool XMLNode::IsArray() {
  string ret = node->name();
  return ret.substr(0, 5) == "this.";
}

bool XMLNode::IsChild() {
  return !IsArray();
}

string XMLNode::GetName() {
  string ret = node->name();
  if(IsArray())
    return ret.substr(5, ret.size());
  return ret;
}

unordered_map<string, string> XMLNode::GetAttrs() {
  unordered_map<string, string> ret;

  for (xml_attribute<> *attr = node->first_attribute();
    attr; attr = attr->next_attribute())
  {
    ret[attr->name()] = attr->value();
  }

  return ret;
}

unordered_map<string, XMLNode> XMLNode::GetVectors() {
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

vector<XMLNode> XMLNode::GetChildren() {
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

// XMLParser

void XMLParser::makeTree(istream &fin) {
  string currentLine, text;
  while (getline(fin, currentLine))
    text += currentLine + "\n";
  doc.parse<0>(strdup(text.c_str()));
}

XMLParser::XMLParser(const string filename) {
  Path p;
  (&p)->Add("..")->Add("..")->Add("data")->Add(filename);
  ifstream fin(p);
  makeTree(fin);
  fin.close();
}

XMLNode XMLParser::GetRoot() {
  return XMLNode(doc.first_node());
}
