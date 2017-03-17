#pragma once

#include "xmlparser.hpp"
#include "abstractions/parsable.hpp"
#include "abstractions/dumpable.hpp"

using namespace std;

template<typename T>
class ParsableFactory {
public:
	static T Build(XMLNode node) = delete;
};

class InfoMessage {
public:
	double time;
	string message;
};
template<>
class ParsableFactory<InfoMessage> {
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
class ParsableFactory<vector<P>> {
public:
	static vector<P> Build(XMLNode node) {
		vector<P> ret;

		for (auto child : node.GetChildren()) {
			ret.push_back(ParsableFactory<P>::Build(child));
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
class ParsableFactory<NeedChange> {
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
	virtual void Dump(ostream &out);
};
template<>
class ParsableFactory<Action> {
public:
	static Action Build(XMLNode node) {
		Action ret;
		auto attrs = node.GetAttrs();
		auto vects = node.GetVectors();

		ret.name = attrs["name"];

		ret.infoMessages = ParsableFactory<vector<InfoMessage>>::Build(vects["InfoMessages"]);
		ret.needChanges = ParsableFactory<vector<NeedChange>>::Build(vects["NeedChanges"]);

		return ret;
	}
};
