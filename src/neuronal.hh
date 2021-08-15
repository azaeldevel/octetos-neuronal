
#ifndef OCTETOS_NEURONAL_HH
#define OCTETOS_NEURONAL_HH

#include <vector>
#include <octetos/core/Exception.hh>
#include <cmath>
#include <iostream>

namespace oct::neu
{
	typedef unsigned short Index;

	double randNumber();
	double randNumber(double max);
	double randNumber(double min, double max);
}

#endif