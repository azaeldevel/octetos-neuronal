
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "network.hh"

namespace oct::neu
{
	
	
	Topology::Topology(Index s) : std::vector<Model>(s)
	{
	}
	Topology::Topology(Index s,ActivationFuntion AF) : std::vector<Model>(s)
	{
		for(Model& model : *this)
		{
			model.AF = AF;
		}
	}
	Topology::Topology(const Topology& t) : std::vector<Model>(t)
	{
		inputsNeurona = t.inputsNeurona;
		
	}
	
	
	
	
	
}
