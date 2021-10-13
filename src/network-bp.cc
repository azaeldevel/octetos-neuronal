
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "network.hh"

namespace oct::neu
{
		bool Network::bp(const std::vector<Data<DATATYPE>>& datas, Learning<DATATYPE>& learning, Plotting<DATATYPE>* plotting)
		{
			Index lastLayer = size() - 1;//optener la ultima capa
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
			//std::cout << "Step 1.0\n";
			
			std::vector<DATATYPE> Se;
			Se.resize(size()+1);
			
			for(Index it = 0; it < learning.iterations; it++)
			{		
				Se[lastLayer+1] = dMSEdR(datas);
				
				if(plotting != NULL)
				{
					plotting->last++;
					oct::math::Plotter::save(filePlotting,plotting->last,Se[lastLayer+1]);
					filePlotting.flush();
					plotting->plotter.plottingFile2D(plotting->filename);
				}

				if(Se[lastLayer+1]  < learning.mE) return true;
				
				DATATYPE dRdZ;
				for(int indexLayer = lastLayer; indexLayer >= 0; indexLayer--)
				{
					dRdZ = 0;
					for(Index neurona = 0; neurona < LAYER(indexLayer).size(); neurona++)
					{
						switch(topology[indexLayer].AF)
						{
							case ActivationFuntion::SIGMOIDEA:
								dRdZ =  Perceptron<DATATYPE>::sigmoidea_D(std::vector<Layer<DATATYPE>>::at(indexLayer).at(neurona).result);
							break;
							case ActivationFuntion::IDENTITY:
								dRdZ = DATATYPE(1);
							break;
							case ActivationFuntion::RELU:
								dRdZ = Perceptron<DATATYPE>::relu_D(std::vector<Layer<DATATYPE>>::at(indexLayer).at(neurona).result);
							break;
							default:
								throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
						};
						DATATYPE step;						
						for(Index input = 0; input < NEURONA(indexLayer,neurona).inputs.size(); input++)
						{
							step = learning.ratio * Se[indexLayer+1] * dRdZ * (*INPUT(indexLayer,neurona,input));
							WEIGHT(indexLayer,neurona,input) = WEIGHT(indexLayer,neurona,input) - step;
						}
					}
					Se[indexLayer] = Se[indexLayer+1] * dRdZ;
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
}
