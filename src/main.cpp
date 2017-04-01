#include <iostream>
#include "utils/parsable_utils.hpp"
#include "worldtime.hpp"
#include "person.hpp"
#include "utils/random.hpp"

int main() {
	Random::Init();
	World::Init();

	Person* p = World::GetPerson(0);

	std::string cmd;
	while (true) {
		std::cin >> cmd;
		Action* act = p->DecideNextAction();
		p->DoAction(act);
	}

	return 0;
}
