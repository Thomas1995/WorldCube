#include <iostream>
#include "utils/parseable_utils.hpp"

int main() {
	auto root = XMLParser("data.xml").GetRoot();
	auto action = ParseableFactory::Build<Action>(root);

	action.Dump(cerr);

	return 0;
}