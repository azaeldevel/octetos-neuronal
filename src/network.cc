
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "network.hh"

namespace oct::neu
{
	
	
	Topology::Topology(Index s) : std::vector<Model>(s)
	{
	}
	Topology::Topology(Index s,ActivationFuntion AF,unsigned int inputs, Index heightLayer, unsigned int outputs) : std::vector<Model>(s)
	{
		for(Index i = 0 ; i < s ; i++)
		{
			std::vector<Model>::at(i).AF = AF;
		}
		
		std::vector<Model>::at(0).height = inputs;
		for(Index i = 1 ; i < s - 1; i++)
		{
			std::vector<Model>::at(i).height = heightLayer;
		}
		std::vector<Model>::at(s-1).height = outputs;
	}
	Topology::Topology(const Topology& t) : std::vector<Model>(t)
	{
		//inputsNeurona = t.inputsNeurona;
		
	}
	
	
	
	
	
}
