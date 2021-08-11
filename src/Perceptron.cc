
#include <octetos/core/Error.hh>
#include <cmath>


#include "Perceptron.hh"

namespace oct::neu
{
	
	datatype sigmoide(datatype v)
	{
		return 1.0/(1.0 + exp(-1.0 * v));
	}
	
	
	
	
	
	
	Perceptron::Perceptron(unsigned short ins, datatype u, datatype (*f)(datatype)) : values(ins), weight(ins),F(f),umbral(u)
	{
	
	}
		
	
	std::vector<datatype>& Perceptron::get_values()
	{
		return values;
	}
	std::vector<datatype>& Perceptron::get_weight()
	{
		return weight;
	}
	datatype& Perceptron::get_out()
	{
		return out;
	}
	
	
	void Perceptron::spread()
	{
		datatype s = sigma();
		out = (s >= umbral) ? F(s) : 0.0;
	}

	
	datatype Perceptron::sigma()
	{
		datatype val = 0;
		if(values.size() != weight.size()) throw octetos::core::Exception("El vector de valores y de pesos tienen diferente tamnano",__FILE__,__LINE__);
		for(unsigned short i = 0; i < values.size(); i++)
		{
			val += values[i]*weight[i];
		}
		
		return val;
	}
	
	
}