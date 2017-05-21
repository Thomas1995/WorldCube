#include <iostream>
#include <algorithm>
#include "worldhandler.h"

WorldHandler* WorldHandler::singletonPtr = nullptr;

WorldHandler::WorldHandler() {}
WorldHandler::~WorldHandler() {}

void WorldHandler::Init() {
	if (singletonPtr == nullptr) {
		singletonPtr = new WorldHandler();
		
		std::cout << "Welcome to WorldCube!" << std::endl;
	}
}

void WorldHandler::SetSettings() {
	char ans;
	std::cout << "Would you like the default settings? (Y/n) ";
	std::cin >> ans;
	if (ans == 'Y')
		SetDefaultSettings();
	else {
		SetPopulation();
		SetTickInterval();
	}
}

void WorldHandler::SetDefaultSettings() {
	singletonPtr->settings.populationSize = 10;
	singletonPtr->settings.tickInterval = 1;
}

void WorldHandler::SetPopulation() {
	int populationSize;
	int namedSubjects;
	char ans;
	std::string name;

	while (true) {
		std::cout << "You will now set population. How big the population should be? Write a positive integer below." << std::endl;
		std::cin >> populationSize;

		if (populationSize <= 0)
			std::cout << "The population should be at least 1! Let's try again." << std::endl;
		else
			break;
	}

	singletonPtr->settings.populationSize = populationSize;

	std::cout << "Would you like to give your subjects names? (Y/n) ";
	std::cin >> ans;
	if (ans == 'Y') {
		std::cout << "How many people would you like to name? ";
		std::cin >> namedSubjects;
		if (namedSubjects <= 0)
			namedSubjects = 0;
		else {
			std::cout << "Write one name on each line." << std::endl;
			namedSubjects = std::min(namedSubjects, populationSize);

			// get the invalid line
			std::getline(std::cin, name);

			for (int i = 0; i < namedSubjects; ++i) {
				std::getline(std::cin, name);
				singletonPtr->settings.populationNames.push_back(name);
			}
		}	
	}

	std::cout << "Population set!" << std::endl;
}

void WorldHandler::SetTickInterval() {
	int tickInt;

	while (true) {
		std::cout << "Choose the tick interval. It represents the number of milliseconds in real life corresponding to the seconds in WorldCube. For bigger population, a higher value would be advisable. 0 is an option." << std::endl;
		std::cin >> tickInt;

		if (tickInt < 0)
			std::cout << "The value should not be negative! Let's try again." << std::endl;
		else
			break;
	}

	singletonPtr->settings.tickInterval = tickInt;
}

WorldHandler::Settings* WorldHandler::GetSettings() {
	return &singletonPtr->settings;
}