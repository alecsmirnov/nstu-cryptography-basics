#include "LCGenerator.h"

#include <algorithm>

const std::vector<double> LCGenerator::chi_square_quantiles = {
		3.841,   5.991,  7.815,  9.488, 11.070,
		12.592, 14.067, 15.507, 16.919, 18.307,
		19.675, 21.026, 22.362, 23.685, 24.996
};

void LCGenerator::setInitialValue(std::size_t K) {
	this->K = K;
}

void LCGenerator::setAlphabetPow(std::size_t N) {
	this->N = N;
}

void LCGenerator::addLCGVector(LCGVector A) {
	B.push_back(A);
}

void LCGenerator::addLCGVector(std::size_t a, std::size_t b) {
	B.push_back({a, b});
}

std::size_t LCGenerator::random() {
	K = (B[t].a * K + B[t].b) % N;
	t = ++t % B.size();

	return K;
}

std::vector<std::size_t> LCGenerator::getSequence(std::size_t length) {
	std::vector<std::size_t> sequence(length);

	for (auto& value : sequence)
		value = random();

	return sequence;
}

std::size_t LCGenerator::getPeriodLength(std::size_t sequence_length) {
	std::size_t period = 1, step = 0;
	std::vector<std::size_t> sequence = getSequence(2 * sequence_length);

	while (period + step != sequence.size())
		if (sequence[step] != sequence[period + step]) {
			++period;

			step = 0;
		}
		else
			++step;

	return period;
}

double LCGenerator::getPearsonChiSquared(std::vector<std::size_t> sequence, std::size_t intervals_count) {
	std::sort(sequence.begin(), sequence.end());

	std::size_t interval_len = N / intervals_count;
	std::vector<double> v(intervals_count);

	std::size_t step = 0;
	for (std::size_t i = 0; i != intervals_count; ++i) {
		std::size_t count = 0;
		for (std::size_t j = step; j != sequence.size() && sequence[j] < interval_len * (i + 1); ++j)
			++count;

		v[i] = 1.0 * count / sequence.size();
		step += count;
	}

	double S = 0;

	double P = 1.0 / intervals_count;
	for (std::size_t i = 0; i !=  intervals_count; ++i)
		S += (v[i] - P) * (v[i] - P) / P;

	return sequence.size() * S;
}

double LCGenerator::getQuantile(int r) {
	return chi_square_quantiles[r - 1];
}

bool LCGenerator::testPearsonHypothesis(double S, double Skp) {
	return S < Skp;
}

void LCGenerator::clearB() {
	t = 0;
	std::vector<LCGVector>().swap(B);
}

LCGenerator::~LCGenerator() {
	clearB();
}
