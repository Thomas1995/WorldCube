#include "person.hpp"
#include "worldtime.hpp"
#include "utils/random.hpp"
#include <sstream>
#include <iomanip>

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

	// the person may do nothing
	weights.push_back(1);
	associatedAction.push_back(World::GetAction("nothing"));

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
	Action* nextAction = associatedAction[index];
	if (nextAction->item != nullptr) {
		Item* item = nextAction->item;
		double weightSum;

		weights.clear();

		for (const auto& subitems : item->subitems) {
			weightSum = 1;
			for (const auto& eff : subitems.effects) {
				const double needVal = needs[eff.name];
				weightSum += -eff.delta * needVal;
			}
			weightSum = std::max(weightSum, 1.0);
			weights.push_back(weightSum);
		}

		int index2 = Random::GetRandIndexProbField(weights);
		Subitem* subitem = &item->subitems[index2];
		envVars["SUBITEMID"] = std::to_string(index2);

		int subitemNameIndex = Random::GetRandUniformDistr(0, (int)subitem->shownNames.size() - 1);
		envVars["ITEM"] = subitem->shownNames[subitemNameIndex];

		double qty = std::max(
			Random::GetRandNormalDistr(subitem->qtyDetails.first, subitem->qtyDetails.second), 
			0.01); // the quantity shouldn't go under an epsilon

		if (subitem->qtyType == Subitem::int_qty) {
			qty = (double)((int)qty);
			if (qty == 0)
				qty = 1;
		}

		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << qty;
		envVars["QUANTITY"] = stream.str();
	}

	/*if (index == -1) {
		index = Random::GetRandUniformDistr(0, (int)actions->size() - 1);

		for (const auto& act : *actions)
			if (index-- == 0)
				return World::GetAction(act.second.name);

		return nullptr;
	}*/

	return associatedAction[index];
}

void Person::DoAction(Action* action) {
	unsigned long long crtTime = Time::GetCurrentTime();
	int spentTime = Random::GetRandBinomialDistr(action->timeSpent.first, action->timeSpent.second);

	double qty = (double)spentTime; // if there is no item associated it means the quantity will be the time spent on action
	if (action->item != nullptr)
		qty = stof(envVars["QUANTITY"]);

	// register InfoMessages
	for (auto& im : action->infoMessages) {
		im.time == 0 ?
			im.s_Print(crtTime, (void*)&im, this) :
			Time::RegisterCbk(crtTime + (unsigned long long)(im.time * spentTime), &im.s_Print, (void*)&im, this);
	}

	AdditionalInfo* addInfo = new AdditionalInfo;
	addInfo->ptr = (void*)action;
	addInfo->val = qty;

	// register end of action
	Time::RegisterCbk(crtTime + spentTime + 1, &Person::FinishAction, this, (void*)addInfo);
}

void Person::FinishAction(unsigned long long t, void* context, void* additionalInfo) {
	Person* pers = static_cast<Person*>(context);
	AdditionalInfo* info = (AdditionalInfo*)additionalInfo;
	const Action* action = (Action*)info->ptr;
	const double qty = info->val;
	delete info;

	TakeEffects(t, action->effects, pers, qty);

	if (action->item != nullptr) {
		int subitemId = std::stoi(pers->envVars["SUBITEMID"]);
		Subitem* item = &action->item->subitems[subitemId];

		TakeEffects(t, item->effects, pers, qty);
	}

	World::AddActionToScheduler(pers, pers->DecideNextAction());
}

void Person::TakeEffects(unsigned long long t, const std::vector<Effect>& effects, Person* pers, const double qty) {
	for (const auto& eff : effects) {
		AdditionalInfo* addInfo = new AdditionalInfo;
		addInfo->ptr = (void*)&eff;
		addInfo->val = qty;

		if (eff.delay == 0) {
			UpdateNeed(t, pers, (void*)addInfo);
		}
		else {
			Time::RegisterCbk(t + (unsigned long long)eff.delay, &Person::UpdateNeed, pers, (void*)addInfo);
		}
	}
}

void Person::UpdateNeed(unsigned long long t, void* context, void* additionalInfo) {
	Person* pers = static_cast<Person*>(context);
	AdditionalInfo* info = (AdditionalInfo*)additionalInfo;
	const Effect* eff = (Effect*)info->ptr;
	const double qty = info->val;
	delete info;

	std::lock_guard<std::mutex> lk(pers->mut);
	pers->needs[eff->name] += eff->delta * qty;
	if (pers->needs[eff->name] < 0)
		pers->needs[eff->name] = 0;
}