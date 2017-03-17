#include <iostream>
#include "utils/parsable_utils.hpp"

int main() {
	auto root = XMLParser("data.xml").GetRoot();
	auto action = ParsableFactory<Action>::Build(root);

	action.Dump(cerr);

	return 0;
}
