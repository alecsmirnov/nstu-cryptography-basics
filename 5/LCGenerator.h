#ifndef LCGENERATOR_H
#define LCGENERATOR_H

#include <vector>

struct LCGVector {
	std::size_t a;
	std::size_t b;
};

class LCGenerator {
public:
	LCGenerator() : K(1), t(0) {}

	void setInitialValue(std::size_t K);
	void setAlphabetPow(std::size_t N);

	void addLCGVector(LCGVector A);
	void addLCGVector(std::size_t a, std::size_t b);

	std::size_t random();
	std::vector<std::size_t> getSequence(std::size_t length);
	std::size_t getPeriodLength(std::size_t sequence_length);
	double getPearsonChiSquared(std::vector<std::size_t> sequence, std::size_t intervals_count);
	static double getQuantile(int r);
	static bool testPearsonHypothesis(double S, double Skp);
	
	void clearB();

	~LCGenerator();

private:
	std::size_t K;
	std::size_t t;

	std::size_t N;

	std::vector<LCGVector> B;

	static const std::vector<double> chi_square_quantiles;
};

#endif
