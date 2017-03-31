#include "utils/random.hpp"

Random* Random::singletonPtr = nullptr;

Random::Random() {}
Random::~Random() {}

void Random::Init() {
	if (singletonPtr == nullptr) {
		singletonPtr = new Random();
	}
}

double Random::GetRandNormalDistr(double inf, double sup, double mean, double stddev) {
	std::normal_distribution<double> d(mean, stddev);
	double ret;
	
	do {
		ret = d(singletonPtr->generator);
	} while (ret < inf || ret > sup);

	return ret;
}

double Random::GetRandUniformDistr(double inf, double sup) {
	std::uniform_real_distribution<double> distribution(inf, sup);
	return distribution(singletonPtr->generator);
}

int Random::GetRandBinomialDistr(int inf, int sup, double p) {
	std::binomial_distribution<int> d(sup - inf, p);
	return inf + d(singletonPtr->generator);
}

int Random::GetRandUniformDistr(int inf, int sup) {
	std::uniform_int_distribution<int> distribution(inf, sup);
	return distribution(singletonPtr->generator);
}