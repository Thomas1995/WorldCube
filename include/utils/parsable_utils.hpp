#pragma once

#include "xmlparser.hpp"
#include "abstractions/parsable.hpp"
#include "abstractions/dumpable.hpp"
#include "world.hpp"
#include <unordered_map>
#include <string>

template<typename T>
class ParsableFactory {
public:
	static T Build(XMLNode node) = delete;
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
class ParsableFactory<std::vector<P>> {
public:
	static std::vector<P> Build(XMLNode node) {
		std::vector<P> ret;

		for (auto child : node.GetChildren()) {
			ret.push_back(ParsableFactory<P>::Build(child));
		}

		return ret;
	}
};

template<typename P>
class ParsableFactory<std::unordered_map<std::string, P>> {
public:
	static std::unordered_map<std::string, P> Build(XMLNode node) {
		std::unordered_map<std::string, P> ret;

		for (auto child : node.GetChildren()) {
			P item = ParsableFactory<P>::Build(child);
			ret[item.name] = item;
		}

		return ret;
	}
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
		if (attrs.find("delay") == attrs.end()) {
			ret.delay = 0;
		}
		else {
			ret.delay = stoi(attrs["delay"]);
		}

		return ret;
	}
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

		ret.infoMessages = ParsableFactory<std::vector<InfoMessage>>::Build(vects["InfoMessages"]);
		ret.effects = ParsableFactory<std::vector<Effect>>::Build(vects["Effects"]);

		if (attrs.find("item") != attrs.end()) {
			ret.item = World::GetItem(attrs["item"]);
		}
		else {
			ret.item = nullptr;
		}

		return ret;
	}
};

template<>
class ParsableFactory<Item> {
public:
	static Item Build(XMLNode node) {
		Item ret;
		auto attrs = node.GetAttrs();
		auto vects = node.GetVectors();

		ret.name = attrs["name"];
		ret.subitems = ParsableFactory<std::vector<Subitem>>::Build(vects["Subitems"]);

		return ret;
	}
};

template<>
class ParsableFactory<Subitem> {
public:
	static Subitem Build(XMLNode node) {
		Subitem ret;
		auto attrs = node.GetAttrs();
		auto vects = node.GetVectors();

		ret.name = attrs["name"];

		if (attrs.find("shown") != attrs.end()) {
			ret.shownNames = Split(attrs["shown"], ';');
		}
		else {
			ret.shownNames.push_back(ret.name);
		}

		ret.qtyDetails = std::make_pair(1.0, 0.0);
		ret.qtyType = Subitem::int_qty;

		if (attrs.find("qty_mean") != attrs.end()) {
			ret.qtyDetails.first = stof(attrs["qty_mean"]);
			ret.qtyDetails.second = stof(attrs["qty_sigma"]);

			if(attrs["qty_type"] == "real")
				ret.qtyType = Subitem::real_qty;
		}

		if (vects.find("Effects") != vects.end())
			ret.effects = ParsableFactory<std::vector<Effect>>::Build(vects["Effects"]);

		return ret;
	}
};

template<>
class ParsableFactory<Need> {
public:
	static Need Build(XMLNode node) {
		Need ret;
		auto attrs = node.GetAttrs();
		auto vects = node.GetVectors();

		if (attrs.find("unit") != attrs.end()) {
			ret.unit = stoi(attrs["unit"]);
		}
		else {
			ret.name = attrs["name"];
			ret.delta = stof(attrs["delta"]);
		}

		return ret;
	}
};
