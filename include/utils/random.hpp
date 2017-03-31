#pragma once

#include <random>

class Random {
	static Random* singletonPtr;

	std::default_random_engine generator;

	Random();
	~Random();

	Random(const Random&) = delete;
	void operator = (const Random&) = delete;

public:
	static void Init();

	static double GetRandNormalDistr(double inf, double sup, double mean = 0, double stddev = 1);
	static double GetRandUniformDistr(double inf, double sup);
	
	static int GetRandBinomialDistr(int inf, int sup, double p = 0.5);
	static int GetRandUniformDistr(int inf, int sup);
};