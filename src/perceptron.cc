
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "neuronal.hh"

namespace oct::neu
{
	datatype sigmoide(datatype v)
	{
		return 1.0/(1.0 + exp(-1.0 * v));
	}
	datatype sigmoide_dev(datatype v)
	{
		return v*(1.0-v);
	}
}