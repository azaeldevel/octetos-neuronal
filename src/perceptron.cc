
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "Perceptron.hh"

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
	
	datatype gradientDecent(datatype)
	{
	
	}
	
	
	
	Perceptron::Perceptron()
	{
	
	}
	Perceptron::Perceptron(unsigned short ins) : values(ins), weight(ins)
	{
	
	}
	void Perceptron::set(unsigned short ins)
	{
		values.resize(ins);
		weight.resize(ins);
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
	
	void Perceptron::set_values(const std::vector<datatype>& v)
	{
		if(values.size() != v.size()) throw octetos::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
		for(unsigned short i = 0; i < v.size(); i++)
		{
			values[i] = v[i];
		}
	}
	
	
	datatype Perceptron::spread(datatype (*F)(datatype))
	{
		datatype s = sigma();
		out =  F(s);
		return out;
	}

	
	datatype Perceptron::sigma()
	{
		datatype val = 0;
		if(values.size() != weight.size()) throw octetos::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
		for(unsigned short i = 0; i < values.size(); i++)
		{
			val += values[i]*weight[i];
		}
		
		return val;
	}
	
	
	
	
	
	
	
	
}