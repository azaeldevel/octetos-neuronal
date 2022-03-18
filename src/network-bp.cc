
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "network.hh"

namespace oct::neu
{
		
		bool Network::bp(const std::vector<Data<DATATYPE>>& datas, Learning<DATATYPE>& learning, Plotting<DATATYPE>* plotting)
		{
			//std::cout << "Step 1.1.1.0\n";
			Index lastLayer = size() - 1;
			outs = &std::vector<Layer<DATATYPE>>::at(lastLayer).get_outputs();
			if(outs->size() != datas[0].outputs.size()) throw oct::core::Exception("La cantidad de entradas no coincide",__FILE__,__LINE__);
			std::ofstream filePlotting;
			if(plotting)
			{
				filePlotting.open(plotting->filename,std::ios::app);
				plotting->plotter.set_terminal("qt");
				std::string labelRatio = "ratio = ";
				labelRatio += std::to_string(learning.ratio);
				plotting->plotter.set_label(labelRatio,10,0.15);
				std::string labeldEdR = "max dEdR = ";
				labeldEdR += std::to_string(learning.epsilon);
				plotting->plotter.set_label(labeldEdR,10,0.10);
				std::string labelCountData = "Data = ";
				labelCountData += std::to_string(datas.size());
				plotting->plotter.set_label(labelCountData,10,0.05);
			}

			//std::cout << "Step 1.1.2.0\n";
			DATATYPE E_prev = 0;
			for(Index indexData = 0; indexData < datas.size(); indexData++)
			{
				//std::cout << "Step 1.1.2.1.0\n";
				/*
				if(it > 0) E_prev = E;
				//std::cout << "Step 1.1.2.2.0\n";
				E = dMSEdR(datas[it]);
				//std::cout << "Step 1.1.2.3.0\n";
				if(plotting != NULL)
				{
					plotting->last++;
					oct::math::Plotter::save(filePlotting,plotting->last,E);
					filePlotting.flush();
					plotting->plotter.plottingFile2D(plotting->filename);
				}
				//std::cout << "Se[lastLayer+2] " << Se[lastLayer+2] << "\n";
				
				if(E  < learning.epsilon) return true;

				//std::cout << "Step 1.1.2.5.0\n";
				if(learning.variable)
				{
					if(E  < E_prev) learning.ratio = learning.ratio * learning.r;
					else learning.ratio = learning.ratio * learning.p;
				}
				*/

				//std::cout << "Step 1.1.2.6.0\n";				
				real dEdR,dEdZ,step,S_out,S_hidden=0;
				for(Index it = 0; it < learning.iterations; it++)				
				{
					//std::cout << "Step 1.1.2.6.1.0\n";
					spread(datas[indexData].inputs);
					
					S_out = 0.0;
					for(Index out = 0; out < LAYER(lastLayer).size(); out++)
					{
						S_out += real(2) * (datas[indexData].outputs[out] - (*LAYER(lastLayer).get_outputs()[out]));
					}
					//std::cout << std::setprecision(6) << "S_out = " << S_out << "\n";
					
					for(Index neurona = 0; neurona < LAYER(lastLayer).size(); neurona++)
					{
						for(Index input = 0; input < NEURONA(lastLayer,neurona).inputs.size(); input++)
						{
							step = learning.ratio * S_out * dRdZ(lastLayer,neurona) * NEURONA(lastLayer,neurona).result;
							//std::cout << std::setprecision(6) << "step = " << step << "\n";
							WEIGHT(lastLayer,neurona,input) = WEIGHT(lastLayer,neurona,input) + step;	
						}				
					}
					
					//capa ocultas
					for(int indexLayer = lastLayer - 1; indexLayer >= 0; indexLayer--)
					{
						//std::cout << "Step 1.1.2.6.4.1.0\n";
						for(Index neurona = 0; neurona < LAYER(indexLayer).size(); neurona++)
						{
							//std::cout << "Step 1.1.2.6.4.1.2.0\n";
							for(Index weight = 0; weight < NEURONA(indexLayer,neurona).weight.size(); weight++)
							{
								//std::cout << "Step 1.1.2.1.0\n";
								step = learning.ratio * S_out * dRdZ(indexLayer,neurona) * NEURONA(indexLayer,neurona).result;
								//std::cout << "Step 1.1.2.2.0\n";
								WEIGHT(indexLayer,neurona,weight) = WEIGHT(indexLayer,neurona,weight) + step;
								//std::cout << "Step 1.1.2.3.0\n";
							}
						}
						//std::cout << "Step 1.1.2.6.4.2.0\n";
					}
					//std::cout << "Step 1.1.2.6.5.0\n";
					//std::cout << "Step 2.0\n";
				}
				//std::cout << "Step 1.1.2.7.0\n";
			}
			if(plotting)
			{
				plotting->plotter.set_terminal("svg");
				plotting->plotter.set_output("out.svg");
				plotting->plotter.plottingFile2D(plotting->filename);
				filePlotting.flush();
				filePlotting.close();
			}
			//std::cout << "Step 1.1.3.0\n";
			return false;
		}	
		
	DATATYPE Network::dMSEdR(const std::vector<Data<DATATYPE>>& datas)
	{
		//std::cout << "Step 1.1.2.2.1.0\n";
		DATATYPE e = 0, eO = 0;
		for(Index indexData = 0; indexData < datas.size(); indexData++)
		{
			spread(datas[indexData].inputs);
			for(Index i = 0; i < datas[indexData].outputs.size(); i++)
			{
				//std::cout << "e = " << e << "\n";
				eO += std::pow(datas[indexData].outputs[i] - (*LAYER(size()-1).get_outputs()[i]),real(2));
			}
			eO /= real(datas[indexData].outputs.size());
			e += eO;
			eO = 0;
		}
		e /= real(datas.size());
		return e;
	}
	DATATYPE Network::dMSEdR(const Data<DATATYPE>& data)
	{
		//std::cout << "Step 1.1.2.2.1.0\n";
		DATATYPE e = 0;
		Index indexData = 0;
		spread(data.inputs);
		//for(Index indexData = 0; indexData < datas.size(); indexData++)
		//{
			for(Index i = 0; i < data.outputs.size(); i++)
			{
				//std::cout << "e = " << e << "\n";
				e += std::pow(data.outputs[i] - (*LAYER(size()-1).get_outputs()[i]),real(2));
			}
			e /= real(data.outputs.size());
		//}
		e /= real(2);
		return e;
	}
	DATATYPE Network::dRdZ(Index layer,Index neurona)
	{
		switch(topology[layer].AF)
		{
		case ActivationFuntion::SIGMOID:
			return  Perceptron<DATATYPE>::sigmoid_D(NEURONA(layer,neurona).result);
		case ActivationFuntion::IDENTITY:
			return DATATYPE(1);
		case ActivationFuntion::RELU:
			return Perceptron<DATATYPE>::relu_D(NEURONA(layer,neurona).result);
		default:
			throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
		};
	}
	
	
}
