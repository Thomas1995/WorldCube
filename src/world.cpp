#include "world.hpp"
#include "person.hpp"
#include "worldtime.hpp"
#include "utils/parsable_utils.hpp"
#include <iostream>
#include <thread>

// InfoMessage

std::mutex InfoMessage::mut;

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

	std::lock_guard<std::mutex> lk(InfoMessage::mut);
	std::cout << t << ", " << Date::SecondsToDate(t) << ": " << printMsg << "\n";
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

FitParam::FitParam(Action* action, const int subitemNo, const double change) {
	this->action = action;
	this->subitemNo = subitemNo;
	this->change = change;
}

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

		for (const auto& it : singletonPtr->actions) {
			const Action* action = &it.second;
			double factor = 1;

			Item* item = action->item;
			if (item != nullptr) {
				unsigned int index = 0;
				for (const auto& subitems : item->subitems) {
					factor = subitems.qtyDetails.first;
					for (const auto& eff : subitems.effects) {
						if (eff.delta < 0)
							singletonPtr->needsNegativeChanges[eff.name].push_back(
								FitParam(GetAction(action->name), index, -eff.delta * factor)
							);
					}
					++index;
				}
			}
			else {
				factor = (action->timeSpent.second - action->timeSpent.first) / 2.0;
			}

			for (const auto& eff : action->effects) {
				if (eff.delta < 0)
					singletonPtr->needsNegativeChanges[eff.name].push_back(FitParam(GetAction(action->name), -1, -eff.delta * factor));
			}
		}

		singletonPtr->population.push_back(new Person("Steve")); // temporary

		std::thread timeThread(&Time::Start, 0);
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
	if (index < 0 || index >= (int)singletonPtr->population.size())
		return nullptr;
	else
		return singletonPtr->population[index];
}

std::vector<Need>* World::GetNeeds() {
	return &singletonPtr->needs;
}

std::vector<FitParam>* World::GetNeedsNegativeChanges(const std::string needName) {
	return &singletonPtr->needsNegativeChanges[needName];
}

std::unordered_map<std::string, Action>* World::GetActions() {
	return &singletonPtr->actions;
}

void World::ApplyOnPopulation(void(*fct)(Person *p)) {
	for (auto p : singletonPtr->population)
		fct(p);
}

void World::AddActionToScheduler(Person* p, Action* action) {
	{
		std::lock_guard<std::mutex> lk(singletonPtr->mut);
		singletonPtr->scheduler.push(std::make_pair(p, action));
	}

	singletonPtr->cond_var.notify_one();
}

void World::PerformNextAction() {
	Person* p;
	Action* action;

	{
		std::unique_lock<std::mutex> lk(singletonPtr->mut);

		singletonPtr->cond_var.wait(lk, [&] {
			return !singletonPtr->scheduler.empty();
		});

		p = singletonPtr->scheduler.front().first;
		action = singletonPtr->scheduler.front().second;
		singletonPtr->scheduler.pop();
	}

	p->DoAction(action);
}

// Date

unsigned long long Date::lastT = 0LL;
int Date::second = 0;
int Date::minute = 0;
int Date::hour = 0;
int Date::day = 1;
int Date::month = 1;
int Date::year = 2015;

void Date::UpdateMonth(unsigned long long &t, int monthDays) {
	if (t >= (unsigned long long)(monthDays - Date::day + 1)) {
		t = t - (monthDays - Date::day + 1);
		Date::day = 1;
		++Date::month;
		if (Date::month == 13) {
			Date::month = 1;
			++Date::year;
		}
	}
	else {
		Date::day += (int)t;
		t = 0;
	}
}

std::string Date::SecondsToDate(unsigned long long t) {
	std::string date;

	if (lastT != t) {
		unsigned long long prevT = t;
		t = t - lastT;
		lastT = prevT;

		Date::second = (t = Date::second + t) % 60;
		t /= 60;

		Date::minute = (t = Date::minute + t) % 60;
		t /= 60;

		Date::hour = (t = Date::hour + t) % 24;
		t /= 24;

		while (t > 0) {
			if (Date::month == Date::MONTHS::FEBRUARY) {
				bool leapYear = Date::year % 4 == 0 && (Date::year % 100 != 0 || Date::year % 400 == 0) ? true : false;
				if (leapYear) {
					Date::UpdateMonth(t, 29);
				}
				else {
					Date::UpdateMonth(t, 28);
				}
			}
			else if (Date::month == Date::MONTHS::JANUARY ||
							 Date::month == Date::MONTHS::MARCH ||
							 Date::month == Date::MONTHS::MAY ||
							 Date::month == Date::MONTHS::JULY ||
							 Date::month == Date::MONTHS::AUGUST ||
							 Date::month == Date::MONTHS::OCTOBER ||
							 Date::month == Date::MONTHS::DECEMBER) {
				Date::UpdateMonth(t, 31);
			}
			else if (Date::month == Date::MONTHS::APRIL ||
							 Date::month == Date::MONTHS::JUNE ||
							 Date::month == Date::MONTHS::SEPTEMBER ||
							 Date::month == Date::MONTHS::NOVEMBER) {
				Date::UpdateMonth(t, 30);
			}
		}
	}

	if (Date::day < 10) {
		date = "0";
	}
	date += std::to_string(Date::day) + "/";
	if (Date::month < 10) {
		date += "0";
	}
	date += std::to_string(Date::month) + "/" +
					std::to_string(Date::year) + " - ";
	if (Date::hour < 10) {
		date += "0";
	}
	date += std::to_string(Date::hour) + ":";
	if (Date::minute < 10) {
		date += "0";
	}
	date += std::to_string(Date::minute) + ":";
	if (Date::second < 10) {
		date += "0";
	}
	date += std::to_string(Date::second);

	return date;
}
