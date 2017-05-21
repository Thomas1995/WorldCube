#include "person.hpp"
#include "worldtime.hpp"
#include "utils/random.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

Person::Person(const std::string name) {
	envVars["NAME"] = name;

	auto needs = World::GetNeeds();
	for (auto need : *needs) {
		needsSeeds[need.name] = Random::GetRandNormalDistr(1.0, 0.25);
	}
}

std::string Person::GetName() {
	return GetEnvVar("NAME");
}

std::string Person::GetEnvVar(const std::string var) {
	return envVars[var.c_str()];
}

Action* Person::DecideNextAction() {
	std::vector<double> weights;

	// the person may do nothing
	weights.push_back(50);

	auto pneeds = World::GetNeeds();
	for (size_t i = 0; i < pneeds->size(); ++i) {
		weights.push_back(needs[pneeds->at(i).name]);
	}

	int index = Random::GetRandIndexProbField(weights) - 1;

	if (index < 0) {
		return World::GetAction("nothing");
	}
	else {
		Need* crtNeed = &pneeds->at(index);
		double needVal = needs[crtNeed->name];
		auto changes = World::GetNeedsNegativeChanges(crtNeed->name);

		weights.clear();
		for (size_t i = 0; i < changes->size(); ++i) {
			weights.push_back(1.0 / std::fabs(needVal - changes->at(i).change));
		}

		index = Random::GetRandIndexProbField(weights);
		Action* nextAction = changes->at(index).action;

		if (nextAction->item != nullptr) {
			Item* item = nextAction->item;
			int subitemNo = changes->at(index).subitemNo;

			// if the item does not matter, tend to choose the one with less side effects
			if (subitemNo == -1) {
				weights.clear();

				double weight;
				for (const auto& subitems : item->subitems) {
					weight = 1;
					for (const auto& eff : subitems.effects)
						if (eff.delta > 0)
							weight += eff.delta;

					weights.push_back(1.0 / weight);
				}

				subitemNo = Random::GetRandIndexProbField(weights);
			}

			Subitem* subitem = &item->subitems[subitemNo];
			envVars["SUBITEMID"] = std::to_string(subitemNo);

			int subitemNameIndex = Random::GetRandUniformDistr(0, (int)subitem->shownNames.size() - 1);
			envVars["ITEM"] = subitem->shownNames[subitemNameIndex];

			double qty = std::max(
				Random::GetRandNormalDistr(subitem->qtyDetails.first, subitem->qtyDetails.second),
				0.01); // the quantity shouldn't go under an epsilon

			if (subitem->qtyType == Subitem::int_qty) {
				qty = (double)((int)qty);
				if (qty == 0)
					qty = 1;

				envVars["QUANTITY"] = std::to_string((int)qty);
			}
			else {
				std::stringstream stream;
				stream << std::fixed << std::setprecision(2) << qty;
				envVars["QUANTITY"] = stream.str();
			}
		}

		return nextAction;
	}
}

void Person::DoAction(Action* action) {
	unsigned long long crtTime = Time::GetCurrentTime();
	int spentTime = Random::GetRandBinomialDistr(action->timeSpent.first, action->timeSpent.second);

	double qty = (double)spentTime; // if there is no item associated it means the quantity will be the time spent on action
	if (action->item != nullptr)
		qty = std::stof(envVars["QUANTITY"]);

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

	// action statistics
	statistics.actionsNo[action->name] += 1;
	statistics.actionsQty[action->name] += qty;
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
