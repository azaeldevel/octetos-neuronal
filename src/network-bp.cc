
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "network.hh"

namespace oct::neu
{
		bool Network::bp(const std::vector<Data<DATATYPE>>& datas, Learning<DATATYPE>& learning, Plotting<DATATYPE>* plotting)
		{
			Index lastLayer = size()-1;
			outs = &std::vector<Layer<DATATYPE>>::at(lastLayer).get_outputs();
			std::ofstream filePlotting;
			if(plotting != NULL)
			{
				filePlotting.open(plotting->filename,std::ios::app);
				plotting->plotter.set_terminal("qt");
				std::string labelRatio = "ratio = ";
				labelRatio += std::to_string(learning.ratio);
				plotting->plotter.set_label(labelRatio,10,0.15);
				std::string labeldEdR = "max dEdR = ";
				labeldEdR += std::to_string(learning.mE);
				plotting->plotter.set_label(labeldEdR,10,0.10);
				std::string labelCountData = "Data = ";
				labelCountData += std::to_string(datas.size());
				plotting->plotter.set_label(labelCountData,10,0.05);
			}
			
			std::vector<DATATYPE> Se;
			Se.resize(size()+2);
			DATATYPE E_prev = 0;
			for(Index it = 0; it < learning.iterations; it++)
			{		
				
				if(it > 0) E_prev = Se[lastLayer+2];
				Se[lastLayer+2] = dMSEdR(datas);
				if(plotting != NULL)
				{
					plotting->last++;
					oct::math::Plotter::save(filePlotting,plotting->last,Se[lastLayer+2]);
					filePlotting.flush();
					plotting->plotter.plottingFile2D(plotting->filename);
				}
				
				if(Se[lastLayer+2]  < learning.mE) return true;
				
				if(learning.variable)
				{
					if(Se[lastLayer+2]  < E_prev) learning.ratio = learning.ratio * learning.r;
					else learning.ratio = learning.ratio * learning.p;
				}
				
				real dEdZ,dRdZ,step,S=0;
				for(Index indexData = 0; indexData < datas.size(); indexData++)
				{
					
					dEdZ = (*LAYER(size()-1).get_outputs()[0]) - datas[indexData].outputs[0];

					//capa de salida
					for(Index neurona = 0; neurona < LAYER(lastLayer).size(); neurona++)
					{
						switch(topology[lastLayer].AF)
						{
							case ActivationFuntion::SIGMOID:
								dRdZ =  Perceptron<DATATYPE>::sigmoid_D(NEURONA(lastLayer,neurona).result);
								break;
							case ActivationFuntion::IDENTITY:
								dRdZ = DATATYPE(1);
								break;
							case ActivationFuntion::RELU:
								dRdZ = Perceptron<DATATYPE>::relu_D(NEURONA(lastLayer,neurona).result);
								break;
							default:
								throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
						};
						
						S += dEdZ * dRdZ;
						
						for(Index input = 0; input < NEURONA(lastLayer,neurona).inputs.size(); input++)
						{
							step = learning.ratio * S * NEURONA(lastLayer,neurona).result;
							WEIGHT(lastLayer,neurona,input) = WEIGHT(lastLayer,neurona,input) + step;	
						}				
					}
					//std::cout << "Step 1.0\n";
					
					//capa ocultas
					for(int indexLayer = lastLayer - 1; indexLayer >= 0; indexLayer--)
					{
						//std::cout << "Step 1.1.0\n";
						for(Index neurona = 0; neurona < LAYER(indexLayer).size(); neurona++)
						{		
							//std::cout << "Step 1.1.1.0\n";
							switch(topology[indexLayer].AF)
							{
								case ActivationFuntion::SIGMOID:
									dRdZ =  Perceptron<DATATYPE>::sigmoid_D(NEURONA(indexLayer,neurona).result);
									break;
								case ActivationFuntion::IDENTITY:
									dRdZ = DATATYPE(1);
									break;
								case ActivationFuntion::RELU:
									dRdZ = Perceptron<DATATYPE>::relu_D(NEURONA(indexLayer,neurona).result);
									break;
								default:
									throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
							};
							//std::cout << "Step 1.1.2.0\n";
							for(Index neuronaLast = 0; neuronaLast < LAYER(lastLayer).size(); neuronaLast++)
							{	
								for(Index weightLast = 0; weightLast < NEURONA(lastLayer,neurona).weight.size(); weightLast++)
								{
									for(Index weight = 0; weight < NEURONA(indexLayer,neurona).weight.size(); weight++)
									{
										//std::cout << "Step 1.1.2.1.0\n";
										step = learning.ratio * S * WEIGHT(indexLayer,neuronaLast,weightLast) * dRdZ * (*INPUT(indexLayer,neurona,weight));
										//std::cout << "Step 1.1.2.2.0\n";
										WEIGHT(indexLayer,neurona,weight) = WEIGHT(indexLayer,neurona,weight) + step;
										//std::cout << "Step 1.1.2.3.0\n";
									}
								}
							}
							//std::cout << "Step 1.1.3.0\n";
						}
					}
					
					//std::cout << "Step 2.0\n";
				}
				
			}
			if(plotting != NULL)
			{
				plotting->plotter.set_terminal("svg");
				plotting->plotter.set_output("out.svg");
				plotting->plotter.plottingFile2D(plotting->filename);
				filePlotting.flush();
				filePlotting.close();
			}
			return false;
		}

	
	/*real Network::changes_outputlayer(const std::vector<Data<DATATYPE>>& datas)
	{
		Index lastLayer = size()-1;
		real dEdZ,dRdZ,dZdW,step,S =0;
		for(Index indexData = 0; indexData < datas.size(); indexData++)
		{
			dEdZ = (*LAYER(size()-1).get_outputs()[0]) - datas[indexData].outputs[0];
						
			for(Index neurona = 0; neurona < LAYER(lastLayer).size(); neurona++)
			{
				switch(topology[lastLayer].AF)
				{
					case ActivationFuntion::SIGMOID:
						dRdZ =  Perceptron<DATATYPE>::sigmoid_D(std::vector<Layer<DATATYPE>>::at(lastLayer).at(neurona).result);
						break;
					case ActivationFuntion::IDENTITY:
						dRdZ = DATATYPE(1);
						break;
					case ActivationFuntion::RELU:
						dRdZ = Perceptron<DATATYPE>::relu_D(std::vector<Layer<DATATYPE>>::at(lastLayer).at(neurona).result);
						break;
					default:
						throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
				};
				
				S += dEdZ * dRdZ;
				
				for(Index input = 0; input < NEURONA(lastLayer,neurona).inputs.size(); input++)
				{
					S += dEdZ * dRdZ;

					dZdW = std::vector<Layer<DATATYPE>>::at(lastLayer).at(neurona).result;
					
					step = dEdZ * dRdZ * dZdW;
					WEIGHT(lastLayer,neurona,input) = WEIGHT(lastLayer,neurona,input) - step;	
				}				
			}		
		}

	}*/
	
}
