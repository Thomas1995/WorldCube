#ifndef __PARSEABLE_BUILDER_H_
#define __PARSEABLE_BUILDER_H_

#include "xmlparser.hpp"
#include "../abstractions/parseable.hpp"
#include "../abstractions/dumpable.hpp"

using namespace std;

class ParseableFactory {
public:
	template<typename P>
	static P Build(XMLNode node) {
		P ret;
		ret.ParseFromXML(node);
		return ret;
	}
};

class InfoMessage : public Parseable {
public:
	double time;
	string message;

	// Parseable implementation
	virtual string GetTagName() { return "InfoMessage"; }
	virtual void ParseFromXML(XMLNode node) {
		auto attrs = node.GetAttrs();

		time = stof(attrs["time"]);
		message = attrs["message"];
	}
};

template<typename P>
class ParseableVector : public vector<P>, public Parseable {
public:
	virtual string GetTagName() { return nullptr; }
	virtual void ParseFromXML(XMLNode node) {
		for(auto child : node.GetChildren()) {
			this->push_back(ParseableFactory::Build<P>(child));
		}
	}
};


class NeedChange : public Parseable {
public:
	string name;
	double delta;

	// Parseable implementation
	virtual string GetTagName() { return "NeedChange"; }
	virtual void ParseFromXML(XMLNode node) {
		auto attrs = node.GetAttrs();
		auto vects = node.GetVectors();

		name = attrs["name"];
		delta = stof(attrs["delta"]);
	}
};

class Action : public Parseable, public Dumpable {
public:
	string name;
	ParseableVector<InfoMessage> infoMessages;
	ParseableVector<NeedChange> needChanges;

	// Parseable implementation
	virtual string GetTagName() { return "Action"; }
	virtual void ParseFromXML(XMLNode node) {
		auto attrs = node.GetAttrs();
		auto vects = node.GetVectors();

		name = attrs["name"];

		infoMessages = ParseableFactory::Build<ParseableVector<InfoMessage>>(vects["InfoMessages"]);
		needChanges = ParseableFactory::Build<ParseableVector<NeedChange>>(vects["NeedChanges"]);
	}

	// Dumpable implementation
	virtual void Dump(ostream &out) {
		out << "Action \"" << name << "\"\n";
		out << "InfoMessages: ";
		for(auto x : infoMessages) {
			out << "\"" << x.message << "\" ";
		}
		out << "\nNeedChanges: ";
		for(auto x : needChanges) {
			out << "[" << x.name << " -> " << x.delta << "] ";
		}
		out << endl;
	}
};



#endif