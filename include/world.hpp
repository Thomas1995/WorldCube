#pragma once

#include <string>
#include <vector>
#include <iostream>
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

class Item : public Dumpable {
public:
	std::string name;

	// Dumpable implementation
	virtual void Dump(std::ostream &out);
};