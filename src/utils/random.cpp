#include "utils/random.hpp"
#include <chrono>

Random* Random::singletonPtr = nullptr;

Random::Random() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	generator.seed(seed);
}

Random::~Random() {}

void Random::Init() {
	if (singletonPtr == nullptr) {
		singletonPtr = new Random();
	}
}

double Random::GetRandNormalDistr(const double inf, const double sup, const double mean, const double stddev) {
	std::normal_distribution<double> d(mean, stddev);
	double ret;
	
	do {
		ret = d(singletonPtr->generator);
	} while (ret < inf || ret > sup);

	return ret;
}

double Random::GetRandUniformDistr(const double inf, const double sup) {
	std::uniform_real_distribution<double> distribution(inf, sup);
	return distribution(singletonPtr->generator);
}

int Random::GetRandBinomialDistr(const int inf, const int sup, const double p) {
	std::binomial_distribution<int> d(sup - inf, p);
	return inf + d(singletonPtr->generator);
}

int Random::GetRandUniformDistr(const int inf, const int sup) {
	std::uniform_int_distribution<int> distribution(inf, sup);
	return distribution(singletonPtr->generator);
}

int Random::GetRandIndexProbField(const std::vector<double>& weights) {
	if (weights.size() <= 1)
		return (int)weights.size() - 1;

	double sum = 0;
	for (const auto& w : weights)
		sum += w;

	double r = GetRandUniformDistr(0.0, sum);
	for (int i = 0; i < weights.size(); ++i) {
		r -= weights[i];
		if (r < 0)
			return i;
	}

	return -1;
}
