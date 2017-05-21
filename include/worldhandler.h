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
	};

	static void Init();
	static Settings* GetSettings();

private:
	Settings settings;

	static void SetPopulation();
};