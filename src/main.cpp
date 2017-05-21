#include "utils/parsable_utils.hpp"
#include "worldhandler.h"
#include "worldtime.hpp"
#include "person.hpp"
#include "utils/random.hpp"

int main() {
	Random::Init();
	WorldHandler::Init();
	Time::Init();
	World::Init();

	while (true) {
		World::PerformNextAction();
	}

	return 0;
}
