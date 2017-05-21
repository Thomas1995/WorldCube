#pragma once

#include <string>
#include <vector>

class WorldHandler {
	static WorldHandler* singletonPtr;

	WorldHandler();
	~WorldHandler();

	WorldHandler(const WorldHandler&) = delete;
	void operator = (const WorldHandler&) = delete;

public:
	struct Settings {
		int populationSize;
		std::vector<std::string> populationNames;

		int tickInterval;
	};

	static void Init();
	static void SetSettings();
	static Settings* GetSettings();

private:
	Settings settings;

	static void SetDefaultSettings();
	static void SetPopulation();
	static void SetTickInterval();
};