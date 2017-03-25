#include "person.hpp"
#include "worldtime.hpp"

Person::Person(const std::string name) {
	envVars["NAME"] = name;
}

std::string Person::GetName() {
	return envVars["NAME"];
}

void Person::DoAction(const Action& action) {
	unsigned long long crtTime = Time::GetCurrentTime();
	int spentTime = 5; // get this value from xml

	for (auto& im : action.infoMessages) {
		im.time == 0 ? 
			im.Print(crtTime, (void*)&im) :
			Time::RegisterCbk(crtTime + im.time * spentTime, &im.Print, (void*)&im);
	}
}