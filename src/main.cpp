#include <iostream>
#include "utils/parsable_utils.hpp"
#include "worldtime.hpp"
#include "person.hpp"
#include "utils/random.hpp"

int main() {
	Random::Init();
	World::Init();

	std::string cmd;
	while (true) {
		std::cin >> cmd;
		World::GetPerson(0)->DoAction(World::GetAction("drink"));
	}

	return 0;
}
