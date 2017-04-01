#include <iostream>
#include "utils/parsable_utils.hpp"
#include "worldtime.hpp"
#include "person.hpp"
#include "utils/random.hpp"

int main() {
	Random::Init();
	World::Init();

	Person* p = World::GetPerson(0);
	p->DoAction(World::GetAction("drink"));

	std::string cmd;
	while (true) {
		//std::cin >> cmd;
		World::PerformNextAction();
	}

	return 0;
}
