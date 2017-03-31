#include <iostream>
#include <thread>
#include "utils/parsable_utils.hpp"
#include "worldtime.hpp"
#include "person.hpp"
#include "utils/random.hpp"

int main() {
	Random::Init();
	World::Init();

	Person p("Steve")

	std::thread timeThread(&Time::Init, 1);
	timeThread.detach();

	std::string cmd;
	while (true) {
		std::cin >> cmd;
		p.DoAction(World::GetAction("drink"));
	}

	return 0;
}
