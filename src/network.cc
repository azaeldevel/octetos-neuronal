
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
	}
	void Network::conecting()
	{
		dendrities = new datatype** [layerWidth.size()];
		for(unsigned short i = 0; i < layerWidth.size(); i++)
		{
			dendrities[i] = new datatype* [outsPerpceptron];
		}
		
		//std::cout << "\n";
		for(unsigned short i = 1; i < size(); i++)
		{
			//std::cout << "Cantidd de Layers = " << size() << "\n";
			for(unsigned short j = 0; j < at(i).size(); j++)
			{
				//std::cout << "\tCantidd de neuraonas = " << at(i).size() << "\n";
				for(unsigned short k = 0; k < at(i).at(j).get_inputs().size(); k++)
				{	
					if(k < at(i-1).size()) at(i).at(j).get_inputs().at(k) = &(at(i-1).at(k).get_out());
					else at(i).at(j).resize(k);					
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
	std::vector<datatype>& Network::spread(std::vector<datatype>& out)
	{
		//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 1\n";
		if(at(0).size() != out.size()) throw octetos::core::Exception("La cantidad de entradas en la red no coincide con la cantidad de datos de entradas",__FILE__,__LINE__);
		
		//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 2\n";
		for(unsigned short i = 0; i < at(0).size(); i++)
		{
			//std::cout << "\tEntradas por neuraona = " << at(0).at(i).get_inputs().size << "\n";
			if(at(0).at(i).get_inputs().size() != 1) throw octetos::core::Exception("La cantidad de entradas por neurona deve ser 1 en la primer capa.",__FILE__,__LINE__);
			at(0).at(i).get_inputs()[0] = &out[i];
		}
		
		//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 3\n";
		for(unsigned short i = 0; i < at(i).size(); i++)
		{
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 3.1 - " << i << "\n";
			at(i).spread();	
		}
		
		//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 4\n";
		outs.clear();
		outs.resize(at(size() - 1).size());
		//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 4.1\n";
		for(unsigned short i = 0; i < at(size() - 1).size(); i++)
		{
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 4.2\n";
			outs[i] = at(size() - 1).at(i).get_out();
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 4.3\n";
		}
		
		//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 5\n";
		return outs;
	}
	
}
