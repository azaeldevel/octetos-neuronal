
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
	
	datatype gradientDecent(datatype)
	{
	
	}
	
	
	
	Perceptron::Perceptron()
	{
	
	}
	Perceptron::Perceptron(unsigned short ins) : inputs(ins), weight(ins)
	{
	
	}
	void Perceptron::set(unsigned short ins)
	{
		inputs.resize(ins);
		weight.resize(ins);
	}
	
	std::vector<datatype*>& Perceptron::get_inputs()
	{
		return inputs;
	}
	std::vector<datatype>& Perceptron::get_weight()
	{
		return weight;
	}
	datatype& Perceptron::get_out()
	{
		return out;
	}
	
	void Perceptron::set_inputs(const std::vector<datatype*>& v)
	{
		if(inputs.size() != v.size()) throw octetos::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
		for(unsigned short i = 0; i < v.size(); i++)
		{
			inputs[i] = v[i];
		}
	}
	
	
	datatype Perceptron::spread(datatype (*F)(datatype))
	{
		//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 1\n";
		datatype s = sigma();
		//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 2\n";
		out =  F(s);
		//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 3\n";
		return out;
	}

	
	datatype Perceptron::sigma()
	{
		//std::cout << "\tdatatype Perceptron::sigma step 1\n";
		datatype val = 0;
		//std::cout << "\tdatatype Perceptron::sigma step 2\n";
		if(inputs.size() != weight.size()) throw octetos::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
		//std::cout << "\tdatatype Perceptron::sigma step 3\n";
		
		for(unsigned short i = 0; i < inputs.size(); i++)
		{
			//std::cout << "\tdatatype Perceptron::sigma step 3.1 - " << i << "\n";
			val += (*inputs[i])*weight[i];
		}
		
		//std::cout << "\tdatatype Perceptron::sigma step 4\n";
		return val;
	}
	void Perceptron::resize(unsigned short ins)
	{
		inputs.resize(ins);
		weight.resize(ins);
	}
	
	
	
	
	
	
	
}