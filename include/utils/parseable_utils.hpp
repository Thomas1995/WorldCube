#pragma once

#include "xmlparser.hpp"
#include "abstractions/parseable.hpp"
#include "abstractions/dumpable.hpp"

using namespace std;

template<typename T>
class ParseableFactory {
public:
	static T Build(XMLNode node) = delete;
};


class InfoMessage {
public:
	double time;
	string message;
};
template<>
class ParseableFactory<InfoMessage> {
public:
	static InfoMessage Build(XMLNode node) {
		InfoMessage ret;
		auto attrs = node.GetAttrs();

		ret.time = stof(attrs["time"]);
		ret.message = attrs["message"];

		return ret;
	}
};


template<typename P>
class ParseableFactory<vector<P>> {
public:
	static vector<P> Build(XMLNode node) {
		vector<P> ret;

		for (auto child : node.GetChildren()) {
			ret.push_back(ParseableFactory<P>::Build(child));
		}

		return ret;
	}
};


class NeedChange {
public:
	string name;
	double delta;
};
template<>
class ParseableFactory<NeedChange> {
public:
	static NeedChange Build(XMLNode node) {
		NeedChange ret;
		auto attrs = node.GetAttrs();
		auto vects = node.GetVectors();

		ret.name = attrs["name"];
		ret.delta = stof(attrs["delta"]);

		return ret;
	}
};

class Action : public Dumpable {
public:
	string name;
	vector<InfoMessage> infoMessages;
	vector<NeedChange> needChanges;

	// Dumpable implementation
	virtual void Dump(ostream &out) {
		out << "Action \"" << name << "\"\n";
		out << "InfoMessages: ";
		for (auto x : infoMessages) {
			out << "\"" << x.message << "\" ";
		}
		out << "\nNeedChanges: ";
		for (auto x : needChanges) {
			out << "[" << x.name << " -> " << x.delta << "] ";
		}
		out << endl;
	}
};
template<>
class ParseableFactory<Action> {
public:
	static Action Build(XMLNode node) {
		Action ret;
		auto attrs = node.GetAttrs();
		auto vects = node.GetVectors();

		ret.name = attrs["name"];

		ret.infoMessages = ParseableFactory<vector<InfoMessage>>::Build(vects["InfoMessages"]);
		ret.needChanges = ParseableFactory<vector<NeedChange>>::Build(vects["NeedChanges"]);

		return ret;
	}
};
