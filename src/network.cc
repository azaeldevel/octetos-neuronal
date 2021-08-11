
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "Perceptron.hh"

namespace oct::neu
{
	void Network::errorToMuchInputsRequiered(unsigned short i,const char* f, unsigned int l)
	{
		std::string msg = "La capa '";
		msg += std::to_string(i) + "' soporta un maximo de " + std::to_string(layerWidth[i] * inputsPerpceptron) + ", sin embargo la capa '";
		msg += std::to_string(i-1) + "' requiere " + std::to_string(layerWidth[i-1]);
		throw octetos::core::Exception(msg,f,l);
	}
	Network::Network(const LayerWidth& lw,datatype (*fa)(datatype),unsigned short insP) : layerWidth(lw),FA(fa),inputsPerpceptron(insP)
	{		
		for(unsigned short i = 2; i < layerWidth.size(); i++)
		{
			if(layerWidth[i] * insP < layerWidth[i-1]) errorToMuchInputsRequiered(i,__FILE__,__LINE__);
		}
		resize(layerWidth.size() + 1);
		
		at(0).set(1,layerWidth[0],fa);
		for(unsigned short i = 1; i < layerWidth.size(); i++)
		{
			at(i).set(insP,layerWidth[i],fa);
		}
		
		conecting();
	}
	
	void Network::conecting()
	{
		for(unsigned short i = 0; i < layerWidth.size(); i++)
		{
			
		}
	}
}
