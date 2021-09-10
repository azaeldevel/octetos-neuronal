
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "network.hh"

namespace oct::neu
{
	
	
	Topology::Topology(Index s) : std::vector<Model>(s)
	{
	}
	Topology::Topology(ActivationFuntion AF,Index width, Index heightLayer, unsigned int inputs, unsigned int outputs) : std::vector<Model>(width)
	{
		for(Index i = 0 ; i < width ; i++)
		{
			std::vector<Model>::at(i).AF = AF;
		}
		
		std::vector<Model>::at(0).height = inputs;
		for(Index i = 1 ; i < width - 1; i++)
		{
			std::vector<Model>::at(i).height = heightLayer;
		}
		std::vector<Model>::at(width-1).height = outputs;
	}
	Topology::Topology(const Topology& t) : std::vector<Model>(t)
	{
		//inputsNeurona = t.inputsNeurona;
		
	}
	
	
	
	
	
}
