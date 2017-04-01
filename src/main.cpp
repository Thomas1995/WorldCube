#include <iostream>
#include "utils/parsable_utils.hpp"
#include "worldtime.hpp"
#include "person.hpp"
#include "utils/random.hpp"

int main() {
	Random::Init();
	Time::Init();
	World::Init();

	Person* p = World::GetPerson(0);
	//p->DecideNextAction();
	p->DoAction(World::GetAction("nothing"));

	//std::string cmd;
	while (true) {
		//std::cin >> cmd;
		World::PerformNextAction();
	}

	return 0;
}
