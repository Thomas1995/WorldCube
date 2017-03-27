#include <iostream>
#include <thread>
#include "utils/parsable_utils.hpp"
#include "worldtime.hpp"
#include "person.hpp"

int main() {
	auto actions = ParsableFactory<std::unordered_map<std::string, Action>>::Build(XMLParser("actions.xml").GetRoot());
	auto items = ParsableFactory<std::unordered_map<std::string, Item>>::Build(XMLParser("items.xml").GetRoot());

	for(auto action : actions)
		action.second.Dump(std::cerr);

	Person p("Steve");

	std::thread timeThread(&Time::Init, 1);
	timeThread.detach();

	std::string cmd;
	while (true) {
		std::cin >> cmd;
		p.DoAction(actions["drink"]);
	}

	return 0;
}
