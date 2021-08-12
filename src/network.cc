
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "neuronal.hh"

namespace oct::neu
{
	
	Network::Network(const LayerWidth& lw,datatype (*fa)(datatype),unsigned short insP,unsigned short outsP) 
		: FA(fa),inputsPerpceptron(insP),outsPerpceptron(outsP)
	{
		//std::cout << "Network::Network(---) = step 1\n";
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
		//std::cout << "Network::Network(---) = step 2\n";
		
		resize(layerWidth.size());
		
		//std::cout << "Network::Network(---) = step 3 - " << size() << "\n";
		at(0).set(1,layerWidth[0],fa);
		//std::cout << "Network::Network(---) = step 3.1 - " << 0 << " - " << at(0).size() << "\n";
		for(unsigned short i = 1; i < layerWidth.size(); i++)
		{
			at(i).set(insP,layerWidth[i],fa);//configurar cada capa
			//std::cout << "Network::Network(---) = step 3.1 - " << i << " - " << at(i).size() << "\n";
		}
		
		//std::cout << "Network::Network(---) = step 4\n";
		conecting();
	}
	Network::~Network()
	{
	}
	void Network::conecting()
	{		
		//std::cout << "\n";
		for(unsigned short i = 1; i < size(); i++)
		{
			//std::cout << "Cantidd de Layers = " << size() << "\n";
			for(unsigned short j = 0; j < at(i).size(); j++)
			{
				//std::cout << "\tCantidd de neuraonas = " << at(i).size() << "\n";
				for(unsigned short k = 0; k < at(i).at(j).get_inputs().size(); k++)
				{
					if(k < at(i-1).size()) 
					{
						at(i).at(j).get_inputs().at(k) = &(at(i-1).at(k).get_out());
					}
					else 
					{
						at(i).at(j).resize(k);
						break;
					}
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
	oct::math::Vector<datatype*>& Network::spread(oct::math::Vector<datatype>& out)
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
		for(unsigned short i = 0; i < size(); i++)
		{
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 3.1 - " << i << "\n";
			at(i).spread();	
		}
		
		//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 4\n";		
		outs = &(at(size() - 1).get_outputs());
		
		//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 5\n";
		return *outs;
	}
	
	void Network::learning(const Datas& ds)
	{
		//std::cout << "void learning(const Datas& ds) Step 1\n";
		spread(*ds[0]);
		for(unsigned short i = 0; i < size(); i++)
		{
			//oct::neu::Layer::print(at(i).get_gradient());
			//oct::neu::Layer::print(at(i).get_outputs());
			//std::cout << "\n";
		}
		//std::cout << "void learning(const Datas& ds) Step 2\n";
		oct::neu::Layer::print(at(size()-1).get_outputs());
		std::cout << "\n";
		//std::cout << "void learning(const Datas& ds) Step 3\n";
		unsigned short lastlayer = size()-1;
		oct::math::Vector<datatype> newvect;
		//std::cout << "void learning(const Datas& ds) Step 4\n";
		at(lastlayer).minimize(50,0.01,newvect);
		//std::cout << "void learning(const Datas& ds) Step 5\n";
		oct::neu::Layer::print(newvect);
		std::cout << "\n";
		//std::cout << "void learning(const Datas& ds) Step 6\n";
	}
	
}
