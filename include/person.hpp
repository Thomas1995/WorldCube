#pragma once

#include "world.hpp"
#include <string>
#include <unordered_map>
#include <mutex>

class Person {
public:
	std::unordered_map<std::string, double> needs;
	std::mutex mut;

	Person(const std::string name);
	
	std::string GetName();
	std::string GetEnvVar(const std::string var);
	void DoAction(Action* action);
	Action* DecideNextAction();

private:
	std::unordered_map<std::string, std::string> envVars;

	static void FinishAction(unsigned long long t, void* context, void* additionalInfo);
	static void TakeEffects(unsigned long long t, const std::vector<Effect>& effects, Person* pers, const double qty);
	static void UpdateNeed(unsigned long long t, void* context, void* additionalInfo);

	struct AdditionalInfo {
		void* ptr;
		double val;
	};
};
