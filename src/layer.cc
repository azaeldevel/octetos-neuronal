
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "Perceptron.hh"

namespace oct::neu
{
	
	
	
	Layer::Layer()
	{
	
	}
	Layer::Layer(unsigned short inputsP, unsigned short countP, datatype (*fa)(datatype)) : std::vector<Perceptron>(countP),FA(fa)
	{
		set(inputsP,countP,fa);
	}
	void Layer::set(unsigned short inputsP, unsigned short countP, datatype (*fa)(datatype))
	{
		if(size() < countP)resize(countP);
		
		FA = fa;
		for(Perceptron& p : *this)
		{
			p.set(inputsP);
		}
		gradient.resize(countP);
		gradient_unit.resize(countP);
		gradient_descent.resize(countP);
	}
	
	std::vector<datatype>& Layer::get_gradient()
	{
		return gradient;
	}
	std::vector<datatype>& Layer::get_gradient_unit()
	{
		return gradient_unit;
	}
	std::vector<datatype>& Layer::get_gradient_descent()
	{
		return gradient_descent;
	}
	
	void Layer::print(const std::vector<datatype>& v)
	{
		std::cout << "(";
		for(unsigned short i = 0; i < v.size(); i++)
		{
			std::cout << v[i];
			if(i < v.size() - 1) std::cout << ",";
		}
		std::cout << ")";
	}
	void Layer::spread(const std::vector<datatype>& c)
	{
		for(Perceptron& p : *this)
		{
			//p.set_inputs(c);
			//p.spread(FA);			
		}
		/*//calculanbdo la derivada de cada funcion
		for(unsigned short i = 0; i < size(); i++)
		{
			gradient[i] = sigmoide_dev(at(i).get_out());
		}
		//calculando vector unitario
		datatype leght = 0;
		for(unsigned short i = 0; i < size(); i++)
		{
			leght += std::pow(gradient[i],2.0);//suma de cuadrados
		}
		leght = std::sqrt(leght);
		for(unsigned short i = 0; i < size(); i++)
		{
			gradient_unit[i] = gradient[i]/leght;
		}*/		
	}
	void Layer::spread()
	{
		//std::cout << "\tvoid Layer::spread(): step 1 \n";
		for(unsigned short i = 0; i < size(); i++)
		{
			//std::cout << "\tvoid Layer::spread(): step 1.1\n";
			at(i).spread(FA);			
		}	
		//std::cout << "\tvoid Layer::spread(): step 2\n";
	}
	void Layer::learning(const Cases& cases)
	{
		for(const Case& c : cases)
		{
			
		}
	}
	
}