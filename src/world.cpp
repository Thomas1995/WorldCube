#include "world.hpp"
#include "person.hpp"
#include "worldtime.hpp"
#include "utils/parsable_utils.hpp"
#include <iostream>
#include <thread>

// InfoMessage

void InfoMessage::s_Print(unsigned long long t, void* context, void* additionalInfo) {
	static_cast<InfoMessage*>(context)->Print(t, static_cast<Person*>(additionalInfo));
}

void InfoMessage::Print(unsigned long long t, Person* owner) {
	std::string printMsg, var;
	bool inVar = false;

	for (const char& c : message) {
		if (c == '%') {
			if (inVar) {
				printMsg += owner->GetEnvVar(var);
				var.clear();
			}
			inVar = !inVar;
			continue;
		}

		if (inVar) {
			var.push_back(c);
		}
		else {
			printMsg.push_back(c);
		}
	}

	std::cout << t << ": " << printMsg << "\n";
}

// Need

int Need::unit = 0;

// Subitem

void Subitem::Dump(std::ostream & out) {}

// Item

void Item::Dump(std::ostream &out) {}

// Action

void Action::Dump(std::ostream &out) {
	out << "Action \"" << name << "\"\n";
	out << "InfoMessages: ";
	for (auto x : infoMessages) {
		out << "\"" << x.message << "\" ";
	}
	out << "\nEffects: ";
	for (auto x : effects) {
		out << "[" << x.name << " -> " << x.delta << "] ";
	}
	out << std::endl;
}

// World

World* World::singletonPtr = nullptr;

World::World() {}

World::~World() {
	for (auto it = population.begin(); it != population.end(); ++it) {
		delete (*it);
	}
	
	population.clear();
	needs.clear();
	items.clear();
	actions.clear();
}

void World::Init() {
	if (singletonPtr == nullptr) {
		singletonPtr = new World();

		// when parsing the needs, the first need is a fictive one which sets the update time, so we must pop it from our vector
		singletonPtr->needs = ParsableFactory<std::vector<Need>>::Build(XMLParser("needs.xml").GetRoot());
		std::swap(singletonPtr->needs.front(), singletonPtr->needs.back());
		singletonPtr->needs.pop_back();

		singletonPtr->items = ParsableFactory<std::unordered_map<std::string, Item>>::Build(XMLParser("items.xml").GetRoot());
		singletonPtr->actions = ParsableFactory<std::unordered_map<std::string, Action>>::Build(XMLParser("actions.xml").GetRoot());

		singletonPtr->population.push_back(new Person("Steve")); // temporary

		std::thread timeThread(&Time::Init, 1);
		timeThread.detach();
	}
}

Action* World::GetAction(const std::string name) {
	return &singletonPtr->actions[name];
}

Item* World::GetItem(const std::string name) {
	return &singletonPtr->items[name];
}

Person* World::GetPerson(const int index) {
	if (index < 0 || index >= singletonPtr->population.size())
		return nullptr;
	else
		return singletonPtr->population[index];
}

std::vector<Need>* World::GetNeeds() {
	return &singletonPtr->needs;
}

std::unordered_map<std::string, Action>* World::GetActions() {
	return &singletonPtr->actions;
}

void World::ApplyOnPopulation(void(*fct)(Person *p)) {
	for (auto p : singletonPtr->population)
		fct(p);
}
