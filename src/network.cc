
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "Perceptron.hh"

namespace oct::neu
{
	Network::Network(const LayerWidth& lw,datatype (*fa)(datatype),unsigned short insP) : layerWidth(lw),FA(fa),inputsPerpceptron(insP)
	{
		if(layerWidth[0] != 1) throw octetos::core::Exception("La primera capa deve tener 1 como valor",__FILE__,__LINE__);
		
		resize(layerWidth.size()+1);
		
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
