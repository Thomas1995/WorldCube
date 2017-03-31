#include "person.hpp"
#include "worldtime.hpp"

Person::Person(const std::string name) {
	envVars["NAME"] = name;
}

std::string Person::GetName() {
	return GetEnvVar("NAME");
}

std::string Person::GetEnvVar(const std::string var) {
	return envVars[var.c_str()];
}

void Person::DoAction(Action* action) {
	unsigned long long crtTime = Time::GetCurrentTime();
	// change to normal distribution here for [action.timeSpent.first, action.timeSpent.second]
	// maybe use a person generated seed to make someone's habits not change so much
	int spentTime = (action->timeSpent.second + action->timeSpent.first) / 2;

	// get this data from xml
	envVars["QUANTITY"] = "1";
	envVars["ITEM"] = "coke";

	// register InfoMessages
	for (auto& im : action->infoMessages) {
		im.time == 0 ? 
			im.s_Print(crtTime, (void*)&im, this) :
			Time::RegisterCbk(crtTime + (unsigned long long)(im.time * spentTime), &im.s_Print, (void*)&im, this);
	}

	// register end of action
	Time::RegisterCbk(crtTime + spentTime + 1, &Person::FinishAction, this, (void*)action);
}

void Person::FinishAction(unsigned long long t, void* context, void* additionalInfo) {
	Person* pers = static_cast<Person*>(context);
	const Action* action = (Action*)additionalInfo;

	for (const auto& eff : action->effects) {
		pers->needs[eff.name] += eff.delta;
		if (pers->needs[eff.name] < 0)
			pers->needs[eff.name] = 0;

		std::cout << eff.name << " " << pers->needs[eff.name] << "\n"; // delete this line
	}
}
