#pragma once

#include "utils/parsable_utils.hpp"
#include <string>
#include <unordered_map>

class Person {
public:
	Person(const std::string name);
	
	std::string GetName();
	void DoAction(const Action& action);

private:
	std::unordered_map<std::string, std::string> envVars;
};