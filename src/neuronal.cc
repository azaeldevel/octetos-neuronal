
#include <random>

#include "neuronal.hh"

namespace oct::neu
{
	double randNumber()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> distr(0.0, 1.0);
		
		return distr(gen);
	}
	double randNumber(double max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> distr(0.0, max);
		
		return distr(gen);
	}
	double randNumber(double min,double max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> distr(min, max);
		
		return distr(gen);
	}
}