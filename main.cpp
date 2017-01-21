#include <iostream>
#include "utils/parseable_utils.hpp"

int main() {
	auto root = XMLParser("data.xml").GetRoot();
	auto action = ParseableFactory<Action>::Build(root);

	action.Dump(cerr);

	return 0;
}