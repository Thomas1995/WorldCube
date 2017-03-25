#pragma once

#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#define CALLBACK std::tuple<unsigned long long, void(*)(unsigned long long, void*), void*>

class Time {
	static Time* singletonPtr;
	
	unsigned long long time;
	std::priority_queue< CALLBACK, std::vector<CALLBACK>, std::greater< CALLBACK > > cbks;
	std::mutex mut;
	
	Time();
	~Time();

	Time(const Time&) = delete;
	void operator = (const Time&) = delete;

public:
	static void Init(int ms);
	static unsigned long long GetCurrentTime();
	static void RegisterCbk(unsigned long long t, void (*)(unsigned long long, void*), void* context);

private:
	void Tick();
};