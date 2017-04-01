#include "person.hpp"
#include "worldtime.hpp"
#include "utils/random.hpp"

Person::Person(const std::string name) {
	envVars["NAME"] = name;
}

std::string Person::GetName() {
	return GetEnvVar("NAME");
}

std::string Person::GetEnvVar(const std::string var) {
	return envVars[var.c_str()];
}

Action* Person::DecideNextAction() {
	std::vector<double> weights;
	std::vector<Action*> associatedAction;

	auto actions = World::GetActions();

	for (const auto& act : *actions)
		for (const auto& eff : act.second.effects) {
			const double needVal = needs[eff.name];

			if (eff.delta < 0 && needVal != 0) {
				weights.push_back(-eff.delta * needVal);
				associatedAction.push_back(World::GetAction(act.second.name));
			}
		}

	int index = Random::GetRandIndexProbField(weights);

	if (index == -1) {
		index = Random::GetRandUniformDistr(0, (int)actions->size() - 1);

		for (const auto& act : *actions)
			if (index-- == 0)
				return World::GetAction(act.second.name);

		return nullptr;
	}

	return associatedAction[index];
}

void Person::DoAction(Action* action) {
	unsigned long long crtTime = Time::GetCurrentTime();
	int spentTime = Random::GetRandBinomialDistr(action->timeSpent.first, action->timeSpent.second);

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
		if (eff.delay == 0) {
			UpdateNeed(t, pers, (void*)&eff);
		} else {
			Time::RegisterCbk(t + (unsigned long long)eff.delay, &Person::UpdateNeed, pers, (void*)&eff);
		}
	}

	World::AddActionToScheduler(pers, pers->DecideNextAction());
}

void Person::UpdateNeed(unsigned long long t, void* context, void* additionalInfo) {
	Person* pers = static_cast<Person*>(context);
	const Effect* eff = (Effect*)(additionalInfo);

	std::lock_guard<std::mutex> lk(pers->mut);
	pers->needs[eff->name] += eff->delta * stof(pers->envVars["QUANTITY"]);
	if (pers->needs[eff->name] < 0)
		pers->needs[eff->name] = 0;

	std::cout << eff->name << " " << pers->needs[eff->name] << "\n"; // delete this line
}
