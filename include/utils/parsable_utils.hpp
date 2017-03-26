#pragma once

#include "xmlparser.hpp"
#include "abstractions/parsable.hpp"
#include "abstractions/dumpable.hpp"
#include <unordered_map>

using namespace std;

template<typename T>
class ParsableFactory {
public:
	static T Build(XMLNode node) = delete;
};

class Person;

class InfoMessage {
public:
	double time;
	string message;

	static void s_Print(unsigned long long t, void* context, void* additionalInfo);
	void Print(unsigned long long t, Person* owner);
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

class Effect {
public:
	string name;
	double delta;
};
template<>
class ParsableFactory<Effect> {
public:
	static Effect Build(XMLNode node) {
		Effect ret;
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
	vector<Effect> effects;
	std::pair<int, int> timeSpent;

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
		
		auto timeSpentVec = Split(attrs["time"], '-');
		if (timeSpentVec.size() == 2) {
			ret.timeSpent.first = atoi(timeSpentVec[0].c_str());
			ret.timeSpent.second = atoi(timeSpentVec[1].c_str());
		}

		ret.infoMessages = ParsableFactory<vector<InfoMessage>>::Build(vects["InfoMessages"]);
		ret.effects = ParsableFactory<vector<Effect>>::Build(vects["Effects"]);

		return ret;
	}
};

class Item : public Dumpable {
public:
	// Dumpable implementation
	virtual void Dump(ostream &out);
};
template<>
class ParsableFactory<Item> {
public:
	static Item Build(XMLNode node) {
		Item ret;
		auto attrs = node.GetAttrs();
		auto vects = node.GetVectors();

		return ret;
	}
};