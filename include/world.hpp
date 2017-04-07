#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "abstractions/dumpable.hpp"

class Person;

class InfoMessage {
public:
	double time;
	std::string message;
	static std::mutex mut;

	static void s_Print(unsigned long long t, void* context, void* additionalInfo);
	void Print(unsigned long long t, Person* owner);
};

class Need {
public:
	static int unit;
	std::string name;
	double delta;
};

class Effect {
public:
	std::string name;
	double delta;
	int delay;
};

class Subitem : public Dumpable {
public:
	std::string name;
	std::vector<Effect> effects;
	std::vector<std::string> shownNames;
	std::pair<double, double> qtyDetails;
	enum {
		int_qty,
		real_qty,
	} qtyType;

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

class Action : public Dumpable {
public:
	std::string name;
	std::vector<InfoMessage> infoMessages;
	std::vector<Effect> effects;
	std::pair<int, int> timeSpent;
	Item* item;

	// Dumpable implementation
	virtual void Dump(std::ostream &out);
};

struct FitParam {
	Action* action;
	int subitemNo;
	double change;

	FitParam(Action* action, const int subitemNo, const double change);
};

class World {
	std::vector<Person*> population;
	std::vector<Need> needs;
	std::unordered_map<std::string, Item> items;
	std::unordered_map<std::string, Action> actions;
	std::queue< std::pair<Person*, Action*> > scheduler;
	std::mutex mut;
	std::condition_variable cond_var;

	std::unordered_map< std::string, std::vector<FitParam> > needsNegativeChanges;

	static World* singletonPtr;

	World();
	~World();

	World(const World&) = delete;
	void operator = (const World&) = delete;

public:
	static void Init();

	static std::vector<Need>* GetNeeds();
	static std::vector<FitParam>* GetNeedsNegativeChanges(const std::string needName);
	static std::unordered_map<std::string, Action>* GetActions();
	static Action* GetAction(const std::string name);
	static Item* GetItem(const std::string name);
	static Person* GetPerson(const int index);

	static void ApplyOnPopulation(void (*fct)(Person* p));

	static void AddActionToScheduler(Person* p, Action* action);
	static void PerformNextAction();
};

class Date {
private:
	enum MONTHS {JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER};
	static int second, minute, hour, day, month, year;
	static unsigned long long lastT;
	static void UpdateMonth(unsigned long long &t, int monthDays);
public:
	static std::string SecondsToDate(unsigned long long t);
};
