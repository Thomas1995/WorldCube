#include <iostream>
#include "utils/parsable_utils.hpp"

int main() {
	auto root = XMLParser("data.xml").GetRoot();
	auto actions = ParsableFactory<vector<Action>>::Build(root);

	for(auto action : actions)
		actions[0].Dump(cerr);

	return 0;
}
