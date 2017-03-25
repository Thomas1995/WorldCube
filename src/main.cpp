#include <iostream>
#include <thread>
#include "utils/parsable_utils.hpp"
#include "worldtime.hpp"
#include "person.hpp"

int main() {
	auto actions = ParsableFactory<vector<Action>>::Build(XMLParser("actions.xml").GetRoot());
	auto items = ParsableFactory<vector<Item>>::Build(XMLParser("items.xml").GetRoot());

	for(auto action : actions)
		action.Dump(cerr);

	Person p("Steve");

	std::thread timeThread(&Time::Init, 1);
	timeThread.detach();

	std::string cmd;
	while (true) {
		cin >> cmd;
		p.DoAction(actions[0]);
	}

	return 0;
}
