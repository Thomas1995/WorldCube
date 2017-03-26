#include "worldtime.hpp"

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
	std::lock_guard<std::mutex> lk(singletonPtr->mut);
	++time;
	while (!singletonPtr->cbks.empty() && std::get<0>(singletonPtr->cbks.top()) <= time) {
		std::get<1>(singletonPtr->cbks.top())(time, std::get<2>(singletonPtr->cbks.top()), std::get<3>(singletonPtr->cbks.top()));
		singletonPtr->cbks.pop();
	}
}

void Time::RegisterCbk(unsigned long long t, void(*fct)(unsigned long long, void*, void*), void* context, void* additionalInfo) {
	std::lock_guard<std::mutex> lk(singletonPtr->mut);
	singletonPtr->cbks.push(std::make_tuple(t, fct, context, additionalInfo));
}