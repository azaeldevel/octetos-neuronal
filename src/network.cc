
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "Perceptron.hh"

namespace oct::neu
{
	
	Network::Network(const LayerWidth& lw,datatype (*fa)(datatype),unsigned short insP,unsigned short outsP) 
		: FA(fa),inputsPerpceptron(insP),outsPerpceptron(outsP)
	{
		//validacion de entradas
		for(unsigned short i = 2; i < lw.size(); i++)
		{
			if(lw[i] * insP < lw[i-1]) errorToMuchInputsRequiered(i,__FILE__,__LINE__);
		}
		layerWidth.resize(lw.size());
		for(unsigned short i = 0; i < lw.size(); i++)
		{
			layerWidth[i] = lw[i];
		}
		
		resize(layerWidth.size());
		
		at(0).set(1,layerWidth[0],fa);
		for(unsigned short i = 1; i < layerWidth.size(); i++)
		{
			at(i).set(insP,layerWidth[i],fa);//configurar cada capa
		}
		
		conecting();
	}
	Network::~Network()
	{
		for(unsigned short i = 0; i < layerWidth.size(); i++)
		{
			/*for(unsigned short j = 0; j < outsPerpceptron; j++)
			{
				delete[] dendrities[i][j];
			}*/
			//delete[] dendrities[i];
		}
		//delete[] dendrities;
	}
	void Network::conecting()
	{
		//dendrities.resize(layerWidth.size() * inputsPerpceptron);//ajustando la cantidad de dendritas posibles
		dendrities = new datatype** [layerWidth.size()];
		for(unsigned short i = 0; i < layerWidth.size(); i++)
		{
			dendrities[i] = new datatype* [outsPerpceptron];
			/*for(unsigned short j = 0; j < outsPerpceptron; j++)
			{
				dendrities[i][j] = new unsigned short [inputsPerpceptron];
			}*/
		}
		
		std::cout << "\n";
		for(unsigned short i = 1; i < size(); i++)
		{
			//std::cout << "Cantidd de Layers = " << size() << "\n";
			for(unsigned short j = 0; j < at(i).size(); j++)
			{
				//std::cout << "\tCantidd de neuraonas = " << at(i).size() << "\n";
				for(unsigned short k = 0; k < at(i).at(j).get_inputs().size(); k++)
				{	
					if(k < at(i-1).size()) at(i).at(j).get_inputs().at(k) = &at(i-1).at(k).get_out();						
				}
			}		
		}
	}
	void Network::errorToMuchInputsRequiered(unsigned short i,const char* f, unsigned int l)
	{
		std::string msg = "La capa '";
		msg += std::to_string(i) + "' soporta un maximo de " + std::to_string(layerWidth[i] * inputsPerpceptron) + ", sin embargo la capa '";
		msg += std::to_string(i-1) + "' requiere " + std::to_string(layerWidth[i-1]);
		throw octetos::core::Exception(msg,f,l);
	}
}
