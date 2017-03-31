#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "abstractions/dumpable.hpp"

class Person;

class InfoMessage {
public:
	double time;
	std::string message;

	static void s_Print(unsigned long long t, void* context, void* additionalInfo);
	void Print(unsigned long long t, Person* owner);
};

class Effect {
public:
	std::string name;
	double delta;
};

class Action : public Dumpable {
public:
	std::string name;
	std::vector<InfoMessage> infoMessages;
	std::vector<Effect> effects;
	std::pair<int, int> timeSpent;

	// Dumpable implementation
	virtual void Dump(std::ostream &out);
};

class Subitem : public Dumpable {
public:
	std::string name;
	std::vector<Effect> effects;
	std::vector<std::string> shownNames;

	// Dumpable implementation
	virtual void Dump(std::ostream &out);
};

class Item : public Dumpable {
public:
	std::string name;
	std::vector<Subitem> subitems;

	// Dumpable implementation
	virtual void Dump(std::ostream &out);
};

class World {
	std::unordered_map<std::string, Action> actions;
	std::unordered_map<std::string, Item> items;

	static World* singletonPtr;

	World();
	~World();

	World(const World&) = delete;
	void operator = (const World&) = delete;

public:
	static void Init();

	static Action& GetAction(const std::string name);
	static Item& GetItem(const std::string name);
};