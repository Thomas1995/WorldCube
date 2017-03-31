#include "worldtime.hpp"
#include "world.hpp"
#include "person.hpp"

Time* Time::singletonPtr = nullptr;

Time::Time() {
	time = 0;
}

Time::~Time() {}

void Time::Init(int ms) {
	if (singletonPtr == nullptr) {
		singletonPtr = new Time();

		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			singletonPtr->Tick();
		}
	}
}

unsigned long long Time::GetCurrentTime() {
	return singletonPtr->time;
}

void Time::Tick() {
	++time;
	CALLBACK top;
	while (!singletonPtr->cbks.empty() && std::get<0>(singletonPtr->cbks.top()) <= time) {
		{
			std::lock_guard<std::mutex> lk(singletonPtr->mut);
			top = singletonPtr->cbks.top();
			singletonPtr->cbks.pop();
		}
		std::get<1>(top)(time, std::get<2>(top), std::get<3>(top));
	}

	// it is time to update needs for the whole population
	if (time % Need::unit == 0) {
		World::ApplyOnPopulation([](Person* p) {
			auto needs = World::GetNeeds();

			std::lock_guard<std::mutex> lk(p->mut);
			for (auto need : *needs) {
				p->needs[need.name] += need.delta;
			}
		});
	}
}

void Time::RegisterCbk(unsigned long long t, void(*fct)(unsigned long long, void*, void*), void* context, void* additionalInfo) {
	std::lock_guard<std::mutex> lk(singletonPtr->mut);
	singletonPtr->cbks.push(std::make_tuple(t, fct, context, additionalInfo));
}