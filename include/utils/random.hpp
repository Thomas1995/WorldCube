#pragma once

#include <random>
#include <vector>

class Random {
	static Random* singletonPtr;

	std::default_random_engine generator;

	Random();
	~Random();

	Random(const Random&) = delete;
	void operator = (const Random&) = delete;

public:
	static void Init();

	static double GetRandNormalDistr(const double mean = 0, const double stddev = 1);
	static double GetRandNormalDistrBordered(const double inf, const double sup, const double mean = 0, const double stddev = 1);
	static double GetRandUniformDistr(const double inf, const double sup);
	
	static int GetRandBinomialDistr(const int inf, const int sup, const double p = 0.5);
	static int GetRandUniformDistr(const int inf, const int sup);

	static int GetRandIndexProbField(const std::vector<double>& weights);
};