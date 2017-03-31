#include "utils/random.hpp"

Random* Random::singletonPtr = nullptr;

Random::Random() {}
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