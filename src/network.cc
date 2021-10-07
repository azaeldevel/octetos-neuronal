
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
	
	
	
	
	
	
	
		Network::Network(const Topology& t,unsigned short countInputs,unsigned short countOutputs) : topology(t)
		{
			//std::cout << "Network::Network(---) = step 1\n";
			if(topology.size() < 3) throw oct::core::Exception("La red deve tener 1 capa de entrada, al menos 1 capa oculta y 1 de salida",__FILE__,__LINE__);
			if(topology.at(0).height != countInputs) throw oct::core::Exception("La cantidad de entradas no coincide",__FILE__,__LINE__);
			//std::cout << topology.at(topology.size() - 1).height << " != " << countOutputs << "\n";
			if(topology.at(topology.size() - 1).height != countOutputs) throw oct::core::Exception("La cantidad de salidas no coincide",__FILE__,__LINE__);
			//std::cout << "Network::Network(---) = step 2\n";
			//if(topology.size() - 2 == counHidden) throw oct::core::Exception("La cantidad de capaz oculpas es incorrecta.",__FILE__,__LINE__);
			
			std::vector<Layer<DATATYPE>>::resize(topology.size());
			
			//std::cout << "Network::Network(---) = step 3 - " << std::vector<Layer<T>>::size() << "\n";
			//cada neurona se conecta con todas las salidas de la capa anterior
			topology.at(0).inputsNeurona = 1;
			std::vector<Layer<DATATYPE>>::at(0).set(topology.at(0));
			for(unsigned short i = 1; i < topology.size(); i++)
			{
				topology.at(i).inputsNeurona = topology.at(i-1).height;
				std::vector<Layer<DATATYPE>>::at(i).set(topology.at(i));
			}

			//std::cout << "Network::Network(---) = step 4\n";
			conecting();
		}
	
		const std::vector<DATATYPE*>& Network::spread(const std::vector<DATATYPE>& ds)
		{
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 1\n";
			//std::cout << std::vector<Layer<T>>::at(0).size() << " != " << ds.size() << "\n";
			if(std::vector<Layer<DATATYPE>>::at(0).size() != ds.size()) throw oct::core::Exception("Los vectores de datos no coinciden",__FILE__,__LINE__);
			ins = ds;
			//print(ins);
			//std::cout << "\n";
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 2\n";
			for(unsigned short i = 0; i < std::vector<Layer<DATATYPE>>::at(0).size(); i++)
			{
				//std::cout << "\tEntradas por neuraona = " << at(0).at(i).get_inputs().size << "\n";
				if(std::vector<Layer<DATATYPE>>::at(0).at(i).get_inputs().size() != 1) throw oct::core::Exception("La cantidad de entradas por neurona deve ser 1 en la primer capa.",__FILE__,__LINE__);
				std::vector<Layer<DATATYPE>>::at(0).at(i).get_inputs()[0] = &ins[i];//se asigna cada dato a cada entrada
			}
			
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 3\n";
			for(unsigned short i = 0; i < std::vector<Layer<DATATYPE>>::size(); i++)
			{
				//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 3.1 - " << i << "\n";
				std::vector<Layer<DATATYPE>>::at(i).spread();	
			}
			
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 4\n";		
			outs = &(std::vector<Layer<DATATYPE>>::at(std::vector<Layer<DATATYPE>>::size() - 1).get_outputs());//se usa la salida de la ultima capa

			
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 5\n";
			return *outs;
		}
				
		Index Network::max(const Layer<DATATYPE>& layer)
		{
			Index maxIndex = 0;
			DATATYPE maxFound = *layer.get_outputs()[maxIndex];
			for(Index i = 0; i < layer.size(); i++)
			{
				if(*layer.get_outputs()[i] > maxFound) 
				{
					maxFound = *layer.get_outputs()[i];
					maxIndex = i;
				}
			}

			return maxIndex;
		}
		
		Index Network::max_weight(const Perceptron<DATATYPE>& perceptron)
		{
			Index maxIndex = 0;
			DATATYPE maxFound = perceptron.weight[maxIndex];
			for(Index i = 0; i < perceptron.weight.size(); i++)
			{
				if(perceptron.weight[i] > maxFound) 
				{
					maxFound = perceptron.weight[i];
					maxIndex = i;
				}
			}

			return maxIndex;
		}
		
		void Network::display(const Data<DATATYPE>& data,oct::math::Plotter* plotGraph, const Learning<DATATYPE>& learning)
		{
			plotGraph->set_terminal("qt");
			//std::string titleplot = "funcion";
			//plotGraph->set_title(titleplot);
			std::string labelRatio = "ratio = ";
			labelRatio += std::to_string(learning.ratio);
			plotGraph->set_label(labelRatio,100,0.15);
			std::string labeldEdR = "max dEdR = ";
			labeldEdR += std::to_string(learning.mE);
			plotGraph->set_label(labeldEdR,100,0.10);
			if(data.inputs.size() < 2 or data.inputs.size() > 3) throw oct::core::Exception("Dimension no manejable",__FILE__,__LINE__);

			std::vector<DATATYPE> vecdat(data.inputs.size() + 1);
			if(data.inputs.size() > 1) vecdat[0] = data.inputs[0];
			if(data.inputs.size() > 2) vecdat[1] = data.inputs[1];
			if(std::vector<Layer<DATATYPE>>::at(std::vector<Layer<DATATYPE>>::size()-1).get_outputs().size() == 1) vecdat[2] = *std::vector<Layer<DATATYPE>>::at(std::vector<Layer<DATATYPE>>::size()-1).get_outputs()[0];
			else throw oct::core::Exception("Dimension no manejable",__FILE__,__LINE__);
		}
		
		void Network::conecting()
		{
			//std::cout << "\n";
			for(unsigned short i = 1; i < std::vector<Layer<DATATYPE>>::size(); i++)//cada capa
			{
				//std::cout << "Cantidd de Layers = " << size() << "\n";
				for(unsigned short j = 0; j < std::vector<Layer<DATATYPE>>::at(i).size(); j++)//cada neurona de la capa i
				{
					//std::cout << "\tCantidd de neuraonas = " << at(i).size() << "\n";
					for(unsigned short k = 0; k < topology.at(i).inputsNeurona; k++)
					{
						std::vector<Layer<DATATYPE>>::at(i).at(j).get_inputs().at(k) = &(std::vector<Layer<DATATYPE>>::at(i-1).at(k).get_out());						
					}
				}	
			}
		}
		
		
		void Network::errorToMuchInputsRequiered(unsigned short i,const char* f, unsigned int l)
		{
			std::string msg = "La capa '";
			msg += std::to_string(i) + "' soporta un maximo de " + std::to_string(topology[i].height * inputsPerpceptron) + ", sin embargo la capa '";
			msg += std::to_string(i-1) + "' requiere " + std::to_string(topology[i-1].height);
			throw octetos::core::Exception(msg,f,l);
		}
		
		bool Network::trainig(const std::vector<Data<DATATYPE>>& datas, Learning<DATATYPE>& learning, Plotting<DATATYPE>* plotting, Index count)
		{
			for (Index i = 0; i < count; i++)
			{
				if(bp(datas,learning,plotting)) return true;
			}
			
			return false;
		}
}
